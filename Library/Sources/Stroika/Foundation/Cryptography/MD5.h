/*
 * Copyright(c) Records For Living, Inc. 2004-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Cryptography_MD5_h_
#define	_Stroika_Foundation_Cryptography_MD5_h_	1

#include	"../StroikaPreComp.h"

#include	<string>
#include	<vector>

#include	"../Configuration/Basics.h"


namespace	Stroika {	
	namespace	Foundation {
		namespace	Cryptography {

			// Returns 16 character hex string
			string	ComputeMD5Digest (const Byte* s, const Byte* e);
			string	ComputeMD5Digest (const vector<Byte>& b);

			template	<typename T>
				string	ComputeMD5Digest_UsingOStream (const T& elt);

		}
	}
}



/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include	"MD5.inl"

#endif	/*_Stroika_Foundation_Cryptography_MD5_h_*/