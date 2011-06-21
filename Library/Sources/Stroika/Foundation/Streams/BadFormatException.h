/*
 * Copyright(c) Records For Living, Inc. 2004-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Streams_BadFormatException_h_
#define	_Stroika_Foundation_Streams_BadFormatException_h_	1

#include	"../StroikaPreComp.h"

#include	"../Execution/StringException.h"

namespace	Stroika {	
	namespace	Foundation {
		namespace	Streams {

			// Use when reading from a structured stream the data is ill-formed
			class	BadFormatException : public Execution::StringException {
				public:
					BadFormatException ();
					BadFormatException (const wstring& details);

				public:
					nonvirtual	wstring	GetDetails () const;

				private:
					wstring	fDetails_;
			};

		}
	}
}
#endif	/*_Stroika_Foundation_Streams_BadFormatException_h_*/





/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include	"BadFormatException.inl"