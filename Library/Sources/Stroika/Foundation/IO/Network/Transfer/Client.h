/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#ifndef _Stroika_Foundation_IO_Network_Transfer_Client_h_
#define _Stroika_Foundation_IO_Network_Transfer_Client_h_   1

#include    "../../../StroikaPreComp.h"

#include    <map>
#include    <string>

#include    "../../../Configuration/Common.h"
#include    "../../../Characters/String.h"
#include    "../../../DataExchangeFormat/InternetMediaType.h"
#include    "../../../Memory/Optional.h"
#include    "../URL.h"
#include    "../HTTP/Status.h"



// *        (o)     Draft API



/*
 * TODO:
 *
 *
 *      (o)     Decide on and DOCUMENT threading policy. For example - do we need locks internally in the connection object or DEFINE that its
 *              the callers resposabiltiy. PROBABLY best to do in the Connection object itself?
 *
 *      (o)     Progress Callbacks?
 *
 *      (o)     ADD CRITICAL SECTIONS!!! - or DOCUMENT CALLERS REPSONABILTY
 *
 *      (o)     Fixup Requqest/Response code to just be driven off fHeaders  list - and have Get/Set ContentType, etc - and other fields -
 *              which map to that list. MAYBE move request/repsonse stuff to HTTP module- so re-usable in framework code which does webserver??
 *
 *      (o)     Redo response / src API using streams?
 *
 *      (o)     Add Client side certs
 *
 *      (o)     Refine server-side-cert checking mechanism (review LIBCURL to see what makes sense)
 *              PROBABLY just a callback mechanism - with a few default callabcks you can plugin (like reject bad certs)
 *              MAYBE just add FLAG saying whether to VALIDATE_HOST? Maybe callback API"?
 *
 *                        //  This COULD be expanded to include either a BOOL to CHECK remote SSL info, or a callback (which can throw)
 *                        //  if invalid SSL INFO from server side. Thats probably best. callback whcih throws, and set it to such a callback by default!
 *                        //      -- LGP 2012-06-26
 *
 */


namespace   Stroika {
    namespace   Foundation {
        namespace   IO {
            namespace   Network {
                namespace   Transfer {

                    using   Characters::String;
                    using   DataExchangeFormat::InternetMediaType;
                    using   Time::DurationSecondsType;


                    struct  Request {
                        Request ();

                        String              fMethod;
                        map<String, String> fOverrideHeaders;
                        vector<Byte>        fData;  // usually empty, but provided for some methods like POST

                        // scans fOverrideHeaders
                        nonvirtual  InternetMediaType   GetContentType () const;
                        // updates fOverrideHeaders
                        nonvirtual  void                SetContentType (const InternetMediaType& ct);

                        struct  Options {
                            Options ();
                            bool    fReturnSSLInfo;
                        };
                        Options             fOptions;
                    };




                    struct  Response {
                        struct  SSLResultInfo;
                        Response ();

                        vector<Byte>                    fData;  // usually empty, but provided for some methods like POST
                        map<String, String>             fHeaders;
                        HTTP::Status                    fStatus;
                        Memory::Optional<SSLResultInfo> fServerEndpointSSLInfo;

                        nonvirtual  bool                GetSucceeded () const;
                        nonvirtual  InternetMediaType   GetContentType () const;    // scans headers
                    };

                    // This info is returned only for secure connections - and is an indicator of whether or
                    // not the SSL connection was valid
                    //
                    // This system allows invalid SSL certs as the target - by default - and returns that info, so its up
                    // to the caller to decide whether or not to accept the data from an invalid SSL cert
                    struct  Response::SSLResultInfo {
                        SSLResultInfo ();

                        wstring fSubjectCommonName;     // hostname declared
                        wstring fSubjectCompanyName;
                        wstring fStyleOfValidation;     // a string saying how the cert was valided - for example 'Domain Controll Validated'
                        wstring fIssuer;
                        enum ValidationStatus {
                            eNoSSL,
                            eSSLOK,
                            eCertNotYetValid,   // start date too soon
                            eCertExpired,
                            eHostnameMismatch,
                            eSSLFailure         // generic - typically bad CERT - or bad trust
                        };
                        ValidationStatus    fValidationStatus;
                    };



                    // TODO:
                    //      Unclear about copyability - maybe if its a smartptr OK to copy - but would be copy-by-reference?
                    //      Could be confusing! CONSIDER
                    class   Connection {
                    protected:
                        class   _IRep {
                        private:
                            NO_COPY_CONSTRUCTOR (_IRep);
                            NO_ASSIGNMENT_OPERATOR (_IRep);

                        public:
                            _IRep ();
                            virtual ~_IRep ();

                        public:
                            virtual URL                 GetURL () const                             =   0;
                            virtual void                SetURL (const URL& url)                     =   0;
                            virtual DurationSecondsType GetTimeout () const                         =   0;
                            virtual void                SetTimeout (DurationSecondsType timeout)    =   0;
                            virtual void                Close ()                                    =   0;
                            virtual Response            SendAndRequest (const Request& r)           =   0;
                        };

                    protected:
                        Connection (const shared_ptr<_IRep>& rep);

                    public:
                        // SendAndRequest should timeout after this amount of time. Note - the initial SendAndRequest may do
                        // much more work (nslookup and tcp connect) than subsequent ones, and this same timeout is used for the combined time.
                        nonvirtual  DurationSecondsType     GetTimeout () const;
                        nonvirtual  void                    SetTimeout (DurationSecondsType timeout);

                    public:
                        nonvirtual  URL     GetURL () const;
                        nonvirtual  void    SetURL (const URL& url);

                    public:
                        // force closed Connection. Can still call Send again, but that autocreates new Connection
                        nonvirtual  void    Close ();

                    public:
                        nonvirtual  Response    SendAndRequest (const Request& r);

                        // Simple wrappers, with hardwired methods
                    public:
                        nonvirtual  Response    Get (const map<String, String>& extraHeaders = map<String, String> ());
                        nonvirtual  Response    Post (const vector<Byte>& data, const InternetMediaType& contentType, const map<String, String>& extraHeaders = map<String, String> ());
                        nonvirtual  Response    Delete (const map<String, String>& extraHeaders = map<String, String> ());
                        nonvirtual  Response    Put (const vector<Byte>& data, const InternetMediaType& contentType, const map<String, String>& extraHeaders = map<String, String> ());
                        nonvirtual  Response    Options (const map<String, String>& extraHeaders = map<String, String> ());

                    private:
                        shared_ptr<_IRep>    fRep_;
                    };



                    /*
                     * Simple connection factory object. If you don't care what backend to use for remote connections, use this API
                     * to construct an unconnected object.
                     */
                    Connection  CreateConnection ();
                }
            }
        }
    }
}
#endif  /*_Stroika_Foundation_IO_Network_Transfer_Client_h_*/





/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
#include    "Client.inl"
