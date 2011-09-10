/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Containers_Bag_h_
#define	_Stroika_Foundation_Containers_Iterator_h_	1

/*
 *
 *		Iterators allow ordered traversal of a Collection. In general, the only
 *	guarantee about iteration order is that it will remain the same as long as
 *	the Collection has not been modified. Difference subclasses of Collection
 *	more narrowly specify the details of traversal order: for example a Stack
 *	will always iterate from the top-most to the bottom-most item.
 *
 *		Iterators are robust against changes to their collection. Adding or
 *	removing items from a collection will not invalidate the iteration. The
 *	only exception is that removing the item currently being iterated over
 *	will invalidate the results of the Current () method (until the next call
 *	to the Next () method). Subclasses of Collection can make further
 *	guarantees about the behavior of iterators in the presence of Collection
 *	modifications. For example a SequenceIterator will always traverse any
 *	items added after the current traversal index, and will never traverse
 *	items added with an index before the current traversal index.
 *
 *		IteratorRep is an envelope class over IteratorRep. Iterators can be
 *	directly used as follows:
 *		for (IteratorRep<int> it (fIntList); not it.Done (); it.Next ()) {
 *			int	current = it.Current ();	// etc
 *		}
 *
 *		Two macros, ForEach and ForEachT (stands for ForEachTyped) can be used
 *	to minimize typing. The ForEach macro takes three arguments: the type (T)
 *	of the collection, the name of the iterator, and the collection to be
 *	iterated over. Thus the equivalent to the above is:
 *		ForEach (int, it, fList) {
 *			int current = it.Current ();	// etc
 *		}
 *
 *		The ForEachT macro takes one additional argument as the first
 *	parameter: the type of iterator. This is used in places where a subclass
 *	of the class IteratorRep is desired. Common subclasses include
 *	SequenceIterator, which provides a CurrentIndex method and can be
 *	traversed in either direction, and Mutators, which are subclasses of
 *	Iterators that allow Collection modifications through methods such as
 *	RemoveCurrent () or UpdateCurrent (). Sample Usage:
 *		ForEachT (SequenceIterator, int, it, fList) {
 *			int current = it.Current ();
 *			size_t	index = it.CurrentIndex ();		// etc
 *		}
 *
 *		Note that we utilize a for-loop based approach to iteration. A
 *	somewhat popular alternative is modeled on Lisp usage: iterating over a
 *	passed in function. This style is sometimes refereed to as "passive"
 *	iteration. However, given C++ lack of support for lambda expressions
 *	(anonymous code blocks) the for-loop based approach seems superior,
 *	since it is always at least as convenient and at least as efficient,
 *	and often is slightly more convenient and slightly more convenient.
 *	DieHards can write ForEach style macros to support the passive style.
 *	For example:
 *		#define	Apply(T,Init,F)\
 			{for (IteratorRep<T> it (Init); not it.Done (); it.Next ())	{ (*F) (it.Current ()); }}
 *	allows usages like Apply(int, fList, PrintInt);
 *	Note: the extra braces above avoid variable name collisions, a minor
 *	benefit of the passive approach.
 *
 *
 * MORE RULES ABOUT ITERATORS (TO BE INTEGRATED INTO DOCS BETTER)
 *
 *		(1)		What is Done() cannot be UnDone()
 *				That is, once an iterator is done, it cannot be restarted.
 *		(2)		Iterators can be copied. They always refer to the same
 *				place they did before the copy, and the old iterator is unaffected
 *				by iteration in the new (though it can be affected indirectly
 *				thru mutations).
 *		(3)		Whether or not you encounter items added after an addition is
 *				undefined.
 *		(4)		A consequence of the above, is that items added after you are done,
 *				are not encountered. I'm not sure if this is currently true. We
 *				may want to rethink definitions, or somehow make this true.
 *
 *
 *	TODO:
 *		->	Should we keep the done variable in Iterator, and assert not
 *			done in Current(), and maybe allow Done () be called on iterators.
 *			Now sure will ever be used given usual More() interface, but it
 *			could be useful. Trouble is that it would cost some to store this
 *			variable, and it might never be used??? Might be OK if its use
 *			could be optimized away???
 *
 *		->	Merge Current virtual call into More() call? Trouble is constructing
 *			T. We could make fields char fCurrent[sizeof(T)] but that poses problems
 *			for copying iterators. On balance, probably best to bag it!!!
 *
 *			Another possability here is having IteratorRep have a ptr to T (ie T*),
 *			that can be peeked at by Current(). This would still involve extra indirection,
 *			but at least no virtual function overhead on the call. That is probably
 *			the best compromise!!!
 *
 */


#include	"../StroikaPreComp.h"

#include	"../Configuration/Common.h"

#include	"../Memory/CopyOnWrite.h"

/*
 *		Iterator are used primarily to get auto-destruction
 *	at the end of a scope where they are used. They are not intended to be used
 *	directly, but just from the ForEach macros.
 *
 *		Logically, it should be a subclass of IteratorRep<T>, but we
 *	avoid this as a performance hack since they we'd generate vtables, etc...
 *	(at least on all known - imho lousy - compilers).
 *
 */


