/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#ifndef _Stroika_Foundation_Cache_LRUCache_inl_
#define _Stroika_Foundation_Cache_LRUCache_inl_    1

/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include    "../Containers/Common.h"
#include    "../Debug/Assertions.h"


namespace   Stroika {
    namespace   Foundation {
        namespace   Cache {


            //  class   LRUCacheDefaultTraits<ELEMENT,TRAITS>
            template    <typename   ELEMENT, typename KEY>
            inline  typename    LRUCacheDefaultTraits<ELEMENT, KEY>::KeyType LRUCacheDefaultTraits<ELEMENT, KEY>::ExtractKey (const ElementType& e)
            {
                return e;
            }
            template    <typename   ELEMENT, typename KEY>
            inline  size_t  LRUCacheDefaultTraits<ELEMENT, KEY>::Hash (const KeyType& e)
            {
                return 0;
            }
            template    <typename   ELEMENT, typename KEY>
            inline  void    LRUCacheDefaultTraits<ELEMENT, KEY>::Clear (ElementType* element)
            {
                (*element) = ElementType ();
            }
            template    <typename   ELEMENT, typename KEY>
            inline  bool    LRUCacheDefaultTraits<ELEMENT, KEY>::Equal (const KeyType& lhs, const KeyType& rhs)
            {
                return lhs == rhs;
            }




            //  class   LRUCache<ELEMENT,TRAITS>::CacheElement
            template    <typename   ELEMENT, typename TRAITS>
            inline  LRUCache<ELEMENT, TRAITS>::CacheElement::CacheElement ()
                : fNext (nullptr)
                , fPrev (nullptr)
                , fElement ()
            {
            }



            //  class   LRUCache<ELEMENT,TRAITS>::CacheIterator
            template    <typename   ELEMENT, typename TRAITS>
            inline  LRUCache<ELEMENT, TRAITS>::CacheIterator::CacheIterator (CacheElement** start, CacheElement** end)
                : fCurV (start)
                , fEndV (end)
                , fCur (start == end ? nullptr : *fCurV)
            {
            }
            template    <typename   ELEMENT, typename TRAITS>
            inline  typename    LRUCache<ELEMENT, TRAITS>::CacheIterator&    LRUCache<ELEMENT, TRAITS>::CacheIterator::operator++ ()
            {
                RequireNotNull (fCur);
                fCur = fCur->fNext;
                if (fCur == nullptr and fCurV != fEndV) {
                    fCurV++;
                    fCur  = *fCurV;
                }
                return *this;
            }
            template    <typename   ELEMENT, typename TRAITS>
            inline  ELEMENT&    LRUCache<ELEMENT, TRAITS>::CacheIterator::operator* ()
            {
                RequireNotNull (fCur);
                return fCur->fElement;
            }
            template    <typename   ELEMENT, typename TRAITS>
            inline  bool LRUCache<ELEMENT, TRAITS>::CacheIterator::operator== (CacheIterator rhs)
            {
                return fCur == rhs.fCur;
            }
            template    <typename   ELEMENT, typename TRAITS>
            inline  bool LRUCache<ELEMENT, TRAITS>::CacheIterator::operator!= (CacheIterator rhs)
            {
                return fCur != rhs.fCur;
            }


