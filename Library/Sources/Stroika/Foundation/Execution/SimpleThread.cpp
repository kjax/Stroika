/*
 * Copyright(c) Records For Living, Inc. 2004-2011.  All rights reserved
 */
#include	"../StroikaPreComp.h"

#include	"../Configuration/StroikaConfig.h"

#if		defined (_WIN32)
	#include	<process.h>
	#include	<windows.h>
#endif

#include	"DLLSupport.h"
#include	"../Debug/Trace.h"
#include	"../Containers/VectorUtils.h"
#include	"../Time/Realtime.h"
#include	"OS_Win32/WaitSupport.h"

#include	"SimpleThread.h"



using	namespace	Stroika::Foundation;

using	Time::TickCountType;



// Leave this off by default since I'm not sure its safe, and at best it uses some time. But make it
// easy to turn on it release builds...
//		-- LGP 2009-05-28
// According to http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx - its best NOT to call this RaiseException call
// unless a debugger is present. Use IsDebuggerPresent(). Still not perfect.
//
//#define	qSupportSetThreadNameDebuggerCall	0
#ifndef	qSupportSetThreadNameDebuggerCall
	#if		defined (_DEBUG)
		#define	qSupportSetThreadNameDebuggerCall	1
	#endif
#endif
#ifndef	qSupportSetThreadNameDebuggerCall
	#define	qSupportSetThreadNameDebuggerCall	0
#endif



using	namespace	Characters;
using	namespace	Execution;


// RE-Examine altenrate appraoches to this and beter docuemnt!!!!
//
//		http://bugzilla/show_bug.cgi?id=646
#ifndef	qUseTLSForSAbortingFlag
#define	qUseTLSForSAbortingFlag		0
#endif
#ifndef	qUseSleelExForSAbortingFlag
#define	qUseSleelExForSAbortingFlag	1
#endif


#if		qUseTLSForSAbortingFlag
__declspec( thread )	bool	s_Aborting	=	false;
#endif



namespace	{
	#if (_WIN32_WINNT < 0x0502)
	namespace XXX {
		struct CLIENT_ID {
			DWORD UniqueProcess; 
			DWORD UniqueThread;
		};
		typedef LONG NTSTATUS;
		#define STATUS_SUCCESS ((NTSTATUS) 0x00000000)
		typedef LONG KPRIORITY;
		struct	THREAD_BASIC_INFORMATION {
			NTSTATUS   ExitStatus;
			PVOID      TebBaseAddress;
			CLIENT_ID  ClientId;
			KAFFINITY  AffinityMask;
			KPRIORITY  Priority;
			KPRIORITY  BasePriority;
		};
		enum THREAD_INFORMATION_CLASS {
			ThreadBasicInformation = 0,
		};
		typedef NTSTATUS (__stdcall *pfnNtQueryInformationThread) (HANDLE, THREAD_INFORMATION_CLASS, PVOID, ULONG, PULONG);
	}
	#endif
int MyGetThreadId (HANDLE thread)
	{
		#if (_WIN32_WINNT >= 0x0502)
			return ::GetThreadId (thread);
		#else
			// See details in http://www.codeguru.com/forum/showthread.php?t=355572 on this... - backcompat - only support
			// GetThreadId (HANDLE) in Win 2003 Server or later
			using namespace XXX;
			static	DLLLoader	ntdll (_T ("ntdll.dll"));
			static	pfnNtQueryInformationThread NtQueryInformationThread = (pfnNtQueryInformationThread)ntdll.GetProcAddress ("NtQueryInformationThread");
			if (NtQueryInformationThread == NULL)
				return 0;	// failed to get proc address
			THREAD_BASIC_INFORMATION	tbi;
			THREAD_INFORMATION_CLASS	tic = ThreadBasicInformation;
			if (NtQueryInformationThread (thread, tic, &tbi, sizeof (tbi), NULL) != STATUS_SUCCESS) {
				return 0;
			}
			return tbi.ClientId.UniqueThread;
		#endif
	}
}




using	Debug::TraceContextBumper;

/*
 ********************************************************************************
 ******************************** SimpleThread::Rep *****************************
 ********************************************************************************
 */
