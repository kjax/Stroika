/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#include    "../StroikaPreComp.h"

#include    "../Characters/Format.h"

#include    "RequiredComponentVersionMismatchException.h"



using   namespace   Stroika::Foundation;
using   namespace   Stroika::Foundation::Execution;





/*
 ********************************************************************************
 **************** RequiredComponentVersionMismatchException *********************
 ********************************************************************************
 */
namespace   {
    wstring mkMsg (const wstring& component, const wstring& requiredVersion)
    {
        wstring x   =   Characters::Format (L"A required component - %s - is missing, or is out of date", component.c_str ());
        if (not requiredVersion.empty ()) {
            x += Characters::Format (L"; version '%s' is required", requiredVersion.c_str ());
        }
        return x;
    }
}
RequiredComponentVersionMismatchException::RequiredComponentVersionMismatchException (const wstring& component, const wstring& requiredVersion):
    StringException (mkMsg (component, requiredVersion))
{
}




