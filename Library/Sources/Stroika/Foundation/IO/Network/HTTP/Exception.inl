/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_IO_Network_HTTP_Exception_inl_
#define	_Stroika_Foundation_IO_Network_HTTP_Exception_inl_	1


/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
namespace	Stroika {	
	namespace	Foundation {
		namespace	IO {
			namespace	Network {
				namespace	HTTP {


				//	class	HTTPException
					inline	HTTPStatus	HTTPException::GetStatus () const
						{
							return fStatus;
						}
					inline	wstring	HTTPException::GetReason () const
						{
							return fReason;
						}
					inline	wstring	HTTPException::GetBody () const
						{
							return fBody;
						}
					inline	bool	HTTPException::IsHTTPStatusOK (HTTPStatus status)
						{
							return 200 <= status and status <= 299;
						}


				}
			}
		}
	}
}
#endif	/*_Stroika_Foundation_IO_Network_HTTP_Exception_inl_*/