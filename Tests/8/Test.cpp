/*
 * Copyright(c) Sophist Solutions Inc. 1990-2011.  All rights reserved
 */
#include	"Stroika/Foundation/StroikaPreComp.h"

#include	<iostream>
#include	<sstream>


#include    "Stroika/Foundation/Containers/Private/Array.h"
#include	"Stroika/Foundation/Debug/Assertions.h"
#include	"Stroika/Foundation/Debug/Trace.h"

#include	"../TestHarness/SimpleClass.h"
#include	"../TestHarness/TestHarness.h"





using	namespace	Stroika;
using	namespace	Stroika::Foundation;
using	namespace	Stroika::Foundation::Containers;







namespace	{
template	<typename T>	void	Test1 ()
{
	Array<T>	a;
	a.InsertAt(T (100), 0);
	// for (size_t i = 0; i < someArray.GetLength (); ++i) { cerr << "someArray[" << i << "] = " << someArray[i].GetValue () << endl; }
	VerifyTestResult (a.GetLength() == 1);
	VerifyTestResult (a[0] == T (100));
	a.RemoveAt(0);
	a.InsertAt(T (2), 0);
	a.InsertAt(T (1), 0);
	a.InsertAt(T (3), 0);
	a.InsertAt(T (4), a.GetLength ());
	VerifyTestResult (a.GetLength() == 4);
	VerifyTestResult (a[0] == T (3));
	VerifyTestResult (a[1] == T (1));
	VerifyTestResult (a[2] == T (2));
	VerifyTestResult (a[3] == T (4));
	a.RemoveAt(a.GetLength ()-1);
	a.RemoveAt(1);
	VerifyTestResult (a.GetLength() == 2);
}

template	<typename T>	void	Test2 ()
{
    Array<T>	someArray;

	const	size_t	kBigSize	=	1001;

	VerifyTestResult (someArray.GetLength() == 0);

	someArray.SetLength (kBigSize, T (0));
	someArray.RemoveAll();
	someArray.SetLength(kBigSize, T (0));
	someArray.SetLength(10, T (0));
	someArray.SetLength(kBigSize, T (0));

	VerifyTestResult (someArray.GetLength() == kBigSize);
	someArray [55] = T (55);
	VerifyTestResult (someArray [55] == T (55));
	VerifyTestResult (someArray [55] != T (56));

	someArray.InsertAt(T (1), 100);
	VerifyTestResult (someArray.GetLength() == kBigSize+1);
	VerifyTestResult (someArray [100] == T (1));

	someArray [101] = T (someArray [100] + 10);
    VerifyTestResult (someArray[101] == T (11));
	someArray.RemoveAt(0);
	VerifyTestResult (someArray[100] == T (11));
	someArray.RemoveAt(1);

	VerifyTestResult (someArray [99] == T (11));
}


}


namespace	{

	void	DoRegressionTests_ ()
		{
		    Test1<size_t> ();
		    Test2<size_t> ();

		    Test1<SimpleClass> ();
		    Test2<SimpleClass> ();
		}
}



#if qOnlyOneMain
extern  int Test_Arrays ()
#else
int main (int argc, const char* argv[])
#endif
{
	Stroika::TestHarness::Setup ();
	Stroika::TestHarness::PrintPassOrFail (DoRegressionTests_);
	return EXIT_SUCCESS;
}



