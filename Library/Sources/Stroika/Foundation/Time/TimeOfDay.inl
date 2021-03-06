/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#ifndef _Stroika_Foundation_Time_TimeOfDay_inl_
#define _Stroika_Foundation_Time_TimeOfDay_inl_ 1


/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include "../Execution/Exceptions.h"

namespace   Stroika {
    namespace   Foundation {
        namespace   Time {


            //  class TimeOfDay
            inline  bool    TimeOfDay::empty () const
            {
                return fTime_ == static_cast<unsigned int> (-1);
            }
            inline  unsigned int    TimeOfDay::GetAsSecondsCount () const
            {
                if (empty ()) {
                    return 0;
                }
                return fTime_;
            }
            inline  uint8_t TimeOfDay::GetHours () const
            {
                uint32_t    n   =   GetAsSecondsCount () / (60 * 60);
                Ensure (0 <= n and n <= 23);
                return n;
            }
            inline  uint8_t TimeOfDay::GetMinutes () const
            {
                uint32_t    n   =   GetAsSecondsCount ();
                n -= GetHours () * 60 * 60;
                n /= 60;
                Ensure (0 <= n and n <= 59);
                return n;
            }
            inline  uint8_t TimeOfDay::GetSeconds () const
            {
                uint32_t    n   =   GetAsSecondsCount ();
                n -= GetHours () * 60 * 60;
                n -= GetMinutes () * 60;
                Ensure (0 <= n and n <= 59);
                return n;
            }
            inline  int TimeOfDay::Compare (const TimeOfDay& rhs) const
            {
                if (empty ()) {
                    return rhs.empty () ? 0 : -1;
                }
                else {
                    return rhs.empty () ? 1 : (GetAsSecondsCount () - rhs.GetAsSecondsCount ());
                }
            }
            inline  bool operator< (const TimeOfDay& lhs, const TimeOfDay& rhs)
            {
                return lhs.Compare (rhs) < 0;
            }
            inline  bool operator> (const TimeOfDay& lhs, const TimeOfDay& rhs)
            {
                return lhs.Compare (rhs) > 0;
            }
            inline  bool operator== (const TimeOfDay& lhs, const TimeOfDay& rhs)
            {
                return lhs.Compare (rhs) == 0;
            }
            inline  bool operator!= (const TimeOfDay& lhs, const TimeOfDay& rhs)
            {
                return lhs.Compare (rhs) != 0;
            }

        }


        namespace   Execution {
            template    <>
            inline  void    _NoReturn_  DoThrow (const Time::TimeOfDay::FormatException& e2Throw)
            {
                DbgTrace (L"Throwing TimeOfDay::FormatException");
                throw e2Throw;
            }
        }

    }
}
#endif  /*_Stroika_Foundation_Time_TimeOfDay_inl_*/