SimpleThread::Rep::Rep ()
	:fThread (INVALID_HANDLE_VALUE)
	,fStatusCriticalSection ()
	,fStatus (eNotYetRunning)
	,fOK2StartEvent (false, false)
	,fRefCountBumpedEvent (false, false)
{
	TraceContextBumper ctx (_T ("SimpleThread::Rep::Rep"));
	fThread = reinterpret_cast<HANDLE> (::_beginthreadex (NULL, 0, &Rep::ThreadProc, this, 0, NULL));
	if (fThread == NULL) {
		ThrowIfError_errno_t ();	// I THINK errno sb set, but in case not, do Win32 / GetLastError throw
		Win32ErrorException::DoThrow (::GetLastError ());
	}
	try {
		fRefCountBumpedEvent.Wait ();	// assure we wait for this, so we don't ever let refcount go to zero before the
										// thread has started...
	}
	catch (...) {
		::CloseHandle (fThread);
		Execution::DoReThrow ();
	}
}

SimpleThread::Rep::~Rep ()
{
	Assert (fStatus != eRunning);
	if (fThread != INVALID_HANDLE_VALUE) {
		::CloseHandle (fThread);
	}
}

void	SimpleThread::Rep::DO_DELETE_REF_CNT ()
{
	// nothing todo, cuz we don't want to get deleted twice!!!'
	// See docs for RefCntPtr<>
}

unsigned int	__stdcall	SimpleThread::Rep::ThreadProc (void* lpParameter)
{
	RequireNotNil (lpParameter);
	/*
	 * NB: It is important that we do NOT call ::_endthreadex () here because that would cause the
	 * RefCntPtr<> here to NOT be destroyed. We could force that with an explicit scope, but there
	 * is no need, since the docs for _beginthreadex () say that _endthreadex () is called automatically.
	 */
	Rep*			rep			=	reinterpret_cast<Rep*> (lpParameter);
	RefCntPtr<Rep>	incRefCnt	=	RefCntPtr<Rep> (RefCntPtr<Rep>::eUsesRefCntPtrBase, rep);	// assure refcount incremented so object not deleted while the thread is running

	incRefCnt->fRefCountBumpedEvent.Set ();

	incRefCnt->fOK2StartEvent.Wait ();	// we used to 'SuspendThread' but that was flakey. Instead - wait until teh caller says
										// we really want to start this thread.
	try {
		DbgTrace ("In SimpleThread::Rep::ThreadProc - setting state to RUNNING for thread= 0x%x", MyGetThreadId (incRefCnt->fThread));
		bool	doRun	=	false;
		{
			AutoCriticalSection enterCritcalSection (incRefCnt->fStatusCriticalSection);
			if (incRefCnt->fStatus == eNotYetRunning) {
				incRefCnt->fStatus = eRunning;
				doRun = true;
			}
		}
		if (doRun) {
			incRefCnt->Run ();
		}
		DbgTrace ("In SimpleThread::Rep::ThreadProc - setting state to COMPLETED for thread= 0x%x", MyGetThreadId (incRefCnt->fThread));
		{
			AutoCriticalSection enterCritcalSection (incRefCnt->fStatusCriticalSection);
			incRefCnt->fStatus = eCompleted;
		}
		return 0;
	}
	catch (ThreadAbortException&) {
		DbgTrace ("In SimpleThread::Rep::ThreadProc - setting state to COMPLETED (ThreadAbortException) for thread= 0x%x", MyGetThreadId (incRefCnt->fThread));
		AutoCriticalSection enterCritcalSection (incRefCnt->fStatusCriticalSection);
		incRefCnt->fStatus = eCompleted;
		return 0;
	}
	catch (...) {
		DbgTrace ("In SimpleThread::Rep::ThreadProc - setting state to COMPLETED (EXCEPT) for thread= 0x%x", MyGetThreadId (incRefCnt->fThread));
		AutoCriticalSection enterCritcalSection (incRefCnt->fStatusCriticalSection);
		incRefCnt->fStatus = eCompleted;
		return -1;
	}
}

void	SimpleThread::Rep::NotifyOfAbort ()
{
	Require (fStatus == eAborting or fStatus == eCompleted);
	// CAREFUL WHEN OVERRIDING CUZ CALLED TYPICALLY FROM ANOTHER  THREAD!!!
	AutoCriticalSection enterCritcalSection (fStatusCriticalSection);
	if (::GetCurrentThreadId () == MyGetThreadId_ ()) {
		ThrowAbortIfNeeded ();
	}
	if (fStatus == eAborting) {
		#if		qUseTLSForSAbortingFlag
			s_Aborting = true;
		#endif
		Verify (::QueueUserAPC (&AbortProc_, fThread, reinterpret_cast<ULONG_PTR> (this)));
	}
}

