/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_IO_Network_HTTP_Exception_h_
#define	_Stroika_Foundation_IO_Network_HTTP_Exception_h_	1

#include	"../../../StroikaPreComp.h"

#include	<map>
#include	<string>

#include	"../../../Configuration/Common.h"
#include	"../../../Execution/StringException.h"


#include	"Status.h"

/*
 * TODO:
 *		(o)		
 */


namespace	Stroika {	
	namespace	Foundation {
		namespace	IO {
			namespace	Network {
				namespace	HTTP {

					using	Foundation::Execution::StringException;

					// See http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6.1.1
					class	HTTPException : public StringException {
						public:
							HTTPException (HTTPStatus status, const wstring& reason, const wstring& body);

						public:
							// throw HTTPException () type iff the status indicates a real HTTP exception code (i.e. ignore 2xx values etc)
							static	void	DoThrowIfError (HTTPStatus status, const wstring& reason = wstring (), const wstring& body = wstring ());
							static	void	DoThrowIfError (const wstring& status, const wstring& reason = wstring (), const wstring& body = wstring ());

							static	bool	IsHTTPStatusOK (HTTPStatus status);

						public:
							nonvirtual	HTTPStatus		GetStatus () const;
							nonvirtual	wstring			GetReason () const;
							nonvirtual	wstring			GetBody () const;

						private:
							HTTPStatus	fStatus;
							wstring		fReason;
							wstring		fBody;
					};


				}
			}
		}
	}
}
#endif	/*_Stroika_Foundation_IO_Network_HTTP_Exception_h_*/





/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include	"Exception.inl"