
/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#ifndef	_Stroika_Foundation_Containers_SetUtils_inl_
#define	_Stroika_Foundation_Containers_SetUtils_inl_	1

namespace	Stroika {	
	namespace	Foundation {
		namespace	Containers {

			template	<class T>
				set<typename T>	operator- (const set<typename T>& lhs, const set<typename T>& rhs)
					{
						set<typename T>	result;
						for (set<typename T>::const_iterator i = lhs.begin (); i != lhs.end (); ++i) {
							if (rhs.find (*i) == rhs.end ()) {
								result.insert (*i);
							}
						}
						return result;
					}
			template	<class T>
				set<typename T>	operator+ (const set<typename T>& lhs, const set<typename T>& rhs)
					{
						set<typename T>	result	=	lhs;
						for (set<typename T>::const_iterator i = rhs.begin (); i != rhs.end (); ++i) {
							if (result.find (*i) == result.end ()) {
								result.insert (*i);
							}
						}
						return result;
					}
			template	<typename T, typename FROMCONTAINER>
				set<typename T>&	operator+= (set<typename T>& lhs, const FROMCONTAINER& rhs)
					{
						for (FROMCONTAINER::const_iterator i = rhs.begin (); i != rhs.end (); ++i) {
							if (lhs.find (*i) == lhs.end ()) {
								lhs.insert (*i);
							}
						}
						return lhs;
					}
			template	<typename T, typename FROMCONTAINER>
				set<typename T>&	operator-= (set<typename T>& lhs, const FROMCONTAINER& rhs)
					{
						for (FROMCONTAINER::const_iterator i = rhs.begin (); i != rhs.end (); ++i) {
							set<typename T>::iterator fi	=	lhs.find (*i);
							if (fi != lhs.end ()) {
								lhs.erase (fi);
							}
						}
						return lhs;
					}


			template	<typename T>
				inline	set<typename T>	mkS ()
					{
						set<T>	v;
						return v;
					}
			template	<typename T>
				inline	set<typename T>	mkS (const T& t1)
					{
						set<T>	s;
						s.insert (t1);
						return s;
					}
			template	<typename T>
				inline	set<typename T>	mkS (const T& t1, const T& t2)
					{
						set<T>	s;
						s.insert (t1);
						s.insert (t2);
						return s;
					}
			template	<typename T>
				inline	set<typename T>	mkS (const T& t1, const T& t2, const T& t3)
					{
						set<T>	s;
						s.insert (t1);
						s.insert (t2);
						s.insert (t3);
						return s;
					}
			template	<typename T>
				inline	set<typename T>	mkS (const T& t1, const T& t2, const T& t3, const T& t4)
					{
						set<T>	s;
						s.insert (t1);
						s.insert (t2);
						s.insert (t3);
						s.insert (t4);
						return s;
					}
			template	<typename T, typename FROMCONTAINER>
				inline	set<typename T>	mkSfromC (const FROMCONTAINER& rhs)
					{
						return set<typename T> (rhs.begin (), rhs.end ());
					}



			template	<typename T>
				void	Intersect (set<T>* s1, const set<T>& s2)
					{
						RequireNotNil (s1);
						// Sloppy - but hopefully adequate implementation
						if (not s1->empty () and not s2.empty ()) {
							*s1 = Intersection (*s1, s2);
						}
					}



			template	<typename T>
				set<typename T>	Intersection (const set<T>& s1, const set<T>& s2)
					{
						set<T>	result;
						for (set<T>::const_iterator i = s1.begin (); i != s1.end (); ++i) {
							if (s2.find (*i) != s2.end ()) {
								result.insert (*i);
							}
						}
						return result;
					}
			template	<typename T>
				bool	Intersect (const set<T>& s1, const set<T>& s2)
					{
						for (set<T>::const_iterator i = s1.begin (); i != s1.end (); ++i) {
							if (s2.find (*i) != s2.end ()) {
								return true;
							}
						}
						return false;
					}

			template	<typename T>
				void	Union (set<T>* s1, const set<T>& s2)
					{
						for (set<T>::const_iterator i = s2.begin (); i != s2.end (); ++i) {
							if (s1->find (*i) == s1->end ()) {
								s1->insert (*i);
							}
						}
					}

			template	<typename T>
				set<typename T>	Union (const set<T>& s1, const set<T>& s2)
					{
						set<T>	result	=	s1;
						Union (&result, s2);
						return result;
					}

		}
	}
}
#endif	/*_Stroika_Foundation_Containers_SetUtils_inl_*/
