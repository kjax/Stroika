/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#ifndef _Stroika_Foundation_Memory_WeakSharedPtr_inl_
#define _Stroika_Foundation_Memory_WeakSharedPtr_inl_   1


/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */

namespace   Stroika {
    namespace   Foundation {
        namespace   Memory {


            // class    Private::WeakSharedPtrCapableSharedPtrEnvelope_<T,BASE_SharedPtr_TRAITS>
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            Private::WeakSharedPtrCapableSharedPtrEnvelope_<T, BASE_SharedPtr_TRAITS>::WeakSharedPtrCapableSharedPtrEnvelope_ (T* ptr)
                : BASE_SharedPtr_TRAITS::Envelope (ptr)
                , fWeakSharedPtrs (nullptr)
            {
                /*
                 * This CTOR is only called when handled an original bare pointer, so its OK to allocate the memory for hte sharedptrs list without
                 * leaking.
                 *
                 * This may not be totally right (when used with SharedFromThis()) pointers - so may need to rethnk that!
                 */
                Require (this->CurrentRefCount () == 0);  // because otherwise we cannot use this API, as it allocates an extra fWeakSharedPtrs list
                if (this->GetPtr () != nullptr) {
                    // only store linkedlist for non-null pointers
                    fWeakSharedPtrs = DEBUG_NEW list<WeakSharedPtrRep_<T, BASE_SharedPtr_TRAITS>*> ();
                }
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            template <typename T2, typename T2_BASE_SharedPtr_TRAITS>
            inline  Private::WeakSharedPtrCapableSharedPtrEnvelope_<T, BASE_SharedPtr_TRAITS>::WeakSharedPtrCapableSharedPtrEnvelope_ (const WeakSharedPtrCapableSharedPtrEnvelope_<T2, T2_BASE_SharedPtr_TRAITS>& from)
                : BASE_SharedPtr_TRAITS::Envelope (from)
                , fWeakSharedPtrs (from.fWeakSharedPtrs)
            {
                Assert (fWeakSharedPtrs != nullptr or this->GetPtr () == nullptr);
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline  bool    Private::WeakSharedPtrCapableSharedPtrEnvelope_<T, BASE_SharedPtr_TRAITS>::Decrement ()
            {
                Execution::AutoCriticalSection critSec (sCriticalSection);  // critical section is IN CASE we need to delete counter
                // - must make sure to grab crit section before so WeakPtr class not in middle of a
                // lock()
                return BASE_SharedPtr_TRAITS::Envelope::Decrement ();
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            void    Private::WeakSharedPtrCapableSharedPtrEnvelope_<T, BASE_SharedPtr_TRAITS>::DoDeleteCounter ()
            {
                RequireNotNull (fWeakSharedPtrs);
                /*
                 * NOTE - this function is ALWAYS and ONLY called from inside Decrement() and so therefore
                 * always already within the critical section.
                 */
                // Assert (sCriticalSection.IsLocked ());       -- would do if there was such an API - maybe it should be added?
                if (fWeakSharedPtrs != nullptr) {
                    for (typename list<WeakSharedPtrRep_<T, BASE_SharedPtr_TRAITS>*>::iterator i = fWeakSharedPtrs->begin (); i != fWeakSharedPtrs->end (); ++i) {
                        (*i)->ClearBecauseUnderlyingPointerGone ();
                    }
                    delete fWeakSharedPtrs;
                    fWeakSharedPtrs = nullptr;
                }
                BASE_SharedPtr_TRAITS::Envelope::DoDeleteCounter ();
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            void    Private::WeakSharedPtrCapableSharedPtrEnvelope_<T, BASE_SharedPtr_TRAITS>::ClearBecauseUnderlyingPointerGone ()
            {
                // get base pointer so we can object slice (dont know what fields to clear otherwise). Then clear our subtype fields.
                typename BASE_SharedPtr_TRAITS::Envelope*    thisAsBaseType    =   this;
                *thisAsBaseType = typename BASE_SharedPtr_TRAITS::Envelope (nullptr);
                fWeakSharedPtrs = nullptr;
            }
            template <typename T, typename BASE_SharedPtr_TRAITS>
            Execution::CriticalSection Private::WeakSharedPtrCapableSharedPtrEnvelope_<T, BASE_SharedPtr_TRAITS>::sCriticalSection;






            // class    Private::WeakSharedPtrRep_<T,BASE_SharedPtr_TRAITS>
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            Private::WeakSharedPtrRep_<T, BASE_SharedPtr_TRAITS>::WeakSharedPtrRep_ (const SharedPtrType& shared)
                : fSharedPtrEnvelope (shared.PeekAtEnvelope ())
            {
                Execution::AutoCriticalSection critSec (WeakSharedPtrCapableSharedPtrEnvelope_<T, BASE_SharedPtr_TRAITS>::sCriticalSection);
                if (fSharedPtrEnvelope.GetPtr () != nullptr) {
                    RequireNotNull (fSharedPtrEnvelope.fWeakSharedPtrs);
                    fSharedPtrEnvelope.fWeakSharedPtrs->push_back (this);
                }
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            Private::WeakSharedPtrRep_<T, BASE_SharedPtr_TRAITS>::~WeakSharedPtrRep_ ()
            {
                Execution::AutoCriticalSection critSec (WeakSharedPtrCapableSharedPtrEnvelope_<T, BASE_SharedPtr_TRAITS>::sCriticalSection);
                if (fSharedPtrEnvelope.GetPtr () != nullptr) {
                    RequireNotNull (fSharedPtrEnvelope.fWeakSharedPtrs);
                    typename list<WeakSharedPtrRep_<T, BASE_SharedPtr_TRAITS>*>::iterator i = std::find (fSharedPtrEnvelope.fWeakSharedPtrs->begin (), fSharedPtrEnvelope.fWeakSharedPtrs->end (), this);
                    Assert (i != fSharedPtrEnvelope.fWeakSharedPtrs->end ());       // cannot be missing without bug or memory corruption
                    fSharedPtrEnvelope.fWeakSharedPtrs->erase (i);
                }
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            SharedPtr<T, WeakSharedPtrCapableSharedPtrTraits<T, BASE_SharedPtr_TRAITS>>   Private::WeakSharedPtrRep_<T, BASE_SharedPtr_TRAITS>::Lock ()
            {
                Execution::AutoCriticalSection critSec (WeakSharedPtrCapableSharedPtrEnvelope_<T, BASE_SharedPtr_TRAITS>::sCriticalSection);
                if (fSharedPtrEnvelope.GetPtr () == nullptr) {
                    return SharedPtr<T, WeakSharedPtrCapableSharedPtrTraits<T, BASE_SharedPtr_TRAITS>> ();
                }
                else {
                    return SharedPtr<T, WeakSharedPtrCapableSharedPtrTraits<T, BASE_SharedPtr_TRAITS>> (fSharedPtrEnvelope);
                }
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline  void   Private::WeakSharedPtrRep_<T, BASE_SharedPtr_TRAITS>::ClearBecauseUnderlyingPointerGone ()
            {
                fSharedPtrEnvelope.ClearBecauseUnderlyingPointerGone ();
            }





            // class    WeakSharedPtr<T,BASE_SharedPtr_TRAITS>
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline  WeakSharedPtr<T, BASE_SharedPtr_TRAITS>::WeakSharedPtr ()
                : fRep_ ()
            {
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline  WeakSharedPtr<T, BASE_SharedPtr_TRAITS>::WeakSharedPtr (const WeakSharedPtr<T, BASE_SharedPtr_TRAITS>& from)
                : fRep_ (from)
            {
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline WeakSharedPtr<T, BASE_SharedPtr_TRAITS>::WeakSharedPtr (const SharedPtrType& from)
                : fRep_ (DEBUG_NEW Private::WeakSharedPtrRep_<T, BASE_SharedPtr_TRAITS> (from))
            {
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline const WeakSharedPtr<T, BASE_SharedPtr_TRAITS>& WeakSharedPtr<T, BASE_SharedPtr_TRAITS>::operator= (const WeakSharedPtr<T, BASE_SharedPtr_TRAITS>& rhs)
            {
                fRep_ = rhs.fRep_;
                return *this;
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline  typename WeakSharedPtr<T, BASE_SharedPtr_TRAITS>::SharedPtrType  WeakSharedPtr<T, BASE_SharedPtr_TRAITS>::Lock () const
            {
                if (fRep_.IsNull ()) {
                    return WeakSharedPtr<T, BASE_SharedPtr_TRAITS>::SharedPtrType ();
                }
                return fRep_->Lock ();
            }







            // class    WeakCapableSharedPtr<T,BASE_SharedPtr_TRAITS>
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline  WeakCapableSharedPtr<T, BASE_SharedPtr_TRAITS>::WeakCapableSharedPtr ()
                : inherited ()
            {
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline  WeakCapableSharedPtr<T, BASE_SharedPtr_TRAITS>::WeakCapableSharedPtr (T* from)
                : inherited (from)
            {
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            inline  WeakCapableSharedPtr<T, BASE_SharedPtr_TRAITS>::WeakCapableSharedPtr (const WeakCapableSharedPtr<T, BASE_SharedPtr_TRAITS>& from)
                : inherited (from)
            {
            }
            template    <typename T, typename BASE_SharedPtr_TRAITS>
            template <typename T2, typename T2_TRAITS>
            inline  WeakCapableSharedPtr<T, BASE_SharedPtr_TRAITS>::WeakCapableSharedPtr (const WeakCapableSharedPtr<T2, T2_TRAITS>& from)
                : inherited (from)
            {
            }



        }
    }
}
#endif  /*_Stroika_Foundation_Memory_WeakSharedPtr_inl_*/