namespace	Stroika {
	namespace	Foundation {
		namespace	Containers {

            template	<typename T> class	IteratorRep;
            template	<typename T> class	Iterator {
                public:
                    Iterator (IteratorRep<T>* it);
                    Iterator (const Iterator<T>& from);
                    ~Iterator ();

                    nonvirtual	Iterator<T>&	operator= (const Iterator<T>& rhs);

                private:
                    Iterator ();	// Never implemented - illegal

                public:
                    nonvirtual	bool	Done () const;
                    nonvirtual	bool	More ();
                    nonvirtual	T		Current () const;

                protected:
                    IteratorRep<T>*	fIterator;
            };



            /*
             *		An iterator is a helper class, that allows ordered access to the
             *	elements of an ADT. Iterators cannot alter the contents of their ADT,
             *	but are robust over changes to their ADT.
             */
            template	<typename T> class	IteratorRep {
                protected:
                    IteratorRep ();

                public:
                    virtual	~IteratorRep ();

            // COMMENTS OUT OF DATE!!!
                /*
                 *		Done () can be called anytime, and it tells if the iteration
                 *	is finished.
                 *
                 *		Next () can only be called if Not Done, OR if you were not done
                 *	before, and you deleted the current item. The reason for this
                 *	apparantly bizarre exception is because a natural use of these
                 *	methods is in a C for loop, with the Next () being called in the
                 *	re-initalization portion, and Done () in the test portion of the for
                 *	loop. If you get into the body of the for loop, and delete the current
                 *	item - causing your iteration to be completed, then you WILL call Next
                 *	before Done in this case. This is the only case we allow however. It
                 *	is not - in general - allowed to call Next when we are already done.
                 *
                 *		Current () can be called anytime Done () is false. It returns the
                 *	current item in the iteration process. It is undefined what Current ()
                 *	will return if it is deleted while current - however - if anything
                 *	else is deleted, Current () is guaranteed to be valid, and remain
                 *	the same.
                 *
                 */
                public:
                    virtual	bool			Done () const		=	0;
                    virtual	bool			More ()				=	0;
                    virtual	T				Current () const	=	0;
                    virtual	IteratorRep<T>*	Clone () const		=	0;
            };

            /*
             * ForEach Macros.
             */

            #if		qGCC_ConversionOperatorInitializerConfusionBug || qMPW_CFront_ConversionOperatorInitializerCodeGenBug
                #define	ForEach(T,It,Init)			for (Iterator<T > It = (Iterator<T >)(Init); It.More ();)
                #define	ForEachT(ItT,T,It,Init)		for (ItT<T > It = (ItT<T >)(Init); It.More ();)
            #else
                #define	ForEach(T,It,Init)			for (Iterator<T > It (Init); It.More ();)
                #define	ForEachT(ItT,T,It,Init)		for (ItT<T > It (Init); It.More ();)
            #endif



		}
    }
}


namespace	Stroika {
	namespace	Foundation {
		namespace	Containers {

            /*
             ********************************************************************************
             ******************************* Implementation Details *************************
             ********************************************************************************
             */


            // class IteratorRep<T>
            template	<typename T> inline	IteratorRep<T>::IteratorRep ()
            {
            }
            template	<typename T> inline	IteratorRep<T>::~IteratorRep ()
            {
            }

            // class Iterator<T>
            template	<typename T> inline	Iterator<T>::Iterator (const Iterator<T>& from) :
                fIterator (0)
            {
                RequireNotNil (from.fIterator);
                fIterator = from.fIterator->Clone ();
                EnsureNotNil (fIterator);
            }
            template	<typename T> inline	Iterator<T>::Iterator (IteratorRep<T>* it) :
                fIterator (it)
            {
                RequireNotNil (it);
                EnsureNotNil (fIterator);
            }
            template	<typename T> inline	Iterator<T>::~Iterator ()
            {
                delete fIterator;
            }
            template	<typename T> inline	Iterator<T>&	Iterator<T>::operator= (const Iterator<T>& rhs)
            {
                RequireNotNil (fIterator);
                RequireNotNil (rhs.fIterator);
                if (fIterator != rhs.fIterator) {
                    delete fIterator;
                    fIterator = rhs.fIterator->Clone ();
                    EnsureNotNil (fIterator);
                }
                return (*this);
            }
            template	<typename T> inline	bool	Iterator<T>::Done () const
            {
                RequireNotNil (fIterator);
                return (fIterator->Done ());
            }
            template	<typename T> inline	bool	Iterator<T>::More ()
            {
                RequireNotNil (fIterator);
                return (fIterator->More ());
            }
            template	<typename T> inline	T	Iterator<T>::Current () const
            {
                Require (not Done ());
                RequireNotNil (fIterator);
                return (fIterator->Current ());
            }


		}
    }
}

#endif	/*_Stroika_Foundation_Containers_Iterator_h_ */