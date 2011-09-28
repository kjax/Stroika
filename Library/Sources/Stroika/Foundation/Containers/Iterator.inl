/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef _Stroika_Foundation_Containers_Iterator_inl_
#define _Stroika_Foundation_Containers_Iterator_inl_

#include	"../Debug/Assertions.h"

// we allow fIterator to equal nullptr, as a sentinal value during iteration, signaling that iteration is Done
#define	qIteratorUsingNullRepAsSentinalValue	1


namespace	Stroika {
	namespace	Foundation {
		namespace	Containers {


			/*
				Support for ranged for syntax: for (it : v) { it.Current (); }
				This typedef lets you easily construct iterators other than the basic
				iterator for the container.
				Sample usage:
				typedef	RangedForIterator<Tally<T>, TallyMutator<T> >		Mutator;
			*/
			template	<typename Container, typename IteratorClass>	class	RangedForIterator {
				public:
					RangedForIterator (Container& t) :
						fIt (t)
					{
					}

					RangedForIterator (const Container& t) :
						fIt (t)
					{
					}
					nonvirtual  IteratorClass    begin () const
					{
						return fIt;
					}

					IteratorClass end () const
					{
						return (IteratorClass (nullptr));
					}

				private:
					IteratorClass	fIt;
			};


            // class Rep<T>
            template	<typename T> inline	Iterator<T>::Rep::Rep ()
            {
            }

            template	<typename T> inline	Iterator<T>::Rep::~Rep ()
            {
            }

            template	<typename T> inline	bool	Iterator<T>::Rep::Done () const
            {
                return not const_cast<Iterator<T>::Rep*> (this)->More (nullptr, false);
            }

			template	<typename T>	bool   Iterator<T>::Rep::IsEqual (const Rep* rhs)  const
			{
				return (this == rhs);
			}

            // class Iterator<T>
            template	<typename T> inline	Iterator<T>::Iterator (const Iterator<T>& from) :
                fIterator (from.fIterator, &Clone_),
                fCurrent (from.fCurrent)
            {
            }

            template	<typename T> inline	Iterator<T>::Iterator (Rep* it)   :
                fIterator (it, &Clone_)
            {
            }

            template	<typename T> inline	Iterator<T>::~Iterator ()
            {
            }

            template	<typename T> inline	Iterator<T>&	Iterator<T>::operator= (const Iterator<T>& rhs)
            {
            	fIterator = rhs.fIterator;
            	fCurrent = rhs.fCurrent;
                return (*this);
            }

            template	<typename T> inline	T	Iterator<T>::Current () const
            {
                return (operator* ());
            }

            template	<typename T> inline	bool	Iterator<T>::Done () const
            {
				// nullptr is a sentinal value, used for begin/end style usage as the end value
				return (fIterator.GetPointer () == nullptr) or fIterator->Done ();
            }

            template	<typename T>  inline    T   Iterator<T>::operator* () const
            {
                RequireNotNull (fIterator);
                return (fCurrent);
            }

            template	<typename T>   inline   void  Iterator<T>::operator++ ()
            {
                RequireNotNull (fIterator);
                fIterator->More (&fCurrent, true);
			}

            template	<typename T>   inline   void  Iterator<T>::operator++ (int)
            {
                RequireNotNull (fIterator);
                fIterator->More (&fCurrent, true);
			}

            template	<typename T> inline bool   Iterator<T>::operator!= (Iterator rhs)  const
            {
            	return not operator== (rhs);
            }

            template	<typename T> inline bool   Iterator<T>::operator== (Iterator rhs)  const
            {
				bool	done = Done ();	// potentially expensive so get once
            	if (rhs.Done ()) {
					return done;
            	}
				else if (done) {
					return false;
            	}
               return (fCurrent == rhs.fCurrent and (fIterator->IsEqual (fIterator.GetPointer ())));
			}

			template	<typename T> inline  typename Iterator<T>::Rep*   Iterator<T>::Clone_ (const typename Iterator<T>::Rep& rep)
			{
				return rep.Clone ();
			}

		}
    }
}



#endif /* _Stroika_Foundation_Containers_Iterator_inl_ */