void	CALLBACK	SimpleThread::Rep::AbortProc_ (ULONG_PTR lpParameter)
{
	TraceContextBumper ctx (_T ("SimpleThread::Rep::AbortProc_"));
	SimpleThread::Rep*	rep	=	reinterpret_cast<SimpleThread::Rep*> (lpParameter);
	Require (rep->fStatus == eAborting || rep->fStatus == eCompleted);
	rep->ThrowAbortIfNeeded ();
	Require (rep->fStatus == eCompleted);	// normally we don't reach this - but we could if we've already been marked completed somehow
											// before the abortProc got called/finsihed...
	//Assert (false);
}

int	SimpleThread::Rep::MyGetThreadId_ () const
{
	return MyGetThreadId (fThread);
}






namespace	{
	class	RunOnceRep_ : public SimpleThread::Rep {
		public:
			RunOnceRep_ (void (*fun2CallOnce) (void* arg), void* arg):
				fFun2CallOnce (fun2CallOnce),
				fArg (arg)
				{
				}
		protected:
			override	void	Run ()
				{
					(fFun2CallOnce) (fArg);
				}

		private:
			void	(*fFun2CallOnce) (void* arg);
			void*	fArg;
	};
}


namespace	{
	class	RunnableRunRep_ : public SimpleThread::Rep {
		private:
			RefCntPtr<IRunnable>	fRunnable;
		public:
			RunnableRunRep_ (const RefCntPtr<IRunnable>& runnable)
				: fRunnable (runnable)
				{
				}
		protected:
			override	void	Run ()
				{
					fRunnable->Run ();
				}
	};
}





/*
 ********************************************************************************
 *********************************** SimpleThread *******************************
 ********************************************************************************
 */
SimpleThread::SimpleThread ():
	fRep ()
{
}

SimpleThread::SimpleThread (const RefCntPtr<Rep>& threadObj):
	fRep (threadObj)
{
}

SimpleThread::SimpleThread (Rep* newThreadObj):
	fRep (RefCntPtr<Rep> (RefCntPtr<Rep>::eUsesRefCntPtrBase, newThreadObj))
{
}

SimpleThread::SimpleThread (void (*fun2CallOnce) (void* arg), void* arg)
	: fRep (RefCntPtr<Rep> (RefCntPtr<Rep>::eUsesRefCntPtrBase, DEBUG_NEW RunOnceRep_ (fun2CallOnce, arg)))
{
}

SimpleThread::SimpleThread (const RefCntPtr<IRunnable>& runnable)
	: fRep (RefCntPtr<Rep> (RefCntPtr<Rep>::eUsesRefCntPtrBase, DEBUG_NEW RunnableRunRep_ (runnable)))
{
}

void	SimpleThread::SetThreadPriority (int nPriority)
{
	RequireNotNil (fRep);
	Verify (::SetThreadPriority (GetOSThreadHandle (), nPriority));
}

void	SimpleThread::SetThreadName (const wstring& threadName)
{
	if (fRep->fThreadName != threadName) {
		TraceContextBumper	ctx (_T ("Execution::SimpleThread::SetThreadName"));
		DbgTrace (L"(ThreadName = '%s')", threadName.c_str ());
		fRep->fThreadName = threadName;
		#if		qSupportSetThreadNameDebuggerCall
			if (::IsDebuggerPresent ()) {
				// This hack from http://www.codeproject.com/KB/threads/Name_threads_in_debugger.aspx
				struct THREADNAME_INFO {
					DWORD dwType;		// must be 0x1000
					LPCSTR szName;		// pointer to name (in user addr space)
					DWORD dwThreadID;	// thread ID (-1=caller thread)
					DWORD dwFlags;		// reserved for future use, must be zero
				};
				string	useThreadName	=	WideStringToACP (threadName);
				THREADNAME_INFO info;
				{
					info.dwType = 0x1000;
					info.szName = useThreadName.c_str ();
					info.dwThreadID = MyGetThreadId (fRep->fThread);
					info.dwFlags = 0;
				}
				IgnoreExceptionsForCall (::RaiseException (0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info));
			}
		#endif
	}
}

void	SimpleThread::Start ()
{
	RequireNotNil (fRep);
	Assert (fRep->fThread != INVALID_HANDLE_VALUE);
	DbgTrace (L"SimpleThread::Start: (thread = 0x%x, name='%s')", MyGetThreadId (fRep->fThread), fRep->fThreadName.c_str ());
	fRep->fOK2StartEvent.Set ();
}

