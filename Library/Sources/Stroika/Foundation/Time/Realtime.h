/*
 * Copyright(c) Records For Living, Inc. 2004-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Time_Realtime_h_
#define	_Stroika_Foundation_Time_Realtime_h_	1

#include	"../StroikaPreComp.h"

namespace	Stroika {	
	namespace	Foundation {
		namespace	Time {


			typedef	double	TickCountType;

			TickCountType	GetTickCount ();

		}
	}
}
#endif	/*_Stroika_Foundation_Time_Realtime_h_*/






/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include	"Realtime.inl"
