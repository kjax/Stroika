/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#include    "../../../StroikaPreComp.h"

#if     qPlatform_Windows
#include    <Windows.h>
#include    <winerror.h>
#include    <wininet.h>     // for error codes
#else
#error "WINDOWS REQUIRED FOR THIS MODULE"
#endif

#include    "../../../Characters/StringUtils.h"
#include    "../../../Characters/Format.h"
#include    "../../../Configuration/Common.h"
#include    "../../../Containers/Common.h"
#include    "../../../Debug/Trace.h"
#include    "../../../Execution/WaitTimedOutException.h"
#if     qPlatform_Windows
#include    "HRESULTErrorException.h"
#endif
#include    "../../../IO/FileAccessException.h"
#include    "../../../IO/FileBusyException.h"
#include    "../../../Time/Realtime.h"

#include    "Exception.h"


using   namespace   Stroika;
using   namespace   Stroika::Foundation;
using   namespace   Stroika::Foundation::Characters;
using   namespace   Stroika::Foundation::Debug;
using   namespace   Stroika::Foundation::Execution;
using   namespace   Stroika::Foundation::Execution::Platform;
using   namespace   Stroika::Foundation::Execution::Platform::Windows;






// for InternetGetConnectedState
#pragma comment(lib, "Wininet.lib")





namespace {
    inline  TString Win32Error2String_ (DWORD win32Err)
    {
        switch (win32Err) {
            case    ERROR_NOT_ENOUGH_MEMORY:
                return TSTR ("Not enough memory to complete that operation (ERROR_NOT_ENOUGH_MEMORY)");
            case    ERROR_OUTOFMEMORY:
                return TSTR ("Not enough memory to complete that operation (ERROR_OUTOFMEMORY)");
        }
        if (INTERNET_ERROR_BASE <= win32Err and win32Err < INTERNET_ERROR_BASE + INTERNET_ERROR_LAST) {
            switch (win32Err) {
                case    ERROR_INTERNET_INVALID_URL:
                    return TSTR ("ERROR_INTERNET_INVALID_URL");
                case    ERROR_INTERNET_CANNOT_CONNECT:
                    return TSTR ("Failed to connect to internet URL (ERROR_INTERNET_CANNOT_CONNECT)");
                case    ERROR_INTERNET_NAME_NOT_RESOLVED:
                    return TSTR ("ERROR_INTERNET_NAME_NOT_RESOLVED");
                case    ERROR_INTERNET_INCORRECT_HANDLE_STATE:
                    return TSTR ("ERROR_INTERNET_INCORRECT_HANDLE_STATE");
                case    ERROR_INTERNET_TIMEOUT:
                    return TSTR ("Operation timed out (ERROR_INTERNET_TIMEOUT)");
                case    ERROR_INTERNET_CONNECTION_ABORTED:
                    return TSTR ("ERROR_INTERNET_CONNECTION_ABORTED");
                case    ERROR_INTERNET_CONNECTION_RESET:
                    return TSTR ("ERROR_INTERNET_CONNECTION_RESET");
                case    ERROR_HTTP_INVALID_SERVER_RESPONSE:
                    return TSTR ("Invalid Server Response (ERROR_HTTP_INVALID_SERVER_RESPONSE)");
                case    ERROR_INTERNET_PROTOCOL_NOT_FOUND: {
                        DWORD   r = 0;
                        if (::InternetGetConnectedState (&r, 0) and (r & INTERNET_CONNECTION_OFFLINE) == 0) {
                            return TSTR ("ERROR_INTERNET_PROTOCOL_NOT_FOUND");
                        }
                        else {
                            return TSTR ("ERROR_INTERNET_PROTOCOL_NOT_FOUND (offline mode)");
                        }
                    }
                default: {
                        TCHAR   buf[1024];
                        (void)::_stprintf_s (buf, TSTR ("INTERNET error code: %d"), win32Err);
                        return buf;
                    }
            }
        }
        TCHAR*  lpMsgBuf    =   nullptr;
        if (not ::FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 nullptr,
                                 win32Err,
                                 MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                                 reinterpret_cast<TCHAR*> (&lpMsgBuf),
                                 0,
                                 nullptr)
           ) {
            return Format (TSTR ("Win32 error# %d"), static_cast<DWORD> (win32Err));
        }
        TString result  =   lpMsgBuf;
        ::LocalFree (lpMsgBuf);
        return Trim (result);
    }
}










/*
 ********************************************************************************
 *********************** Platform::Windows::Exception ***************************
 ********************************************************************************
 */
