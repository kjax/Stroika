/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#ifndef _Stroika_Foundation_Containers_Bag_Array_h_
#define _Stroika_Foundation_Containers_Bag_Array_h_

#include    "../StroikaPreComp.h"

#include    "Bag.h"



namespace   Stroika {
    namespace   Foundation {
        namespace   Containers {

            template    <class T>   class   Bag_ArrayRep;
            template    <class T>   class   Bag_Array : public Bag<T> {
            public:
                Bag_Array ();
                Bag_Array (const Bag<T>& bag);
                Bag_Array (const Bag_Array<T>& bag);
                Bag_Array (const T* start, const T* end);

                nonvirtual  Bag_Array<T>& operator= (const Bag_Array<T>& bag);

                nonvirtual  size_t  GetCapacity () const;
                nonvirtual  void    SetCapacity (size_t slotsAlloced);

            private:
                nonvirtual  const Bag_ArrayRep<T>*  GetRep () const;
                nonvirtual  Bag_ArrayRep<T>*        GetRep ();
            };
        }
    }
}





#endif  /*_Stroika_Foundation_Containers_Bag_Array_h_ */


/*
 ********************************************************************************
 ******************************* Implementation Details *************************
 ********************************************************************************
 */

#include "Bag_Array.inl"