void	SimpleThread::Stop ()
{
	// I'm not sure this is 100% thread-friendly, in case two people from two differnt threads tried
	// to stop the same (third) thread at the same time. But its probably good enough for starters.
	//		-- LGP 2009-01-14
	RequireNotNil (fRep);

	// You cannot call STOP from within the thread you are stopping! Calling stop would cause a throw out - preventing the stop...
	Require (::GetCurrentThreadId () != MyGetThreadId (fRep->fThread));

	// first try to send abort exception, and then - if force - get serious!
	AutoCriticalSection enterCritcalSection (fRep->fStatusCriticalSection);
	if (fRep->fStatus != eCompleted) {
		fRep->fStatus = eAborting;
	}
	if (fRep->fStatus == eAborting) {
		// by default - tries to trigger a throw-abort-excption in the right thread using UNIX signals or QueueUserAPC ()
		fRep->NotifyOfAbort ();
	}
}

void	SimpleThread::Stop_Forced_Unsafe ()
{
	// You cannot call STOP from within the thread you are stopping! Calling stop would cause a throw out - preventing the stop...
	Require (::GetCurrentThreadId () != MyGetThreadId (fRep->fThread));

	Stop ();

	// Wait some reasonable amount of time for the thread to abort
	IgnoreExceptionsForCall (WaitForDone (5.0f));
	AutoCriticalSection enterCritcalSection (fRep->fStatusCriticalSection);
	if (fRep->fStatus != eCompleted and fRep->fThread != INVALID_HANDLE_VALUE) {
		// This is VERY bad to do. Put assert here that it never happens...
		Assert (false);
		::TerminateThread (fRep->fThread, -1);
	}
}

void	SimpleThread::WaitForDone (float timeout) const
{
	RequireNotNil (fRep);
	bool	doWait	=	false;
	HANDLE	thread	=	NULL;
	{
		AutoCriticalSection enterCritcalSection (fRep->fStatusCriticalSection);
		if (fRep->fThread != INVALID_HANDLE_VALUE and fRep->fStatus != eCompleted) {
			doWait = true;
			thread = fRep->fThread;
		}
	}
	if (doWait) {
		DWORD	dTimeout	=	timeout < 0.0f? UINT_MAX: (DWORD)(timeout * 1000);
		if (::WaitForSingleObject (thread, dTimeout) == WAIT_TIMEOUT) {
			Win32ErrorException::DoThrow (WAIT_TIMEOUT);
		}
	}
}

void	SimpleThread::PumpMessagesAndReturnWhenDoneOrAfterTime (float timeToPump) const
{
	HANDLE	thread	=	NULL;
	{
		AutoCriticalSection enterCritcalSection (fRep->fStatusCriticalSection);
		if (fRep->fThread != INVALID_HANDLE_VALUE and fRep->fStatus != eCompleted) {
			thread = fRep->fThread;
		}
	}
	if (thread != NULL) {
		OS_Win32::WaitAndPumpMessages (NULL, Containers::mkV<HANDLE> (thread), timeToPump);
	}
}

void	SimpleThread::WaitForDoneWhilePumpingMessages (float timeout) const
{
	TickCountType	timeoutAt	=	Time::GetTickCount () + timeout;
	// CRUDDY impl - but decent enuf for first draft
	while (GetStatus () != SimpleThread::eCompleted) {
		if (timeout < 0.0f) {
			PumpMessagesAndReturnWhenDoneOrAfterTime ();
		}
		else {
			TickCountType	time2Wait	=	timeoutAt - Time::GetTickCount ();
			if (time2Wait <= 0) {
				Win32ErrorException::DoThrow (WAIT_TIMEOUT);
			}
			PumpMessagesAndReturnWhenDoneOrAfterTime (static_cast<float> (time2Wait));
		}
	}
}

SimpleThread::Status	SimpleThread::GetStatus () const
{
	if (fRep.IsNull ()) {
		return eNull;
	}
	AutoCriticalSection enterCritcalSection (fRep->fStatusCriticalSection);
	return fRep->fStatus;
}







/*
 ********************************************************************************
 ************************* Execution::CheckForThreadAborting ********************
 ********************************************************************************
 */
void	Execution::CheckForThreadAborting ()
{
	#if		qUseTLSForSAbortingFlag
		if (s_Aborting) {
			Execution::DoThrow (ThreadAbortException ());
		}
	#elif	qUseSleelExForSAbortingFlag
		::SleepEx (0, true);
	#else
		Assert (false);
		/*
			* I think we could use SleepEx() or WaitForMultipleObjectsEx(), but SleepEx(0,true) may cause a thread to give up
			* the CPU (ask itself to be rescheduled). WaitForMultipleObjectsEx - from the docs - doesn't appear to do this.
			* I think its a lower-cost way to check for a thread being aborted...
			*			-- LGP 2010-10-26
			*/
//				(void)::WaitForMultipleObjectsEx (0, NULL, false, 0, true);
	#endif
}
