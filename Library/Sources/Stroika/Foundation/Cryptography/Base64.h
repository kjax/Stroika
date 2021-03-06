/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#ifndef _Stroika_Foundation_Cryptography_Base64_h_
#define _Stroika_Foundation_Cryptography_Base64_h_  1

#include    "../StroikaPreComp.h"

#include    <string>
#include    <vector>

#include    "../Configuration/Common.h"


namespace   Stroika {
    namespace   Foundation {
        namespace   Cryptography {

            vector<Byte>    DecodeBase64 (const string& s);

            enum    LineBreak { eLF_LB, eCRLF_LB, eAuto_LB = eCRLF_LB };
            string          EncodeBase64 (const Byte* start, const Byte* end, LineBreak lb = eAuto_LB);
            string          EncodeBase64 (const vector<Byte>& b, LineBreak lb = eAuto_LB);

        }
    }
}
#endif  /*_Stroika_Foundation_Cryptography_Base64_h_*/




/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
