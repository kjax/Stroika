/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#include    "../StroikaPreComp.h"

#include    "../Characters/Format.h"

#include    "FeatureNotSupportedInThisVersionException.h"



using   namespace   Stroika::Foundation;
using   namespace   Stroika::Foundation::Execution;






/*
 ********************************************************************************
 **************** FeatureNotSupportedInThisVersionException *********************
 ********************************************************************************
 */
FeatureNotSupportedInThisVersionException::FeatureNotSupportedInThisVersionException (const wstring& feature)
    : StringException (Characters::Format (L"%s is not supported in this version of HealthFrame: see the documentation (F1) on features in this version", feature.c_str ()))
    , fFeature (feature)
{
}

