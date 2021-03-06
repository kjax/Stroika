/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#include    "../StroikaPreComp.h"

#include    "UserCanceledException.h"



using   namespace   Stroika::Foundation;
using   namespace   Stroika::Foundation::Execution;





/*
 ********************************************************************************
 ******************************** Execution::DoThrow ****************************
 ********************************************************************************
 */
template    <>
void    _NoReturn_  Execution::DoThrow (const UserCanceledException& e2Throw)
{
    DbgTrace (TSTR ("Throwing UserCanceledException"));
    throw e2Throw;
}

