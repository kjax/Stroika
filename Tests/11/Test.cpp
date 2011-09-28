/*
 * Copyright(c) Sophist Solutions Inc. 1990-2011.  All rights reserved
 */
#include	"Stroika/Foundation/StroikaPreComp.h"

#include	<iostream>
#include	<sstream>



#include    "Stroika/Foundation/Containers/Tally.h"
#include    "Stroika/Foundation/Containers/Tally_Array.h"
#include    "Stroika/Foundation/Containers/Tally_LinkedList.h"
#include	"Stroika/Foundation/Debug/Assertions.h"
#include	"Stroika/Foundation/Debug/Trace.h"


#include	"../TestHarness/SimpleClass.h"
#include	"../TestHarness/TestHarness.h"



using   namespace   Stroika;
using	namespace	Stroika::Foundation;
using	namespace	Stroika::Foundation::Containers;





namespace	{


template	<typename T>	void	TallyIteratorTests(Tally<T>& s)
{
	const	size_t	kTestSize	=	6;

	VerifyTestResult (s.GetLength () == 0);

	For (it, s ) {
		VerifyTestResult (false);
	}

	/*
	 * Try removes while iterating forward.
	 */
	{
		for (size_t i = 1; i <= kTestSize; i++) {
			s.Add (T (i));
		}


		For (it, s) {
			it.UpdateCount (1);
		}

		VerifyTestResult (s.GetLength () == kTestSize);

		{
			For (it, s) {
				for (size_t i = 1; i <= kTestSize; i++) {
					VerifyTestResult (s.Contains (T(i)));
					VerifyTestResult (s.GetLength () == kTestSize - i + 1);
					s.Remove (T(i));
					VerifyTestResult (not s.Contains (T(i-1)));
				}
			}
			VerifyTestResult (s.IsEmpty ());
			VerifyTestResult (s.GetLength () == 0);
		}

		for (size_t i = 1; i <= kTestSize; i++) {
			s.Add (T(i));
		}
		VerifyTestResult (s.GetLength () == kTestSize);
		{
			For (it, s) {
				it.RemoveCurrent ();
			}
			VerifyTestResult (s.IsEmpty ());
			VerifyTestResult (s.GetLength () == 0);
		}

		for (size_t i = 1; i <= kTestSize; i++) {
			s.Add (T(i));
		}
		VerifyTestResult (s.GetLength () == kTestSize);
		For (it2, s) {
			s.Remove (it2.Current ().fItem);
		}
		VerifyTestResult (s.GetLength () == 0);
	}

	/*
	 * Try removes multiple iterators present.
	 */
	{
		s.RemoveAll ();
		VerifyTestResult (s.GetLength () == 0);
		for (size_t i = 1; i <= kTestSize; i++) {
			s.Add (T(i));
		}
		VerifyTestResult (s.GetLength () == kTestSize);
		size_t i =	1;

		For (it, s) {
			For (it2, s) {
				For (it3, s) {
					if (s.GetLength () != 0) {
						it3.UpdateCount (3);
						it3.RemoveCurrent ();
						s.Add (T(i));
						s.Remove (T(i));
					}
				}
			}
		}
	}
}

template	<typename T>	void	SimpleTallyTests (Tally<T>& s)
{
	T	three (3);

	Tally<T>	s1 (s);

	VerifyTestResult (s1 == s);
	VerifyTestResult (s1 == s);
	Tally<T>	s2 = s1;

	VerifyTestResult (s2 == s);
	VerifyTestResult (s2 == s1);
	s2.Add (three);
	VerifyTestResult (s1 == s);
	VerifyTestResult (s2 != s1);

	const	size_t	K = 200;

	VerifyTestResult (s.IsEmpty ());
	s.Add (three);
	VerifyTestResult (s.GetLength () == 1);
	s += three;
	VerifyTestResult (s.GetLength () == 1);
	VerifyTestResult (s.Contains (three));
	VerifyTestResult (s.TallyOf (three) == 2);
	s.Remove (three);
	VerifyTestResult (s.GetLength () == 1);
	VerifyTestResult (s.Contains (three));
	VerifyTestResult (s.TallyOf (three) == 1);
	s.Remove (three);
	VerifyTestResult (s.IsEmpty ());
	s.RemoveAll ();
	VerifyTestResult (s.IsEmpty ());
	for (size_t i = 1; i <= K; i++) {
		s.Add (T (i));
	}

	for (size_t i = 1; i <= s.GetLength (); i++) {
		VerifyTestResult (s.Contains (T (i)));
		VerifyTestResult (not s.Contains (T (0)));
	}

	for (size_t i = 1; i <= s.GetLength (); i++) {
		For (it, s) {
			if (it.Current ().fItem == i) {
				break;
			}
		}
	}
	For (it, typename Tally<T>::It (s)) {
		For (it1, s) {
			s.RemoveAll ();
		}
	}
	VerifyTestResult (s.IsEmpty ());
	VerifyTestResult (s.GetLength () == 0);

	For (it1, s) {
		For (it2, s) {
			VerifyTestResult (false);
		}
	}
	VerifyTestResult (s.IsEmpty ());


	for (size_t i = 1; i <= K; i++) {
		s.Add (T (i));
		VerifyTestResult (s.Contains (T (i)));
		VerifyTestResult (s.TallyOf (T (i)) == 1);
		VerifyTestResult (s.GetLength () == i);
	}
	for (size_t i = K; i > 0; i--) {
		s.Remove (T (i));
		VerifyTestResult (not s.Contains (T (i)));
		VerifyTestResult (s.GetLength () == (i-1));
	}
	VerifyTestResult (s.IsEmpty ());

	for (size_t i = 1; i <= K/2; i++) {
		s += T (1);
		VerifyTestResult (s.TallyOf (T (1)) == i);
	}
	size_t oldLength = s.GetLength ();
	size_t oldTotal = s.TotalTally ();
	s += s;
	VerifyTestResult (s.GetLength () == oldLength);
	VerifyTestResult (s.TotalTally () == oldTotal*2);

	s.RemoveAll ();
	VerifyTestResult (s.GetLength () == 0);
}


}


namespace	{
	template	<typename ConType, typename T>	void	RunTests ()
	{
		ConType	s;
		SimpleTallyTests<T> (s);
		TallyIteratorTests<T> (s);

	}

	void	DoRegressionTests_ ()
		{
            RunTests<Tally_LinkedList<size_t>, size_t> ();
            RunTests<Tally_LinkedList<SimpleClass>,SimpleClass> ();

            RunTests<Tally_Array<size_t>,size_t> ();
            RunTests<Tally_Array<SimpleClass>,SimpleClass> ();

			// just proof that they can be constructed
			Tally<size_t> t;
			Tally<SimpleClass>	s1;
	}
}


#if qOnlyOneMain
extern  int Test_Tallys ()
#else
int main (int argc, const char* argv[])
#endif
{
	Stroika::TestHarness::Setup ();
	Stroika::TestHarness::PrintPassOrFail (DoRegressionTests_);
	return EXIT_SUCCESS;
}

