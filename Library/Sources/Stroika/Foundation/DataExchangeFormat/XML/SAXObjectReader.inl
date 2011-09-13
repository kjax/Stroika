/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_DataExchangeFormat_XML_SAXObjectReader_inl_
#define	_Stroika_Foundation_DataExchangeFormat_XML_SAXObjectReader_inl_	1


/*
 ********************************************************************************
 ***************************** Implementation Details ***************************
 ********************************************************************************
 */
namespace	Stroika {	
	namespace	Foundation {
		namespace	DataExchangeFormat {
			namespace	XML {

				//	class	SAXObjectReader
				inline	void	SAXObjectReader::Push (const Memory::SharedPtr<ObjectBase>& elt)
					{
						Containers::ReserveSpeedTweekAdd1 (fStack);
						fStack.push_back (elt);
					}
				inline	void	SAXObjectReader::Pop ()
					{
						fStack.pop_back ();
					}


				template	<>
					class	BuiltinReader<String> : public SAXObjectReader::ObjectBase {
						public:
							BuiltinReader (String* intoVal);
						private:
							String* value_;
						public:
							virtual	void	HandleChildStart (SAXObjectReader &r, const String& uri, const String& localname, const String& qname, const map<String,Memory::VariantValue>& attrs) override;
							virtual	void	HandleTextInside (SAXObjectReader &r, String text) override;
							virtual	void	HanldeEndTag (SAXObjectReader &r) override;
					};
				template	<>
					class	BuiltinReader<int> : public SAXObjectReader::ObjectBase {
						public:
							BuiltinReader (int* intoVal);
						private:
							String	tmpVal_;
							int*	value_;
						public:
							virtual	void	HandleChildStart (SAXObjectReader &r, const String& uri, const String& localname, const String& qname, const map<String,Memory::VariantValue>& attrs) override;
							virtual	void	HandleTextInside (SAXObjectReader &r, String text) override;
							virtual	void	HanldeEndTag (SAXObjectReader &r) override;
					};
				template	<>
					class	BuiltinReader<Time::DateTime> : public SAXObjectReader::ObjectBase {
						public:
							BuiltinReader (Time::DateTime* intoVal);
						private:
							String			tmpVal_;
							Time::DateTime* value_;
						public:
							virtual	void	HandleChildStart (SAXObjectReader &r, const String& uri, const String& localname, const String& qname, const map<String,Memory::VariantValue>& attrs) override;
							virtual	void	HandleTextInside (SAXObjectReader &r, String text) override;
							virtual	void	HanldeEndTag (SAXObjectReader &r) override;
					};
			}
		}
	}
}
#endif	/*_Stroika_Foundation_DataExchangeFormat_XML_SAXObjectReader_inl_*/
	