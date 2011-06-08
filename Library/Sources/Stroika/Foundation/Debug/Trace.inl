
/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#ifndef	_Trace_inl
#define	_Trace_inl	1

namespace	Stroika {	
	namespace	Foundation {
		namespace	Debug {


		//	class	Trace::Emitter
			template	<typename	CHARTYPE>
				inline	void	Emitter::EmitUnadornedText (const CHARTYPE* p)
					{
						DoEmit_ (p);
					}

		//	class	TraceContextBumper
			inline	TraceContextBumper::TraceContextBumper ()
				#if		qDefaultTracingOn
					: fDoEndMarker (false)
					//,fSavedContextName ()
				#endif
				{
					#if		qDefaultTracingOn
						IncCount ();
					#endif
				}
			inline	TraceContextBumper::TraceContextBumper (const TCHAR* contextName)
				#if		qDefaultTracingOn
					: fDoEndMarker (true)
					//,fSavedContextName ()
				#endif
				{
					#if		qDefaultTracingOn
						fLastWriteToken = Emitter::Get ().EmitTraceMessage (5, _T ("<%s> {\r\n"), contextName);
						_tcsncpy_s (fSavedContextName, contextName, NEltsOf (fSavedContextName)-1);
						fSavedContextName[NEltsOf(fSavedContextName)-1] = '\0';
						IncCount ();
					#endif
				}
			inline	TraceContextBumper::~TraceContextBumper ()
				{
					#if		qDefaultTracingOn
						DecrCount ();
						if (fDoEndMarker) {
							if (Emitter::Get ().UnputBufferedCharactersForMatchingToken (fLastWriteToken)) {
								Emitter::Get ().EmitUnadornedText ("/>\r\n");
							}
							else {
								Emitter::Get ().EmitTraceMessage (_T ("} </%s>"), fSavedContextName);
							}
						}
					#endif
				}


			namespace	Private {
				struct	MODULE_INIT {
					MODULE_INIT ();
					~MODULE_INIT ();

					Emitter	fEmitter;
				};
			}


		}
	}
}

namespace	{
	Stroika::Foundation::Execution::ModuleInitializer<Stroika::Foundation::Debug::Private::MODULE_INIT>	_TracePrivateINIT_;	// this object constructed for the CTOR/DTOR per-module side-effects
}

inline	Stroika::Foundation::Debug::Emitter&	Stroika::Foundation::Debug::Emitter::Get ()
	{
		return _TracePrivateINIT_.Actual ().fEmitter;
	}
#endif	/*_Trace_inl*/