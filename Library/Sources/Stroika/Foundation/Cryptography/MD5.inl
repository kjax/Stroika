/*
 * Copyright(c) Records For Living, Inc. 2004-2011.  All rights reserved
 */
/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include	<ostream>
#include	<sstream>

namespace	Stroika {	
	namespace	Foundation {
		namespace	Cryptography {
			template	<typename T>
				string	ComputeMD5Digest_UsingOStream (const T& elt)
					{
						stringstream	out;
						out << elt;
						string	tmp = out.str ();
						return ComputeMD5Digest (vector<Byte> (Containers::Start (tmp), Containers::End (tmp)));
					}
		}
	}
}