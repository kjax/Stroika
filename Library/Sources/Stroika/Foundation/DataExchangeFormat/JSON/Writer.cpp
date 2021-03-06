/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2012.  All rights reserved
 */
#include    "../../StroikaPreComp.h"

#include    <sstream>

#include    "Writer.h"


using   namespace   Stroika::Foundation;
using   namespace   Stroika::Foundation::DataExchangeFormat;


/*
 * TODO:
 *      (1) Must add support for quoted characters like '"' etc - which cause trouble inside json. Also - quoted utf-8 characters.
 */



/*
 ********************************************************************************
 *************** DataExchangeFormat::JSON::PrettyPrint **************************
 ********************************************************************************
 */
namespace   {
    void    Indent_ (ostream& out, int indentLevel)
    {
        for (int i = 0; i < indentLevel; ++i) {
            out << "    ";
        }
    }
}
namespace   {
    void    PrettyPrint_ (const Memory::VariantValue& v, ostream& out, int indentLevel);
    void    PrettyPrint_Null_ (ostream& out)
    {
        out << "null";
    }
    void    PrettyPrint_ (bool v, ostream& out)
    {
        if (v) {
            out << "true";
        }
        else {
            out << "false";
        }
    }
    void    PrettyPrint_ (int v, ostream& out)
    {
        out << v;
    }
    void    PrettyPrint_ (float v, ostream& out)
    {
        out << v;
    }
    void    PrettyPrint_ (const wstring& v, ostream& out)
    {
        out << "\"";
        string  tmp =   Characters::WideStringToUTF8 (v);
        for (string::const_iterator i = tmp.begin (); i != tmp.end (); ++i) {
            switch (*i) {
                case '\"':
                    out << "\\\"";
                    break;
                case '\\':
                    out << "\\\\";
                    break;
                case '\n':
                    out << "\\n";
                    break;
                case '\r':
                    out << "\\r";
                    break;
// unclear if we need to quote other chars such as \f\t\b\ but probably not...
                default:
                    out << *i;
                    break;
            }
        }
        out << "\"";
    }
    void    PrettyPrint_ (const vector<Memory::VariantValue>& v, ostream& out, int indentLevel)
    {
        out << "[" << endl;
        for (vector<Memory::VariantValue>::const_iterator i = v.begin (); i != v.end (); ++i) {
            Indent_ (out, indentLevel + 1);
            PrettyPrint_ (*i, out, indentLevel + 1);
            if (i + 1 != v.end ()) {
                out << ",";
            }
            out << endl;
        }
        Indent_ (out, indentLevel);
        out << "]";
    }
    void    PrettyPrint_ (const map<wstring, Memory::VariantValue>& v, ostream& out, int indentLevel)
    {
        out << "{" << endl;
        for (map<wstring, Memory::VariantValue>::const_iterator i = v.begin (); i != v.end ();) {
            Indent_ (out, indentLevel + 1);
            PrettyPrint_ (i->first, out, indentLevel + 1);
            out << " : ";
            PrettyPrint_ (i->second, out, indentLevel + 1);
            ++i;
            if (i != v.end ()) {
                out << ",";
            }
            out << endl;
        }
        Indent_ (out, indentLevel);
        out << "}";
    }
    void    PrettyPrint_ (const Memory::VariantValue& v, ostream& out, int indentLevel)
    {
        switch (v.GetType ()) {
            case    Memory::VariantValue::eNull:
                PrettyPrint_Null_ (out);
                break;
            case    Memory::VariantValue::eBoolean:
                PrettyPrint_ (v.As<bool> (), out);
                break;
            case    Memory::VariantValue::eInteger:
                PrettyPrint_ (v.As<int> (), out);
                break;
            case    Memory::VariantValue::eFloat:
                PrettyPrint_ (v.As<float> (), out);
                break;
            case    Memory::VariantValue::eString:
                PrettyPrint_ (v.As<wstring> (), out);
                break;
            case    Memory::VariantValue::eMap:
                PrettyPrint_ (v.As<map<wstring, Memory::VariantValue>> (), out, indentLevel);
                break;
            case    Memory::VariantValue::eArray:
                PrettyPrint_ (v.As<vector<Memory::VariantValue>> (), out, indentLevel);
                break;
            default:
                RequireNotReached ();       // only certain types allowed
        }
    }
}

void    DataExchangeFormat::JSON::PrettyPrint (const Memory::VariantValue& v, ostream& out)
{
    PrettyPrint_ (v, out, 0);
}