void    Execution::Platform::Windows::Exception::DoThrow (DWORD error)
{
    switch (error) {
        case    ERROR_SUCCESS: {
                DbgTrace ("Platform::Windows::Exception::DoThrow (ERROR_SUCCESS) - throwing Platform::Windows::Exception (ERROR_NOT_SUPPORTED)");
                throw Platform::Windows::Exception (ERROR_NOT_SUPPORTED);   // unsure WHAT to throw here - SOMETHING failed - but GetLastError () must have given
                // a bad reason code?
            }
        case    ERROR_NOT_ENOUGH_MEMORY:
        case    ERROR_OUTOFMEMORY: {
                DbgTrace ("Platform::Windows::Exception::DoThrow (0x%x) - throwing bad_alloc", error);
                throw bad_alloc ();
            }
        case    ERROR_SHARING_VIOLATION: {
                DbgTrace ("Platform::Windows::Exception::DoThrow (0x%x) - throwing FileBusyException", error);
                throw IO::FileBusyException ();
            }
        case ERROR_FILE_NOT_FOUND: {
                DbgTrace ("Platform::Windows::Exception::DoThrow (0x%x) - throwing FileAccessException", error);
                throw IO::FileAccessException ();   // don't know if they were reading or writing at this level..., and don't know file name...
            }
        case ERROR_PATH_NOT_FOUND: {
                DbgTrace ("Platform::Windows::Exception::DoThrow (0x%x) - throwing FileAccessException", error);
                throw IO::FileAccessException ();   // don't know if they were reading or writing at this level..., and don't know file name...
            }
        case WAIT_TIMEOUT: {
                DbgTrace ("Platform::Windows::Exception::DoThrow (0x%x) - throwing WaitTimedOutException", error);
                throw Execution::WaitTimedOutException ();
            }
        default: {
                DbgTrace ("Platform::Windows::Exception::DoThrow (0x%x) - throwing Platform::Windows::Exception", error);
                throw Platform::Windows::Exception (error);
            }
    }
}

TString Execution::Platform::Windows::Exception::LookupMessage (DWORD dw)
{
    return Win32Error2String_ (dw);
}






/*
 ********************************************************************************
 ***************************** ThrowIfShellExecError ****************************
 ********************************************************************************
 */
void    Execution::Platform::Windows::ThrowIfShellExecError (HINSTANCE r)
{
    int errCode =   reinterpret_cast<int> (r);
    if (errCode <= 32) {
        DbgTrace ("ThrowIfShellExecError (0x%x) - throwing exception", errCode);
        switch (errCode) {
            case    0:
                Platform::Windows::Exception::DoThrow (ERROR_NOT_ENOUGH_MEMORY);    // The operating system is out of memory or resources.
            case    ERROR_FILE_NOT_FOUND:
                Platform::Windows::Exception::DoThrow (ERROR_FILE_NOT_FOUND);   // The specified file was not found.
            case    ERROR_PATH_NOT_FOUND:
                Platform::Windows::Exception::DoThrow (ERROR_PATH_NOT_FOUND);   //  The specified path was not found.
            case    ERROR_BAD_FORMAT:
                Platform::Windows::Exception::DoThrow (ERROR_BAD_FORMAT);       //  The .exe file is invalid (non-Microsoft Win32� .exe or error in .exe image).
            case    SE_ERR_ACCESSDENIED:
                throw (Platform::Windows::HRESULTErrorException (E_ACCESSDENIED));          //  The operating system denied access to the specified file.
            case    SE_ERR_ASSOCINCOMPLETE:
                Platform::Windows::Exception::DoThrow (ERROR_NO_ASSOCIATION);   //  The file name association is incomplete or invalid.
            case    SE_ERR_DDEBUSY:
                Platform::Windows::Exception::DoThrow (ERROR_DDE_FAIL);         //  The Dynamic Data Exchange (DDE) transaction could not be completed because other DDE transactions were being processed.
            case    SE_ERR_DDEFAIL:
                Platform::Windows::Exception::DoThrow (ERROR_DDE_FAIL);         //  The DDE transaction failed.
            case    SE_ERR_DDETIMEOUT:
                Platform::Windows::Exception::DoThrow (ERROR_DDE_FAIL);         //  The DDE transaction could not be completed because the request timed out.
            case    SE_ERR_DLLNOTFOUND:
                Platform::Windows::Exception::DoThrow (ERROR_DLL_NOT_FOUND);        //  The specified dynamic-link library (DLL) was not found.
                //case  SE_ERR_FNF:             throw (Platform::Windows::Exception (ERROR_FILE_NOT_FOUND));        //  The specified file was not found.
            case    SE_ERR_NOASSOC:
                Platform::Windows::Exception::DoThrow (ERROR_NO_ASSOCIATION);   //  There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable.
            case    SE_ERR_OOM:
                Platform::Windows::Exception::DoThrow (ERROR_NOT_ENOUGH_MEMORY);    //  There was not enough memory to complete the operation.
                //case  SE_ERR_PNF:             throw (Platform::Windows::Exception (ERROR_PATH_NOT_FOUND));        //  The specified path was not found.
            case    SE_ERR_SHARE:
                Platform::Windows::Exception::DoThrow (ERROR_INVALID_SHARENAME);    //
            default: {
                    // Not sure what error to report here...
                    Platform::Windows::Exception::DoThrow (ERROR_NO_ASSOCIATION);
                }
        }
    }
}









/*
 ********************************************************************************
 *********** Execution::RegisterDefaultHandler_invalid_parameter ****************
 ********************************************************************************
 */
namespace   {
    /*
     *  Because of Microsoft's new secure-runtime-lib  - we must provide a handler to catch errors (shouldn't occur - but in case.
     *  We treat these largely like ASSERTION errors, but then translate them into a THROW of an exception - since that is
     *  probably more often the right thing todo.
     */
    void    invalid_parameter_handler_ (const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved)
    {
        TraceContextBumper  trcCtx (TSTR ("invalid_parameter_handler"));
        DbgTrace  (L"Func='%s', expr='%s', file='%s'.", function, expression, file);
        Assert (false);
        Execution::DoThrow (Execution::Platform::Windows::Exception (ERROR_INVALID_PARAMETER));
    }
}
void    Execution::Platform::Windows::RegisterDefaultHandler_invalid_parameter ()
{
    (void)_set_invalid_parameter_handler (invalid_parameter_handler_);
}
