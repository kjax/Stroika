/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Execution_Event_inl_
#define	_Stroika_Foundation_Execution_Event_inl_	1


/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include	"AtomicOperations.h"
#include	"WaitAbandonedException.h"
#include	"WaitTimedOutException.h"

namespace	Stroika {	
	namespace	Foundation {
		namespace	Execution {



		// class	Event
			inline	Event::Event (bool manualReset, bool initialState)
#if			qPlatform_Windows
				: fEventHandle (::CreateEvent (NULL, manualReset, initialState, NULL))
#endif
				{
					#if			qPlatform_Windows
						ThrowIfFalseGetLastError (fEventHandle != NULL);
						#if		qTrack_Execution_HandleCounts
							Execution::AtomicIncrement (&sCurAllocatedHandleCount);
						#endif
					#else
						AssertNotImplemented ();
					#endif
				}
			inline	Event::~Event ()
				{
					#if			qPlatform_Windows
						Verify (::CloseHandle (fEventHandle));
						#if		qTrack_Execution_HandleCounts
							AtomicDecrement (&sCurAllocatedHandleCount);
						#endif
					#else
						AssertNotImplemented ();
					#endif
				}
			inline	void	Event::Pulse() throw ()
				{
					#if			qPlatform_Windows
						AssertNotNil (fEventHandle);
						Verify (::PulseEvent (fEventHandle));
					#else
						AssertNotImplemented ();
					#endif
				}
			inline	void	Event::Reset () throw ()
				{
					#if			qPlatform_Windows
						AssertNotNil (fEventHandle);
						Verify (::ResetEvent (fEventHandle));
					#else
						AssertNotImplemented ();
					#endif
				}
			inline	void	Event::Set () throw ()
				{
					#if			qPlatform_Windows
						AssertNotNil (fEventHandle);
						Verify (::SetEvent (fEventHandle));
					#else
						AssertNotImplemented ();
					#endif
				}
			inline	void	Event::Wait (float timeout) const
				{
					#if			qPlatform_Windows
						AssertNotNil (fEventHandle);
						DWORD	milliseconds	=	static_cast<DWORD> (timeout * 1000);
						if (timeout > 1000) {
							milliseconds = INFINITE;	// must be careful about rounding errors in int->float->int
						}
						DWORD	result	=	::WaitForSingleObject (fEventHandle, milliseconds);
						switch (result) {
							case	WAIT_TIMEOUT:	DoThrow (WaitTimedOutException ());
							case	WAIT_ABANDONED:	DoThrow (WaitAbandonedException ());
						}
						Verify (result == WAIT_OBJECT_0);
					#else
						AssertNotImplemented ();
					#endif
				}
			#if			qPlatform_Windows
			inline	Event::operator HANDLE () const
				{
					AssertNotNil (fEventHandle);
					return fEventHandle;
				}
			#endif


		}
	}
}
#endif	/*_Stroika_Foundation_Execution_Event_inl_*/
