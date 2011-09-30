/*
 * Copyright(c) Sophist Solutions Inc. 1990-2011.  All rights reserved
 */
#include	"Stroika/Foundation/StroikaPreComp.h"

#include	<iostream>
#include	<sstream>


#include    "Stroika/Foundation/Containers/Private/LinkedList.h"

#include	"Stroika/Foundation/Debug/Assertions.h"
#include	"Stroika/Foundation/Debug/Trace.h"

#include	"../TestHarness/SimpleClass.h"
#include	"../TestHarness/TestHarness.h"





using	namespace	Stroika;
using	namespace	Stroika::Foundation;
using	namespace	Stroika::Foundation::Containers;
using	namespace	Stroika::Foundation::Containers::Private;




namespace	{

template	<typename T>	void	Test ()
{
	LinkedList_Patch<T>	someLL;
	const	size_t	kBigSize	=	1000;

	VerifyTestResult (someLL.GetLength() == 0);

	Assert (kBigSize > 10);
	VerifyTestResult (someLL.GetLength() == 0);
	{ for (size_t i = 1; i <= kBigSize; i++) { someLL.Prepend (T (0)); } }
	someLL.RemoveAll();
	{ for (size_t i = 1; i <= kBigSize; i++) { someLL.Prepend (T (0)); } }
	{ for (size_t i = 1; i <= kBigSize-10; i++) { someLL.RemoveFirst (); } }
	someLL.RemoveAll();
	{ for (size_t i = 1; i <= kBigSize; i++) { someLL.Prepend (T (0)); } }


	VerifyTestResult (someLL.GetLength() == kBigSize);

	someLL.SetAt (T(55), 55);
	VerifyTestResult (someLL.GetAt (55) == T (55));
	VerifyTestResult (not(someLL.GetAt(55) == T (56)));
	{
		size_t i = 1;
		for (LinkedListMutator_Patch<T> it (someLL); it.More(nullptr, true); i++) {
			if (i==100) {
				it.AddAfter (T (1));
				break;
			}
		}
	}

	VerifyTestResult (someLL.GetLength() == kBigSize+1);
	VerifyTestResult (someLL.GetAt (100) == T (1));

	someLL.SetAt (T (someLL.GetAt (100) + 5), 101);

	VerifyTestResult (T (someLL.GetAt (101)) == T (6));
	someLL.RemoveFirst ();
	VerifyTestResult (T (someLL.GetAt (100)) == T (6));

	someLL.RemoveAll ();
	VerifyTestResult(someLL.GetLength() == 0);

	for (size_t i = kBigSize; i >= 1; --i) {
		VerifyTestResult (not someLL.Contains(T (i)));
		someLL.Prepend (T(i));
		VerifyTestResult (someLL.GetFirst () == T (i));
		VerifyTestResult (someLL.Contains(T (i)));
	}
	for (size_t i = 1; i <= kBigSize; ++i) {
		VerifyTestResult (someLL.GetFirst () == T (i));
		someLL.RemoveFirst ();
		VerifyTestResult (not someLL.Contains(T (i)));
	}
	VerifyTestResult(someLL.GetLength() == 0);

	for (size_t i = kBigSize; i >= 1; --i) {
		someLL.Prepend (T (i));
	}
	for (size_t i = kBigSize; i >= 1; --i) {
//cerr << "i, getat(i-1) = " << i << ", " << someLL.GetAt (i-1).GetValue () << endl;
		VerifyTestResult(someLL.GetAt(i-1) == T (i));
	}
}

}


namespace	{

	void	DoRegressionTests_ ()
		{
		    Test<size_t> ();
		    Test<SimpleClass> ();
		}
}



#if qOnlyOneMain
extern  int Test_LinkedList ()
#else
int main (int argc, const char* argv[])
#endif
{
	Stroika::TestHarness::Setup ();
	Stroika::TestHarness::PrintPassOrFail (DoRegressionTests_);
	return EXIT_SUCCESS;
}