            //  class   LRUCache<ELEMENT,TRAITS>
            template    <typename   ELEMENT, typename TRAITS>
            LRUCache<ELEMENT, TRAITS>::LRUCache (size_t maxCacheSize)
#if 0
                : fCachedElts_BUF_ ()
                , fCachedElts_First_ (nullptr)
                , fCachedElts_fLast_ (nullptr)
#endif
#if     qKeepLRUCacheStats
                : fCachedCollected_Hits (0)
                , fCachedCollected_Misses (0)
#endif
            {
                // TODO: Find more elegant but equally efficent way to initailize and say these are all initialized to zero
                // (INCLUDING fCachedElts_BUF_)
                (void)::memset (&fCachedElts_First_, 0, sizeof (fCachedElts_First_));
                (void)::memset (&fCachedElts_fLast_, 0, sizeof (fCachedElts_First_));

                SetMaxCacheSize (maxCacheSize);
            }
            template    <typename   ELEMENT, typename TRAITS>
            inline  size_t  LRUCache<ELEMENT, TRAITS>::GetMaxCacheSize () const
            {
                return TRAITS::HASH_TABLE_SIZE * fCachedElts_BUF_.size ();
            }
            template    <typename   ELEMENT, typename TRAITS>
            void    LRUCache<ELEMENT, TRAITS>::SetMaxCacheSize (size_t maxCacheSize)
            {
                Require (maxCacheSize >= 1);
                maxCacheSize =  ((maxCacheSize + TRAITS::HASH_TABLE_SIZE - 1) / TRAITS::HASH_TABLE_SIZE);   // divide size over number of hash chains
                maxCacheSize = max (maxCacheSize, static_cast<size_t> (1)); // must be at least one per chain
                for (size_t hi = 0; hi < TRAITS::HASH_TABLE_SIZE; hi++) {
                    if (maxCacheSize != fCachedElts_BUF_[hi].size ()) {
                        fCachedElts_BUF_[hi].resize (maxCacheSize);
                        // Initially link LRU together.
                        fCachedElts_First_[hi] = Containers::Start (fCachedElts_BUF_[hi]);
                        fCachedElts_fLast_[hi] = fCachedElts_First_[hi] + maxCacheSize - 1;
                        fCachedElts_BUF_[hi][0].fPrev = nullptr;
                        for (size_t i = 0; i < maxCacheSize - 1; ++i) {
                            fCachedElts_BUF_[hi][i].fNext = fCachedElts_First_[hi] + (i + 1);
                            fCachedElts_BUF_[hi][i + 1].fPrev = fCachedElts_First_[hi] + (i);
                        }
                        fCachedElts_BUF_[hi][maxCacheSize - 1].fNext = nullptr;
                    }
                }
            }
            template    <typename   ELEMENT, typename TRAITS>
            inline  typename    LRUCache<ELEMENT, TRAITS>::CacheIterator LRUCache<ELEMENT, TRAITS>::begin ()
            {
                return CacheIterator (StartOfArray (fCachedElts_First_), EndOfArray (fCachedElts_First_));
            }
            template    <typename   ELEMENT, typename TRAITS>
            inline  typename    LRUCache<ELEMENT, TRAITS>::CacheIterator LRUCache<ELEMENT, TRAITS>::end ()
            {
                return CacheIterator (nullptr, nullptr);
            }
            template    <typename   ELEMENT, typename TRAITS>
            inline  void    LRUCache<ELEMENT, TRAITS>::ShuffleToHead_ (size_t chainIdx, CacheElement* b)
            {
                Require (chainIdx < TRAITS::HASH_TABLE_SIZE);
                RequireNotNull (b);
                if (b == fCachedElts_First_[chainIdx]) {
                    Assert (b->fPrev == nullptr);
                    return; // already at head
                }
                CacheElement*   prev    =   b->fPrev;
                AssertNotNull (prev);                   // don't call this if already at head
                // patch following and preceeding blocks to point to each other
                prev->fNext = b->fNext;
                if (b->fNext == nullptr) {
                    Assert (b == fCachedElts_fLast_[chainIdx]);
                    fCachedElts_fLast_[chainIdx] = b->fPrev;
                }
                else {
                    b->fNext->fPrev = prev;
                }

                // Now patch us into the head of the list
                CacheElement*   oldFirst    =   fCachedElts_First_[chainIdx];
                AssertNotNull (oldFirst);
                b->fNext = oldFirst;
                oldFirst->fPrev = b;
                b->fPrev = nullptr;
                fCachedElts_First_[chainIdx] = b;

                Ensure (fCachedElts_fLast_[chainIdx] != nullptr and fCachedElts_fLast_[chainIdx]->fNext == nullptr);
                Ensure (fCachedElts_First_[chainIdx] != nullptr and fCachedElts_First_[chainIdx] == b and fCachedElts_First_[chainIdx]->fPrev == nullptr and fCachedElts_First_[chainIdx]->fNext != nullptr);
            }
            template    <typename   ELEMENT, typename TRAITS>
            inline  void    LRUCache<ELEMENT, TRAITS>::ClearCache ()
            {
                for (size_t hi = 0; hi < TRAITS::HASH_TABLE_SIZE; hi++) {
                    for (CacheElement* cur = fCachedElts_First_[hi]; cur != nullptr; cur = cur->fNext) {
                        TRAITS::Clear (&cur->fElement);
                    }
                }
            }
            template    <typename   ELEMENT, typename TRAITS>
            /*
            @METHOD:        LRUCache<ELEMENT>::LookupElement
            @DESCRIPTION:   <p>Check and see if the given element is in the cache. Return that element if its tehre, and nullptr otherwise.
                        Note that this routine re-orders the cache so that the most recently looked up element is first, and because
                        of this re-ordering, its illegal to do a Lookup while a @'LRUCache<ELEMENT>::CacheIterator' exists
                        for this LRUCache.</p>
            */
            inline  ELEMENT*    LRUCache<ELEMENT, TRAITS>::LookupElement (const KeyType& item)
            {
                size_t      chainIdx    =   TRAITS::Hash (item) % TRAITS::HASH_TABLE_SIZE;
                for (CacheElement* cur = fCachedElts_First_[chainIdx]; cur != nullptr; cur = cur->fNext) {
                    if (TRAITS::Equal (TRAITS::ExtractKey (cur->fElement), item)) {
                        ShuffleToHead_ (chainIdx, cur);
#if     qKeepLRUCacheStats
                        fCachedCollected_Hits++;
#endif
                        return &fCachedElts_First_[chainIdx]->fElement;
                    }
                }
#if     qKeepLRUCacheStats
                fCachedCollected_Misses++;
#endif
                return nullptr;
            }
            template    <typename   ELEMENT, typename TRAITS>
            /*
            @METHOD:        LRUCache<ELEMENT>::AddNew
            @DESCRIPTION:   <p>Create a new LRUCache element (potentially bumping some old element out of the cache). This new element
                        will be considered most recently used. Note that this routine re-orders the cache so that the most recently looked
                        up element is first, and because of this re-ordering, its illegal to do a Lookup while
                        a @'LRUCache<ELEMENT>::CacheIterator' exists for this LRUCache.</p>
            */
            inline  ELEMENT*    LRUCache<ELEMENT, TRAITS>::AddNew (const KeyType& item)
            {
                size_t      chainIdx    =   TRAITS::Hash (item) % TRAITS::HASH_TABLE_SIZE;
                ShuffleToHead_ (chainIdx, fCachedElts_fLast_[chainIdx]);
                return &fCachedElts_First_[chainIdx]->fElement;
            }

        }
    }
}
#endif  /*_Stroika_Foundation_Cache_LRUCache_inl_*/
