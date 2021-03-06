/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#ifndef _Stroika_Foundation_Execution_Logger_h_
#define _Stroika_Foundation_Execution_Logger_h_ 1

#include    "../StroikaPreComp.h"

#include    <cstdarg>

#include    "../Configuration/Common.h"
#include    "../Characters/String.h"
#include    "../Debug/Assertions.h"



namespace   Stroika {
    namespace   Foundation {
        namespace   Execution {

            using   namespace   Configuration;

            using   Characters::String;




            /*
             *  OVERVIEW:
             *
             *      The point of the Logging Module is to provide a simple, portable wrapper on end-user-targetted application logging.
             *  This form of logging is the kind of logging you leave builtin to your application, and write focused on end-user readability.
             *  It is NOT (primarily) for debugging (for that - use the Stroika::Foundation::Debug::Trace module).
             *
             *      Think of this as a simple/portable wrapper on syslog/log4j/WindowsEventlog.
             *
             *      The Logger is a singleton object. It can be set at any number of application logging levels. And it will write information to the
             *  backend logger its setup with. But default - this is none.
             *
             *      To use the logger and actually get logging - pick a logger rep, and call SetLoggingAppender ();
             *
             *      This logging API CANNOT be used before main () has started, or after main () has completed [if we choose to allow that we must use ModuleInit,
             *      but since this requires a call to setup a logger, that seems unlikely]
             *
             */
            class   Logger {
            public:
                // See syslog for enumeration of various targets/etc.

            public:
                class   IAppenderRep;
            private:
                shared_ptr<IAppenderRep> fRep_;

            public:
#if     qHas_Syslog
                class   SysLogAppender;
#endif
                class   FileAppender;
#if     qPlatform_Windows
                class   WindowsEventLogAppender;
#endif

            public:
                static  Logger& Get ();
            private:
                Logger ();

            public:
                nonvirtual  shared_ptr<IAppenderRep> GetAppender () const;
                nonvirtual  void                     SetAppender (const shared_ptr<IAppenderRep>& rep);

            private:
                shared_ptr<IAppenderRep> fAppender_;

            public:

                /*
                 * Stroika uses this facility for NO DEBUGGING - but and to be portbale, but
                    convenitnetly these numbers corredspond to teh SYSLOG values (so they can be substeid as needed).

                    NOTE: I don't think so many levels makes sense and I dont think these are well defined. I should probably trim the list,
                     and keep the

                     UPDATE DOCS
                 */
                enum    Priority {
                    // Names and numbers for syslog from http://unix.superglobalmegacorp.com/Net2/newsrc/sys/syslog.h.html
                    eInfo_P             =   6,              // SYSLOG name LOG_INFO     -   The message is purely informational
                    eNotice_P           =   5,              // SYSLOG name LOG_NOTICE   -   The message describes a normal but important event
                    eWarning_P          =   4,              // SYSLOG name LOG_WARNING  -   The message is a warning
                    eError_P            =   3,              // SYSLOG name LOG_ERR      -   The message describes an error
                    eCriticalError_P    =   2,              // SYSLOG name LOG_CRIT     -   The message states a critical condition
                    eAlertError_P       =   1,              // SYSLOG name LOG_ALERT    -   Action on the message must be taken immediately
                    eEmergency_P        =   0,              // SYSLOG name LOG_EMERG    -   The message says the system is unusable
                };
            public:
                // EITHER RENAME AND DO ORDER DIFRNELY OR DONT TIE NNUMBERS TO SYSLOG NUMBERS
                Priority    fMinLogLevel_;  // SB PRIVATE
                // Get/Set LogLevel - this affects what WE EAT INLINE versus passon



            public:
                // DoLog
                static  void    Log (Priority logLevel, const String& format, ...); // varargs logger
            private:
                static  void    Log_ (Priority logLevel, const String& format, va_list argList);


            private:
                static  Logger  sThe_;
            };


            class   Logger::IAppenderRep {
            public:
                virtual ~IAppenderRep ();
            public:
                virtual void    Log (Priority logLevel, const String& message) = 0;
            };
#if     qHas_Syslog
            class   Logger::SysLogAppender : public Logger::IAppenderRep {
            public:
                SysLogAppender (const String& applicationName);
                SysLogAppender (const String& applicationName, int facility);
                ~SysLogAppender ();
            public:
                virtual void    Log (Priority logLevel, const String& message) override;
            private:
                TString fApplicationName_;  // must save like this because open-syslog appears to KEEP ahold of pointer (http://www.gnu.org/s/hello/manual/libc/openlog.html)
            };
#endif
            class   Logger::FileAppender : public Logger::IAppenderRep {
            public:
                FileAppender (const String& fileName);
            public:
                virtual void    Log (Priority logLevel, const String& message) override;
            };

        }
    }
}
#endif  /*_Stroika_Foundation_Execution_Logger_h_*/




/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include    "Logger.inl"
