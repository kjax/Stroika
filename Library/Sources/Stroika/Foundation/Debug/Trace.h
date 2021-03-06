/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#ifndef _Stroika_Foundation_Debug_Trace_h_
#define _Stroika_Foundation_Debug_Trace_h_  1

#include    "../StroikaPreComp.h"

#if     qPlatform_Windows
#include    <windows.h>
#include    <tchar.h>
#endif

#include    "../Characters/TChar.h"
#include    "../Characters/TString.h"
#include    "../Configuration/Common.h"
#include    "../Execution/ModuleInit.h"
#include    "../Time/Realtime.h"



namespace   Stroika {
    namespace   Foundation {
        namespace   Debug {


            using   Characters::TString;
            using   Characters::TChar;


            /*
             *  So we can distribute near-release versions of HF to users, and get feedback - a trace file
             *  to help debug problems we cannot reproduce.
             */
#if     !defined (qTraceToFile)
#error "qTraceToFile should normally be defined indirectly by StroikaConfig.h"
#endif



            /*
            @CONFIGVAR:     qDefaultTracingOn
            @DESCRIPTION:   <p>Led contains a limited amount of pre-built tracing code. This could be expanded someday,
                        depending on how useful people find it. This defaults to being on only for Windows and if @'qDebug' is
                        on (windows only cuz thats the only place I've implemented the trace message emitter so far).</p>
                            <p>See also @'qDebug' and @'DebugTrace'</p>
             */
#if     !defined (qDefaultTracingOn)
#error "qDefaultTracingOn should normally be defined indirectly by StroikaConfig.h"
#endif

            namespace   Private {
                struct  MODULE_INIT;
            };

            class   Emitter {
            private:
                Emitter ();
            public:
                static  Emitter&    Get ();

#if     qTraceToFile
            public:
                nonvirtual  TString GetTraceFileName () const;
#endif

            public:
                nonvirtual  void    EmitTraceMessage (const char* format, ...);
                nonvirtual  void    EmitTraceMessage (const wchar_t* format, ...);

                // The 'TraceLastBufferedWriteTokenType' overload of EmitTraceMessage allows you to specify a set of (trailing) characters to
                // be temporarily buffered. These characters are not immediately emitted, and can be cleared via UnputBufferedCharactersForMatchingToken ().
                // They will eventually be flushed out on the next call to EmitTraceMessage ().
                typedef int TraceLastBufferedWriteTokenType;
                nonvirtual  TraceLastBufferedWriteTokenType EmitTraceMessage (size_t bufferLastNChars, const char* format, ...);
                nonvirtual  TraceLastBufferedWriteTokenType EmitTraceMessage (size_t bufferLastNChars, const wchar_t* format, ...);

            public:
                // if the last write matches the given token (no writes since then) and the timestamp is unchanged, abandon
                // the buffered characters and return true. Else flush(write) them, and return false.
                nonvirtual  bool    UnputBufferedCharactersForMatchingToken (TraceLastBufferedWriteTokenType token);

                template    <typename   CHARTYPE>
                nonvirtual  void    EmitUnadornedText (const CHARTYPE* p);

            private:
                // This is the same as EmitTraceMessage_ - but it takes a plain string - and assumes the caller does any 'sprintf' stuff...
                template    <typename   CHARTYPE>
                nonvirtual  TraceLastBufferedWriteTokenType DoEmitMessage_ (size_t bufferLastNChars, const CHARTYPE* p, const CHARTYPE* e);

            private:
                size_t                          fLastNCharBufCharCount_;
                char                            fLastNCharBuf_CHAR_[10];
                wchar_t                         fLastNCharBuf_WCHAR_[10];
                bool                            fLastNCharBuf_WCHARFlag_;
                TraceLastBufferedWriteTokenType fLastNCharBuf_Token_;
                Time::DurationSecondsType       fLastNCharBuf_WriteTickcount_;

                nonvirtual  void    BufferNChars_ (size_t nChars, const char* p);
                nonvirtual  void    BufferNChars_ (size_t nChars, const wchar_t* p);

            private:
                nonvirtual  void    FlushBufferedCharacters_ ();

            private:
                nonvirtual  void    DoEmit_ (const char* p);
                nonvirtual  void    DoEmit_ (const wchar_t* p);
                nonvirtual  void    DoEmit_ (const char* p, const char* e);
                nonvirtual  void    DoEmit_ (const wchar_t* p, const wchar_t* e);

            private:
                friend  struct  Private::MODULE_INIT;
            };



            class   TraceContextBumper {
            public:
                TraceContextBumper ();
                TraceContextBumper (const TChar* contextName);
                ~TraceContextBumper ();

            private:
                NO_COPY_CONSTRUCTOR(TraceContextBumper);
                NO_ASSIGNMENT_OPERATOR(TraceContextBumper);

#if     qDefaultTracingOn
            public:
                bool    fDoEndMarker;
            private:
                TChar                                       fSavedContextName_[64];
                Emitter::TraceLastBufferedWriteTokenType    fLastWriteToken_;           // used to COMBINE items into a single line if they happen quickly enuf

            public:
                static  unsigned int    GetCount ();
            private:
                static  void    IncCount ();
                static  void    DecrCount ();
#endif
            };


            /*
            @METHOD:        DbgTrace
            @DESCRIPTION:   <p>This function either does NOTHING (trying to not even evaluate its arguments)
                        or does a printf style PRINT function by delegating to @'EmitTraceMessage'. Which of
                        these two behaviors you see is conditioned on @'qDefaultTracingOn'</p>
            */
#ifndef DbgTrace
#if     qDefaultTracingOn
#define DbgTrace    Stroika::Foundation::Debug::Emitter::Get ().EmitTraceMessage
#else
#define DbgTrace    _NoOp_
#endif
#endif



        }
    }
}
#endif  /*_Stroika_Foundation_Debug_Trace_h_*/





/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include    "Trace.inl"
