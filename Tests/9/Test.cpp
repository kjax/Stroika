/*
 * Copyright(c) Sophist Solutions Inc. 1990-2011.  All rights reserved
 */
#include	"Stroika/Foundation/StroikaPreComp.h"

#include	<iostream>
#include	<sstream>


#include    "Stroika/Foundation/Containers/Bag.h"
#include	"Stroika/Foundation/Debug/Assertions.h"
#include	"Stroika/Foundation/Debug/Trace.h"

#include	"../TestHarness/SimpleClass.h"
#include	"../TestHarness/TestHarness.h"

#include    "Stroika/Foundation/Containers/Bag_LinkedList.h"

using   namespace   Stroika;
using	namespace	Stroika::Foundation;
using	namespace	Stroika::Foundation::Containers;



namespace	{

#if		qDebug
	const	size_t	K = 200;
#else
	const	size_t	K = 500;
#endif

template	<typename T>	void	AddInOrder(Bag<T>& s, size_t first, size_t last)
{
	Require (first <= last);
	size_t	oldLength = s.GetLength ();
	for (size_t i = first; i <= last; ++i) {
		s.Add (T (i));
	}
	VerifyTestResult ((s.GetLength () - oldLength) == (last - first + 1));
}


template	<typename T>	void	BagIteratorTests(Bag<T>& s)
{
	const	size_t	kTestSize	= 100;

	const	Bag<T>& sCont = s;

	VerifyTestResult(s.GetLength() == 0);
	/*
	 * Try removes while iterating forward.
	 */
	for(size_t i = 1; i <= kTestSize; i++) {
		s.Add(T(i));
	   VerifyTestResult(s.Contains(T(i)));
	}

	VerifyTestResult(s.GetLength() == kTestSize);
	For (it, sCont) {
		size_t	oldLength = s.GetLength ();
		VerifyTestResult(s.Contains(it.Current ()));
		VerifyTestResult(s.Contains(T (s.GetLength ())));
		s.Remove (T (s.GetLength ()));
		VerifyTestResult(s.GetLength () == oldLength-1);
	}

	s.RemoveAll ();
	AddInOrder (s, 1, kTestSize);

	For (it, s) {
		for(size_t i = 1; i <= kTestSize; i++) {
			VerifyTestResult(s.Contains(T (i)));
			VerifyTestResult(s.GetLength() == kTestSize - i + 1);
			s.Remove(T (i));
			VerifyTestResult(not s.Contains(T (i-1)));
			VerifyTestResult(s.GetLength() == kTestSize - i);
		}
	}
	VerifyTestResult(s.IsEmpty());
	VerifyTestResult(s.GetLength() == 0);

	AddInOrder (s, 1, kTestSize);
	VerifyTestResult(s.GetLength() == kTestSize);
	For (it, s) {
		it.RemoveCurrent();
	}

	VerifyTestResult(s.IsEmpty());
	VerifyTestResult(s.GetLength() == 0);

	AddInOrder (s, 1, kTestSize);
	VerifyTestResult(s.GetLength() == kTestSize);

	For(it2, s) {
		s.Remove(it2.Current());
	}
	VerifyTestResult(s.GetLength() == 0);

	/*
	 * Try removes multiple iterators present.
	 */
	s.RemoveAll();
	VerifyTestResult(s.GetLength() == 0);
	AddInOrder (s, 1, kTestSize);
	VerifyTestResult(s.GetLength() == kTestSize);

	size_t i =	1;
	For(it, s) {
		For(it2, s) {
			For(it3, s) {
				it3.UpdateCurrent(T(i));
				it3.RemoveCurrent();
				s.Add(T(i));
				s.Remove(T(i));
				i++;
			}
		}
	}

    s.RemoveAll ();
	VerifyTestResult(s.GetLength() == 0);
}


template	<typename T>	void	BagTimings(Bag<T>& s)
{
	s.RemoveAll ();
	AddInOrder (s, 1, K);

	for(size_t i = 1; i <= s.GetLength(); i++) {
		VerifyTestResult(s.Contains(T(i)));
		VerifyTestResult(not s.Contains(T(0)));
	}

	for(size_t i = 1; i <= s.GetLength(); i++) {
		For(it, s) {
			if(it.Current() == i) {
				break;
			}
		}
	}

	For(it, s) {
		For(it1, s) {
			s.RemoveAll();
		}
	}
	VerifyTestResult(s.IsEmpty());
	VerifyTestResult(s.GetLength() == 0);

	For(it1, s) {
		For(it2, s) {
			VerifyTestResult(false);
		}
	}
	VerifyTestResult(s.GetLength() == 0);
}

template	<typename T>	void	BagTests (Bag<T>& s)
{
	T	three = T (3);

	Bag<T>	s1(s);

	VerifyTestResult(s1 == s);
	VerifyTestResult(s1 == s);

	Bag<T>	s2 = s1;

	VerifyTestResult(s2 == s);
	VerifyTestResult(s2 == s1);

	s2.Add(three);
	VerifyTestResult(s1 == s);
	VerifyTestResult(s2 != s1);

	size_t i;

	VerifyTestResult(s.IsEmpty());
	s.Add(three);
	VerifyTestResult(s.GetLength() == 1);
	s += three;
	VerifyTestResult(s.GetLength() == 2);
	VerifyTestResult(s.Contains(three));
	s.Remove(three);
	VerifyTestResult(s.GetLength() == 1);
	s -= three;
	VerifyTestResult(s.IsEmpty());
	s.RemoveAll();
	VerifyTestResult(s.IsEmpty());


	for(i = 1; i <= K; i++) {
		s.Add (T (i));
		VerifyTestResult(s.Contains(T (i)));
		VerifyTestResult(s.TallyOf(T (i)) == 1);
		VerifyTestResult(s.GetLength() == i);
	}
	for(i = K; i > 0; i--) {
		s -= T (i);
		VerifyTestResult(not s.Contains(T (i)));
		VerifyTestResult(s.GetLength() ==(i-1));
	}
	VerifyTestResult(s.IsEmpty());

	for(i = 1; i <= K/2; i++) {
		s += T(1);
		VerifyTestResult(s.TallyOf(T(1)) == i);
	}
	size_t oldLength = s.GetLength();
	s += s;
	VerifyTestResult(s.GetLength() == oldLength*2);
	s -= s;
	VerifyTestResult(s.GetLength() == 0);
}

template	<typename T>	void	BasicTests (Bag<T>& s)
{
	T	three (3);

	Bag<T>	s1(s);

	VerifyTestResult(s1 == s);
	VerifyTestResult(s1 == s);

	Bag<T>	s2 = s1;

	VerifyTestResult(s2 == s);
	VerifyTestResult(s2 == s1);
	s2.Add(three);
	VerifyTestResult(s1 == s);
	VerifyTestResult(s2 != s1);

	VerifyTestResult(s.IsEmpty());
	s.Add(three);
	VerifyTestResult(s.GetLength() == 1);
	s.Add(three);
	VerifyTestResult(s.GetLength() >= 1);
	VerifyTestResult(s.Contains(three));
	s.Remove(three);
	VerifyTestResult(s.GetLength() <= 1);
	s.RemoveAll();
	VerifyTestResult(s.IsEmpty());
}

}


namespace	{

	template	<typename ConType, typename T>	void	RunTests ()
	{
		ConType	s;
		BasicTests<T> (s);
		BagTests<T> (s);
		BagIteratorTests<T> (s);
		BagTimings<T> (s);
	}

	void	DoRegressionTests_ ()
	{
		RunTests<Bag_LinkedList<size_t>, size_t> ();
		RunTests<Bag_LinkedList<SimpleClass>, SimpleClass> ();

		RunTests<Bag_Array<size_t>, size_t> ();
		RunTests<Bag_Array<SimpleClass>, SimpleClass> ();

		// just proof that they can be constructed
		Bag<size_t>	s;
		BasicTests<size_t> (s);
		Bag<SimpleClass>	s1;
		BasicTests<SimpleClass> (s1);

		VerifyTestResult(SimpleClass::GetTotalLiveCount() == 0);
	}
}


#if qOnlyOneMain
extern  int Test_Bags ()
#else
int main (int argc, const char* argv[])
#endif
{
	Stroika::TestHarness::Setup ();
	Stroika::TestHarness::PrintPassOrFail (DoRegressionTests_);
	return EXIT_SUCCESS;
}

