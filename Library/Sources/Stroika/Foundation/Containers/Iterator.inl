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
				Subclassed by front-end container writers.
				Most of the work is done in More, which does a lot of work because it is the
				only virtual function called during iteration, and will need to lock its
				container when doing "safe" iteration. More does the following:
					iterate to the next container value if advance is true
					(then) copy the current value into current, if current is not null
					return true if iteration can continue (not done iterating)

					typical uses:
						it++ -> More (null, true)
						*it -> More (&v, false); return v;
						Done -> More (null, false)

						(note that for performance and safety reasons the iterator envelope actually
						passes fCurrent into More when implementing ++it)
			*/
            template	<typename T> class	Iterator<T>::Rep {
                protected:
                    Rep ();

                public:
                    virtual	~Rep ();

                public:
                    virtual	bool	More (T* current, bool advance)   = 0;
                    virtual	Rep*	Clone () const		= 0;
                    nonvirtual bool Done () const;

                    virtual	bool	IsEqual (const Rep* rhs) const;
            };


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
                return (*fCurrent);
            }

            template	<typename T>   inline   void  Iterator<T>::operator++ ()
            {
                RequireNotNull (fIterator);
                NoConstructorWrapper<T> tmp;
                T*	pT = tmp.AsPointer ();
                if (fIterator->More (pT, true)) {
					fCurrent = NoConstructorWrapper<T>(*pT);
                }
#if qDebug
                else {
                	fCurrent.Clear ();
                }
#endif
			}

            template	<typename T>   inline   void  Iterator<T>::operator++ (int)
            {
            	operator++ ();
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

