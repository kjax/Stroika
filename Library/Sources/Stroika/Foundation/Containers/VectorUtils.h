/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Containers_VectorUtils_h_
#define	_Stroika_Foundation_Containers_VectorUtils_h_	1

#include	"../StroikaPreComp.h"

#include	<vector>

#include	"../Configuration/Common.h"


/// THIS MODULE SB OBSOLETE ONCE WE GET STROIKA CONTAINERS WORKING



namespace	Stroika {	
	namespace	Foundation {
		namespace	Containers {

			template	<typename T>
				vector<typename T>	mkV ();
			template	<typename T>
				vector<typename T>	mkV (const T& t1);
			template	<typename T>
				vector<typename T>	mkV (const T& t1, const T& t2);
			template	<typename T>
				vector<typename T>	mkV (const T& t1, const T& t2, const T& t3);
			template	<typename T>
				vector<typename T>	mkV (const T& t1, const T& t2, const T& t3, const T& t4);
			template	<typename T>
				vector<typename T>	mkV (const T& t1, const T& t2, const T& t3, const T& t4, const T& t5);

			template	<typename T, typename ContainerOfT>
				vector<typename T>	mkVC (const ContainerOfT& ts);


			template	<typename T>
				void	Append2Vector (vector<typename T>* v, const vector<typename T>& v2);
			template	<typename T, typename ContainerOfT>
				void	Append2Vector (vector<typename T>* v, const typename ContainerOfT& v2);


			template	<typename T>
				vector<typename T>	operator+ (const vector<typename T>& l, const vector<typename T>& r);

			template	<typename T>
				vector<typename T>&	operator+= (vector<typename T>& l, const vector<typename T>& r);

			template	<typename T>
				vector<typename T>	Intersection (const vector<T>& s1, const vector<T>& s2);

		}
	}
}
#endif	/*_Stroika_Foundation_Containers_VectorUtils_h_*/



/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include	"VectorUtils.inl"

