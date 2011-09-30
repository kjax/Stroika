/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Containers_Iterator_h_
#define	_Stroika_Foundation_Containers_Iterator_h_	1



/*
	Iterators allow ordered traversal of a container. In general, the only
 	guarantee about iteration order is that it will remain the same as long as
 	the container has not been modified. Difference subclasses of Container
 	more narrowly specify the details of traversal order: for example a Stack
 	will always iterate from the top-most to the bottom-most item.

	Iterators are robust against changes to their container. Adding or
	removing items from a container will not invalidate the iteration.
	Subclasses of Container can make further guarantees about the behavior
	of iterators in the presence of Container modifications.
	For example a SequenceIterator will always traverse any
	items added after the current traversal index, and will never traverse
	items added with an index before the current traversal index.

	Current returns the current iterator value. Current can be called at anytime not Done.
	The value of Current is guaranteed to be valid if it was called when not Done, even if
	that value was removed from the container at some point after Current was called.
	The value of Current is undefined if called when Done.
	operator* is equivalent to Current, you can use it.Current () or *it.

	Iteration is advanced via operator++. If not Done, then the iterator advances to the
	next item in the container, changing the value returned by Current. You can call operator++
	even if Done: this has no effect. ** SSW NOTE: we need to check if this is the case
	when an iterator was Done but then more items are added past it -- we don't want it to continue
	iterating if Done was called. Maybe can enforce in the Iterator object via a sentinal value for current? **

	Once an iteration has advanced it will never return to the previous place.
	Iterations cannot be restarted.

	Iterators can be copied. They always refer to the same place they did before the copy,
	and the old iterator is unaffected by iteration in the new.

	Ideally, iterators are equal if they have the produce the identical answers to
	a sequence of Current, operator++ calls, until each is Done.
	For practical reasons, Iterators may not reach this ideal, but
	they will only produce false negatives (i.e. say thay are not equal when
	if tested out, they would in fact turn out to be equal). Iterators that are copied are equal
	if they have advanced via operator++ an equal number of times since creation of the copy.
	Iterators that are Done are all equal.

	Iterator are used primarily to get auto-destruction
 	at the end of a scope where they are used. They can be used directly,
 	or using ranged for syntax (currently imitated by For macro).

 	Example usage:
		for (Iterator<size_t> it (myContainer); ! it.Done (); ++it)
		for (Iterator<size_t> it = myContainer.begin (); it != myContainer.end (); ++it)
		For (it, myContainer)
 */



#include	"../StroikaPreComp.h"

#include	"../Configuration/Common.h"

#include	"../Memory/SharedByValue.h"


namespace	Stroika {
	namespace	Foundation {
		namespace	Containers {

		// start of NoConstructorWrapper, cut here
		template	<typename T> class	NoConstructorWrapper {
			public:
				NoConstructorWrapper () :
					fConstructed (false)
				{
				}

				NoConstructorWrapper (const NoConstructorWrapper& rhs) :
					fConstructed (false)
				{
					if (rhs.fConstructed) {
						new (fItem) T (reinterpret_cast<const T&> (rhs.fItem));
						fConstructed = true;
					}
				}

				explicit	NoConstructorWrapper (T item) :
					fConstructed (false)
				{
					new (fItem) T (reinterpret_cast<const T&> ( item));
					fConstructed = true;
				}

				~NoConstructorWrapper ()
				{
					DestroyCurrent ();
				}

				void	operator= (const NoConstructorWrapper& rhs)
				{
					DestroyCurrent ();
					if (rhs.fConstructed) {
						new (fItem) T (reinterpret_cast<const T&> (rhs.fItem));
						fConstructed = true;
					}
				}

				T   operator* () const
				{
					Require (fConstructed);
					return (*reinterpret_cast<const T*> (&fItem));
				}

				bool	operator== (const NoConstructorWrapper<T>& rhs) const
				{
					return (fConstructed and (memcmp (fItem, rhs.fItem, sizeof (T)) == 0));
				}

				bool	operator!= (const NoConstructorWrapper<T>& rhs) const
				{
					return (not operator== (rhs));
				}

				T*	AsPointer ()
				{
					/*
						This is a potential problem. I think we want to insist that
						it be treated as raw memory, so we make require that we
						aren't constructed at this point (can use Clear to do that,
						but probably (nearly) always better to construct a temporary
						variable and call AsPointer from it, then copy back to what you want.
					*/
					Require (not fConstructed);
					T*	item = reinterpret_cast<T*> (&fItem);
					return item;
				}

				// currently only called in debug mode, perhaps should jettison entirely
				nonvirtual	void	Clear ()
				{
					DestroyCurrent ();
				}

			private:
				nonvirtual	void	DestroyCurrent ()
				{
					if (fConstructed) {
						reinterpret_cast<T*> (&fItem)->~T ();
						fConstructed = false;
					}
				}

			private:
				Byte	fItem[sizeof (T)];
				bool	fConstructed;
		};

		template	<typename T>	bool	operator== (const NoConstructorWrapper<T>& lhs, const NoConstructorWrapper<T>& rhs)
		{
			return (rhs.fConstructed and lhs.operator== (rhs.fItem));
		}

		template	<typename T>	bool	operator!= (const NoConstructorWrapper<T>& lhs, const NoConstructorWrapper<T>& rhs)
		{
			return not operator== (lhs,rhs);
		}
		// end cutting for NoConstructorWrapper


            template	<typename T> class	Iterator {
             	public:
					class	Rep;
				public:
					explicit	Iterator (Rep* it);
                    Iterator (const Iterator<T>& from);
                    ~Iterator ();

                    nonvirtual	Iterator<T>&	operator= (const Iterator<T>& rhs);

                private:
                    Iterator ();	// Never implemented - illegal

                public:
                    nonvirtual  T       operator* () const;
                    nonvirtual  void    operator++ ();
                    nonvirtual  void    operator++ (int);
                    nonvirtual  bool    operator!= (Iterator rhs) const;

                    nonvirtual  bool    operator== (Iterator rhs) const;

                public:
                    nonvirtual	T		Current () const;
                    nonvirtual	bool	Done () const;

                protected:
                    Memory::SharedByValue<Rep>	fIterator;

				private:
					NoConstructorWrapper<T>	fCurrent;
                   // T       fCurrent;   // SSW 9/19/2011: naive impementation that requires a no-arg constructor for T and has to build a T before being asked for current

					static	Rep*	Clone_ (const Rep& rep);
			};



            /*
             For macro:
             This will be removed when compilers support ranged for. Just replace For with for, and the comma with a colon
             For (it, myBag) with for (it : myBag)
             */
			#define	For(_it,_Container)			for (auto _it = _Container.begin (); _it != _Container.end (); ++_it)

		}
    }
}

#endif	/*_Stroika_Foundation_Containers_Iterator_h_ */



/*
 ********************************************************************************
 ******************************* Implementation Details *************************
 ********************************************************************************
 */

#include "Iterator.inl"
