/* Copyright(c) Sophist Solutions, Inc. 1994-2001.  All rights reserved */

/*
 * $Header: /home/lewis/share/RFLWork_CVSRoot/DevRoot/ThirdPartyLibs/Led/LedSrc/Sources/Led_BiDiLayoutEngine.cpp,v 1.3 2009-05-31 15:31:35 Lewis Exp $
 *
 * Changes:
 *	$Log: Led_BiDiLayoutEngine.cpp,v $
 *	Revision 1.3  2009-05-31 15:31:35  Lewis
 *	*
 *	*********************     HealthFrameWorks Server 1.0      **************************
 *	*
 *
 *	Revision 1.2  2006/10/25 00:34:41  lewis
 *	*
 *	**************************         HealthFrame 2.1         **************************
 *	*
 *	
 *	Revision 1.1  2006/08/31 19:39:35  lewis
 *	copied from latest (3.1b2x) Led code - but renamed to Led_ file names and fixed MINOR issues to get to compile with HF/Newer Visual Studio
 *	
 *	Revision 2.28  2003/12/12 19:14:02  lewis
 *	SPR#1596: overlaod TextLayoutBlock_Copy::BlockRep::operator delete () cuz we allocate using new[].
 *	
 *	Revision 2.27  2003/12/12 01:54:07  lewis
 *	*** empty log message ***
 *	
 *	Revision 2.26  2003/12/01 02:24:04  lewis
 *	SPR#1566: fix TextLayoutBlock_Basic::Construct_UNISCRIBE () to compute visualSegStarts() array and then use that inside loop. This way we get the speedtweek of the earlier fix, but its now also correct.
 *	
 *	Revision 2.25  2003/11/27 20:16:44  lewis
 *	SPR#1566: BIG tweek to Construct_UNISCRIBE () - at least big for very long lines (30K segments). Also added operator== (const TextLayoutBlock& lhs, const TextLayoutBlock& rhs)
 *	
 *	Revision 2.24  2003/11/06 16:27:43  lewis
 *	*** empty log message ***
 *	
 *	Revision 2.23  2003/11/03 20:02:58  lewis
 *	SPR#1551: Lose unused qMapR2VPosForTL stuff. Add new efficeint storage/copying TextLayoutBlock_Copy class (performance). Add 'initialDirection' CTOR argument so you can specify (optionally) the base direction to use for a given line layout (when its externally specified, like on successive rows of a paragraph)
 *	
 *	Revision 2.22  2003/03/07 21:05:58  lewis
 *	SPR#1327- Added CharacterProperties::IsMirrored (), and used it in the UNISCRIBE version
 *	of TextLayoutBlock_Basic::Construct_UNISCRIBE ().
 *	
 *	Revision 2.21  2003/02/28 19:03:22  lewis
 *	SPR#1316- CRTDBG_MAP_ALLOC/CRTDBG_MAP_ALLOC_NEW Win32/MSVC debug mem leak support
 *	
 *	Revision 2.20  2003/02/24 23:03:13  lewis
 *	cleanup warnings
 *	
 *	Revision 2.19  2003/02/13 15:49:43  lewis
 *	SPR#1295 - speed tweek to TextLayoutBlock_Basic::Construct_UNISCRIBE - maxToCheck
 *	
 *	Revision 2.18  2003/01/13 04:01:55  lewis
 *	SPR#1236- memoized version of GetInitialUNISCRIBEDir used to provide initial dir for UNISCRIBE ScriptItemize
 *	
 *	Revision 2.17  2003/01/12 23:16:03  lewis
 *	change so qUseFriBidi defaults to OFF and if on it takes precedence over qTryToUseUNISCRIBEForTextRuns
 *	
 *	Revision 2.16  2003/01/10 22:21:08  lewis
 *	SPR#1231 - fixed TextLayoutBlock_Basic::Construct_Default () to not create empty element if no
 *	text and imrpoved Invariant_ ()
 *	
 *	Revision 2.15  2003/01/10 18:12:34  lewis
 *	SPR#1224 - Cleanup TextLayoutBlock_Basic::Construct_UNISCRIBE so produces results more like fribidi and MSWord.
 *	
 *	Revision 2.14  2003/01/10 15:22:26  lewis
 *	as part of SPR#1226 - added prelim MapRealOffsetToVirtual/MapVirtualOffsetToReal methods, and
 *	qDebugHack_UpperCaseCharsTratedAsRTL support
 *	
 *	Revision 2.13  2003/01/02 15:11:33  lewis
 *	SPR#1218- restructure code so can use qTryToUseUNISCRIBEForTextRuns or other means to breakup text
 *	
 *	Revision 2.12  2002/12/13 18:39:51  lewis
 *	SPR#1204 - added TextLayoutBlock::GetCharacterDirection () API
 *	
 *	Revision 2.11  2002/12/12 16:49:53  lewis
 *	BIDI work- Added CopyOutRealText/CopyOutVirtualText (maybe replace the peek methods with this?). Added
 *	qDebugHack_ReverseDirections define so I can test with ENGLISH text (like abc) and see it reversed.
 *	Debugging hack only
 *	
 *	Revision 2.10  2002/12/04 19:38:34  lewis
 *	SPR#1194- fix buffer sizes for fribidi calls - to avoid crash. Also - added qMapR2VPosForTL to only conidiotnally
 *	make that available. Get rid of fVirtaulLength/fRealLength distinction - make API assume these are always the same
 *	
 *	Revision 2.9  2002/12/04 16:45:38  lewis
 *	*** empty log message ***
 *	
 *	Revision 2.8  2002/12/03 15:49:04  lewis
 *	SPR#1191- use new TextDirection enum defined in LedGDI.h. Get rid of ScriptRunElement::fLength. Add direction
 *	arg to Led_Tablet_::TabbedTextOut () and use that to call (WIN32)Led_Tablet_::SetTextAlign(). Hopefully this
 *	fixes display on Win9x of Arabic text.
 *	
 *	Revision 2.7  2002/12/02 00:49:10  lewis
 *	SPR#1183 BIDI code - added TextLayoutBlock::LessThanVirtualStart and cleanup a few warningsHeaders/BiDiLayoutEngine.h
 *	
 *	Revision 2.6  2002/11/28 20:54:47  lewis
 *	work on BIDI code (SPR#1183) - Add GetRealText/GetVirtualText no-arg routines. Plus, lots of fixups to the scriptRun code
 *	
 *	Revision 2.5  2002/11/28 03:46:19  lewis
 *	added fRealText to the TextLayoutBlock_VirtualSubset class so it could copy the source text by value and have
 *	a buffer  of the right size and contiguous
 *	
 *	Revision 2.4  2002/11/28 02:16:20  lewis
 *	tons of changes/improvements/cleanups. Temporarily get rid of MapReal/VirtualOffsetToEachOther routines since unused right now,
 *	and I'm not sure I'll need them. Wait and see. Added/fixed GetScriptRuns code to keep track of MUCH more more information.
 *	THIS is the basic information I will really use/need.
 *	
 *	Revision 2.3  2002/11/27 15:10:57  lewis
 *	add GetVirtualText{Length} method and fix code for PeekAt in SubTextLayoutBlock code (end correctly
 *	
 *	Revision 2.2  2002/11/26 22:45:43  lewis
 *	looked into using IBM's ICU package for bidi layout - but it was to big and intrusive to be worth the
 *	effort right now. Also - added GetScriptRuns () support to API and a few other small improvements
 *	
 *	Revision 2.1  2002/11/25 23:38:27  lewis
 *	SPR#1183- checked in very prelim version of BidiLayoutEngine
 *	
 *	
 *
 *
 *
 */
#if		qIncludePrefixFile
	#include	"stdafx.h"
#endif

#include	<algorithm>

#include	"Led_CodePage.h"

#include	"Led_BiDiLayoutEngine.h"


#if		qTryToUseUNISCRIBEForTextRuns
#include	<Usp10.h>
#endif

#if		qUseFriBidi
#include	"fribidi.h"
#endif

#if		qUseICUBidi
#include	"unicode/ubidi.h"
#endif





/*
 *	Hack to build BiDiLevelProc for SPR#1236
 */
#ifndef	qBuildMemoizedBiDiLevelProc
	#define	qBuildMemoizedBiDiLevelProc		0
#endif

#if		qBuildMemoizedBiDiLevelProc
	#include	<fstream>
#endif


#if		defined (CRTDBG_MAP_ALLOC_NEW)
	#define	new	CRTDBG_MAP_ALLOC_NEW
#endif




#if		qLedUsesNamespaces
	namespace	Led {
#endif




/*
 *	This hack reverses the direction results returned by this class for basic text parsing/layout. This is
 *	useful to debug/test - cuz then I can see ENGLISH text (words) displayed RTL (right to left) - which I can
 *	better identify and see if they are working properly.
 *
 *	Note that this ONLY reverses directions WITHIN a run - not the order of the runs. Thats still good enough
 *	to debug most display issues.
 */
#ifndef	qDebugHack_ReverseDirections
#define	qDebugHack_ReverseDirections	0
#endif





/*
 *	This hack treats all english aplhabetic upper case characters as RTL characters, and lower case characters
 *	as LTR characters. This hack COULD be improved to actually PRESERVE the original characters in the virtual
 *	output, but that hasn't been done yet.
 */
#ifndef	qDebugHack_UpperCaseCharsTratedAsRTL
#define	qDebugHack_UpperCaseCharsTratedAsRTL	0
#endif







/*
 *	Code to test how FriBidi does compared with UNISCRIBE for laying out Arabic text. for plain English text
 *	these are the same. But - even simple Arabic files seem to produce different results between FriBidi and
 *	UNISCRIBE. For example - an Arabic string whcih ends in a period - with FriBidi - it all ends up as one
 *	LTR run. But - with UNISCRIBE - you get two runs - and the period comes out on the right.
 */
#ifndef	qTestUNISCRIBEResultsEqualFriBidi
	#if		qDebug && qTryToUseUNISCRIBEForTextRuns && qUseFriBidi
//		#define	qTestUNISCRIBEResultsEqualFriBidi	0
		#define	qTestUNISCRIBEResultsEqualFriBidi	1
	#endif
#endif

enum	InitialUNISCRIBEDir { eLTRChar, eRTLChar, eNeutralChar };
static	InitialUNISCRIBEDir	myGetInitialUNISCRIBEDir (wchar_t c);



/*
 *	Use this to test my myGetInitialUNISCRIBEDir function produce the right results. (SPR#1236).
 */
#ifndef	qTestMyWriteMemoizedUniscribeDirFunction
	#define	qTestMyWriteMemoizedUniscribeDirFunction			(qUseFriBidi && qTryToUseUNISCRIBEForTextRuns)
#endif




#if		qTestMyWriteMemoizedUniscribeDirFunction || qBuildMemoizedBiDiLevelProc
namespace {
	InitialUNISCRIBEDir	GetInitialUNISCRIBEDir (wchar_t c)
		{
			FriBidiCharType	ct	=	fribidi_get_type (c);
			if (FRIBIDI_IS_LETTER (ct)) {
				if (FRIBIDI_DIR_TO_LEVEL (ct) & 1) {
					return eRTLChar;
				}
				else {
					return eLTRChar;
				}
			}
			else {
				return eNeutralChar;
			}
		}
};
#endif



#if		qBuildMemoizedBiDiLevelProc
namespace {
	template	<class FUNCTION>
		void	WriteMemoizedUniscribeDirProc (FUNCTION function, const string& origFunctionName, const string& functionName)
				{
					ofstream	outStream ("IsXXXProc.txt");

					outStream << "/*\n";
					outStream << " ********************************************************************************\n";
					outStream << " ********** " << functionName << " (AUTOGENERATED memoize of " << origFunctionName << " - " << __DATE__ << ") ***********\n";
					outStream << " ********************************************************************************\n";
					outStream << " */\n";
					outStream << "// Hack for SPR#1236\n";
					outStream << "InitialUNISCRIBEDir	" << functionName << " (wchar_t c)\n";
					outStream << "{\n";

					bool	firstTime	=	true;
					InitialUNISCRIBEDir	lastIST	=	eNeutralChar;
					size_t	firstRangeIdxTrue	=	0;
					for (size_t i = 0; i < 256*256; ++i) {
						InitialUNISCRIBEDir	isT		=	function (static_cast<wchar_t> (i));
						if (i == 0) {
							lastIST = isT;
						}
						if (isT != lastIST or i >= 256*256 -1) {
							if (lastIST!= eNeutralChar) {
								// then emit the range...
								outStream << "\tif (" << firstRangeIdxTrue << " <= c && c <= " << i-1 << ") {\n";
								switch (lastIST) {
									case	eNeutralChar:	outStream << "\t\treturn eNeutralChar;\n"; break;
									case	eRTLChar:		outStream << "\t\treturn eRTLChar;\n"; break;
									case	eLTRChar:		outStream << "\t\treturn eLTRChar;\n"; break;
								}
								outStream << "\t}\n";
							}
							lastIST = isT;
							firstRangeIdxTrue = i;
						}
					}
					outStream << "\treturn eNeutralChar;\n";
					outStream << "}\n";
					outStream << "\n";
				}

		struct	DoRunIt {
			DoRunIt ()
			{
				WriteMemoizedUniscribeDirProc (GetInitialUNISCRIBEDir, "GetInitialUNISCRIBEDir", "myGetInitialUNISCRIBEDir");
			}
		}	gRunIt;
};
#endif



#if		qTestMyWriteMemoizedUniscribeDirFunction
class	MyIWriteMemoizedUniscribeDirFunction {
	public:
		MyIWriteMemoizedUniscribeDirFunction ()
			{
				for (wchar_t c = 0; c < 0xffff; ++c) {
					Led_Assert (GetInitialUNISCRIBEDir (c) == myGetInitialUNISCRIBEDir (c));
				}
			}
}	aMyIWriteMemoizedUniscribeDirFunction;
#endif





#if		qWindows && qWideCharacters && qTryToUseUNISCRIBEForTextRuns
namespace {
		
	/*
	 *	Use LoadLibrary/GetProcAddress instead of direct call to avoid having to link with
	 *	Usp10.lib. This avoidance allows us to run on systems that don't it installed.
	 */
	struct	UniscribeDLL {
		UniscribeDLL ():
			fDLL (::LoadLibrary (_T ("Usp10.dll"))),
			fScriptItemize (NULL),
			fScriptLayout (NULL)
			{
				if (fDLL != NULL) {
					fScriptItemize = (HRESULT (WINAPI *) (const WCHAR*, int, int, const SCRIPT_CONTROL*, const SCRIPT_STATE*, SCRIPT_ITEM*, int*))
							(::GetProcAddress (fDLL, "ScriptItemize"));
					fScriptLayout = (HRESULT (WINAPI *) (int, const BYTE*, int*, int*))
							(::GetProcAddress (fDLL, "ScriptLayout"));
				}
			}

		~UniscribeDLL ()
			{
				if (fDLL != NULL) {
					Led_Verify (::FreeLibrary (fDLL));
				}
			}

		nonvirtual	bool	IsAvail () const
			{
				return fDLL != NULL;
			}

		HRESULT	WINAPI	ScriptItemize (const WCHAR *pwcInChars, int cInChars, int cMaxItems, const SCRIPT_CONTROL *psControl, const SCRIPT_STATE *psState, SCRIPT_ITEM *pItems, int *pcItems)
			{
				if (fScriptItemize == NULL) {
					return E_FAIL;
				}
				return (*fScriptItemize) (pwcInChars, cInChars, cMaxItems, psControl, psState, pItems, pcItems);
			}

		HRESULT	WINAPI	ScriptLayout (int cRuns, const BYTE* pbLevel, int* piVisualToLogical, int* piLogicalToVisual)
			{
				if (fScriptLayout == NULL) {
					return E_FAIL;
				}
				return (*fScriptLayout) (cRuns, pbLevel, piVisualToLogical, piLogicalToVisual);
			}


		HINSTANCE			fDLL;
		HRESULT	 (WINAPI*	fScriptItemize) (const WCHAR*, int, int, const SCRIPT_CONTROL*, const SCRIPT_STATE*, SCRIPT_ITEM*, int*);
		HRESULT	 (WINAPI*	fScriptLayout) (int, const BYTE*, int*, int*);
	};
	static	UniscribeDLL	sUniscribeDLL;
};
#endif





		
/*
 ********************************************************************************
 ******************************** TextLayoutBlock *******************************
 ********************************************************************************
 */
TextLayoutBlock::TextLayoutBlock ()
{
}

TextLayoutBlock::~TextLayoutBlock ()
{
}

/*
@METHOD:		TextLayoutBlock::GetCharacterDirection
@DESCRIPTION:	
*/
TextDirection	TextLayoutBlock::GetCharacterDirection (size_t realCharOffset) const
{
	vector<ScriptRunElt>	runs		=	GetScriptRuns ();
	for (vector<ScriptRunElt>::const_iterator i = runs.begin (); i != runs.end (); ++i) {
		if ((*i).fRealStart <= realCharOffset and realCharOffset < (*i).fRealEnd) {
			return (*i).fDirection;
		}
	}
	Led_Assert (false); return eLeftToRight;
}

/*
@METHOD:		TextLayoutBlock::MapRealOffsetToVirtual
@DESCRIPTION:	
*/
size_t	TextLayoutBlock::MapRealOffsetToVirtual (size_t /*i*/) const
{
	Led_Assert (false);	//NYI
	return 0;
}

/*
@METHOD:		TextLayoutBlock::MapVirtualOffsetToReal
@DESCRIPTION:	
*/
size_t	TextLayoutBlock::MapVirtualOffsetToReal (size_t i) const
{
	Led_Require (i < GetTextLength ());
	vector<ScriptRunElt>	runs		=	GetScriptRuns ();
	for (vector<ScriptRunElt>::const_iterator runIt = runs.begin (); runIt != runs.end (); ++runIt) {
		const ScriptRunElt&	se	=	*runIt;
		if (se.fVirtualStart <= i and i < se.fVirtualEnd) {
			if (se.fDirection == eLeftToRight) {
				size_t	result	=	(i - se.fVirtualStart) + se.fRealStart;
				Led_Ensure (result < GetTextLength ());
				return (result);
			}
			else {
				size_t	segLen	=	se.fRealEnd - se.fRealStart;
				size_t	result	=	(segLen - (i - se.fVirtualStart)) - 1 + se.fRealStart;
				Led_Ensure (result < GetTextLength ());
				return (result);
			}
		}
	}
	Led_Assert (false);	// BAD INDEX
	return 0;
}

void	TextLayoutBlock::CopyOutRealText (const ScriptRunElt& scriptRunElt, Led_tChar* buf) const
{
	const Led_tChar*	s	=	NULL;
	const Led_tChar*	e	=	NULL;
	PeekAtRealText (scriptRunElt, &s, &e);
	copy (s, e, buf);
}

void	TextLayoutBlock::CopyOutVirtualText (const ScriptRunElt& scriptRunElt, Led_tChar* buf) const
{
	const Led_tChar*	s	=	NULL;
	const Led_tChar*	e	=	NULL;
	PeekAtVirtualText (scriptRunElt, &s, &e);
	copy (s, e, buf);
}

void	TextLayoutBlock::PeekAtRealText (const ScriptRunElt& scriptRunElt, const Led_tChar** startText, const Led_tChar** endText) const
{
	Led_RequireNotNil (startText);
	Led_RequireNotNil (endText);
	PeekAtRealText_ (startText, endText);
	*startText += scriptRunElt.fRealStart;
	size_t	len	=	scriptRunElt.fRealEnd - scriptRunElt.fRealStart;
	Led_Assert (*endText - *startText >= static_cast<ptrdiff_t> (len));		// make sure we are SHRINKING the text and not making it point past its end
	*endText = *startText + len;
}

Led_tString	TextLayoutBlock::GetRealText () const
{
	const Led_tChar*	s	=	NULL;
	const Led_tChar*	e	=	NULL;
	PeekAtRealText_ (&s, &e);
	return Led_tString (s, e);
}

Led_tString	TextLayoutBlock::GetRealText (const ScriptRunElt& scriptRunElt) const
{
	const Led_tChar*	s	=	NULL;
	const Led_tChar*	e	=	NULL;
	PeekAtRealText (scriptRunElt, &s, &e);
	return Led_tString (s, e);
}

void	TextLayoutBlock::PeekAtVirtualText (const ScriptRunElt& scriptRunElt, const Led_tChar** startText, const Led_tChar** endText) const
{
	Led_RequireNotNil (startText);
	Led_RequireNotNil (endText);
	PeekAtVirtualText_ (startText, endText);
	*startText += scriptRunElt.fVirtualStart;
	size_t	len	=	scriptRunElt.fVirtualEnd - scriptRunElt.fVirtualStart;
	Led_Assert (*endText - *startText >= static_cast<ptrdiff_t> (len));		// make sure we are SHRINKING the text and not making it point past its end
	*endText = *startText + len;
}

Led_tString	TextLayoutBlock::GetVirtualText () const
{
	const Led_tChar*	s	=	NULL;
	const Led_tChar*	e	=	NULL;
	PeekAtVirtualText_ (&s, &e);
	return Led_tString (s, e);
}

Led_tString	TextLayoutBlock::GetVirtualText (const ScriptRunElt& scriptRunElt) const
{
	const Led_tChar*	s	=	NULL;
	const Led_tChar*	e	=	NULL;
	PeekAtVirtualText (scriptRunElt, &s, &e);
	return Led_tString (s, e);
}

#if		qDebug
void	TextLayoutBlock::Invariant_ () const
{
	size_t	len	=	GetTextLength ();

	vector<ScriptRunElt>	scriptRuns	=	GetScriptRuns ();

	// Make sure each scriptrun elt is non-empty
	{
		for (vector<ScriptRunElt>::const_iterator j = scriptRuns.begin (); j != scriptRuns.end (); ++j) {
			Led_Assert ((*j).fRealStart < len);
			Led_Assert ((*j).fRealEnd <= len);
			Led_Assert ((*j).fVirtualStart < len);
			Led_Assert ((*j).fVirtualEnd <= len);
			Led_Assert ((*j).fRealStart < (*j).fRealEnd);	// no empty elements
		}
	}

	/*
	 *	Validate the fScriptRuns list. Make sure it completely covers the real and virtual range. Make
	 *	sure each element has exactly LENGTH spaces used up for the real text and the virtual text.
	 *	That - by itself - assures also that there are no overlapping elements.
	 */
	{
		for (size_t i = 0; i < len; ++i) {
			// Assure pos 'i' found in source and virtual
			bool	foundReal		=	false;
			bool	foundVirtual	=	false;
			size_t	nRealFound		=	0;
			size_t	nVirtualFound	=	0;
			for (vector<ScriptRunElt>::const_iterator j = scriptRuns.begin (); j != scriptRuns.end (); ++j) {
				if ((*j).fRealStart <= i and i < (*j).fRealEnd) {
					Led_Assert (not foundReal);
					foundReal = true;
				}
				Led_Assert ((*j).fVirtualStart < (*j).fVirtualEnd);	// no empty elements
				if ((*j).fVirtualStart <= i and i < (*j).fVirtualEnd) {
					Led_Assert (not foundVirtual);
					foundVirtual = true;
				}
				nRealFound += (*j).fRealEnd - (*j).fRealStart;
				nVirtualFound += (*j).fVirtualEnd - (*j).fVirtualStart;
			}
			Led_Assert (foundReal);
			Led_Assert (foundVirtual);
			Led_Assert (nRealFound == len);
			Led_Assert (nVirtualFound == len);
		}
	}
}
#endif

bool	operator== (const TextLayoutBlock& lhs, const TextLayoutBlock& rhs)
{
	if (lhs.GetTextLength () != rhs.GetTextLength ()) {
		return false;
	}
	if (lhs.GetRealText () != rhs.GetRealText ()) {
		return false;
	}
	if (lhs.GetVirtualText () != rhs.GetVirtualText ()) {
		return false;
	}
	vector<TextLayoutBlock::ScriptRunElt>	lhsSR	=	lhs.GetScriptRuns ();
	vector<TextLayoutBlock::ScriptRunElt>	rhsSR	=	rhs.GetScriptRuns ();
	if (lhsSR.size () != rhsSR.size ()) {
		return false;
	}
	for (size_t i = 0; i < lhsSR.size (); ++i) {
		if (lhsSR[i] != rhsSR[i]) {
			return false;
		}
	}
	return true;
}







/*
 ********************************************************************************
 ****************************** TextLayoutBlock_Basic ***************************
 ********************************************************************************
 */
TextLayoutBlock_Basic::TextLayoutBlock_Basic (const Led_tChar* realText, const Led_tChar* realTextEnd):
	inherited (),
	fTextLength (0),
	fRealText (0),
	fVirtualText (0),
	fScriptRuns ()
{
	Construct (realText, realTextEnd, NULL);
}

TextLayoutBlock_Basic::TextLayoutBlock_Basic (const Led_tChar* realText, const Led_tChar* realTextEnd, TextDirection initialDirection):
	inherited (),
	fTextLength (0),
	fRealText (0),
	fVirtualText (0),
	fScriptRuns ()
{
	Construct (realText, realTextEnd, &initialDirection);
}

void	TextLayoutBlock_Basic::Construct (const Led_tChar* realText, const Led_tChar* realTextEnd, const TextDirection* initialDirection)
{
	#if		!qWideCharacters
		Led_Arg_Unused (initialDirection);
	#endif
	size_t	textLength	=	realTextEnd - realText;
	fTextLength = textLength;
	fRealText.GrowToSize (textLength);
	fVirtualText.GrowToSize (textLength);

	copy (realText, realText + textLength, static_cast<Led_tChar*> (fRealText));

	#if		qDebugHack_UpperCaseCharsTratedAsRTL
	{
		for (size_t i = 0; i < textLength; ++i) {
			if ('A' <= fRealText[i] and fRealText[i] <= 'Z') {
				fRealText[i] = 0xfe7d;	// random arabic character
			}
		}
	}
	#endif

	/*
	 * Try a number of different ways to generate the virtual text/maps etc.
	 */
	#if		qTestUNISCRIBEResultsEqualFriBidi
		(void)Construct_UNISCRIBE (initialDirection);
		Led_SmallStackBuffer<Led_tChar>	savedVirtualText (fTextLength);
		copy (static_cast<const Led_tChar*> (fVirtualText), static_cast<const Led_tChar*> (fVirtualText) + fTextLength, static_cast<Led_tChar*> (savedVirtualText));
		vector<ScriptRunElt>			savedScriptRuns = fScriptRuns;
		fScriptRuns = vector<ScriptRunElt> ();
		Construct_FriBidi (initialDirection);
		{
			for (size_t i = 0; i < fTextLength; ++i) {
				Led_Assert (savedVirtualText[i] == fVirtualText[i]);
			}
		}
		{
			Led_Assert (savedScriptRuns.size () == fScriptRuns.size ());
			for (size_t i = 0; i < savedScriptRuns.size (); ++i) {
				Led_Assert (savedScriptRuns[i] == fScriptRuns[i]);
			}
		}
	#else
		#if		qUseFriBidi
			Construct_FriBidi (initialDirection); if (1) {} else
		#elif	qTryToUseUNISCRIBEForTextRuns
			if (Construct_UNISCRIBE (initialDirection)) {} else
		#elif	qUseICUBidi
			Construct_ICU (initialDirection); if (1) {} else
		#endif
			{
				Construct_Default ();
			}
	#endif

	#if		qDebugHack_ReverseDirections
		{
			for (vector<ScriptRunElt>::iterator i = fScriptRuns.begin (); i != fScriptRuns.end (); ++i) {
				ScriptRunElt&	se	=	*i;
				TextDirection	newDir	=	(se.fDirection == eLeftToRight)? eRightToLeft: eLeftToRight;
				se.fDirection = newDir;
				// now reverse the virtual text in the run...
				size_t	runLen	=	se.fVirtualEnd - se.fVirtualStart;
				Led_SmallStackBuffer<Led_tChar>	reverseBuf (runLen);
				for (size_t j = se.fVirtualStart; j < se.fVirtualEnd; ++j) {
					reverseBuf[runLen-1-(j-se.fVirtualStart)] = fVirtualText[j];
				}
				copy (static_cast<Led_tChar*> (reverseBuf), static_cast<Led_tChar*> (reverseBuf) + runLen, static_cast<Led_tChar*> (fVirtualText) + se.fVirtualStart);
			}
		}
	#endif

	Invariant ();
}

#if		qTryToUseUNISCRIBEForTextRuns
bool	TextLayoutBlock_Basic::Construct_UNISCRIBE (const TextDirection* initialDirection)
{
	if (sUniscribeDLL.IsAvail () and fTextLength > 0) {
		/*
		 *	See SPR#1224 for why we pass along zeroed scriptControl/scriptState.
		 */
		Led_SmallStackBuffer<SCRIPT_ITEM>		scriptItems (fTextLength+1);
		int										nScriptItems	=	0;
		SCRIPT_CONTROL							scriptControl;
		SCRIPT_STATE							scriptState;
		memset (&scriptControl, 0, sizeof (scriptControl));
		memset (&scriptState, 0, sizeof (scriptState));

		// Guess inital uBidiLevel to simulate the _ON feature of fribidi
		{
			InitialUNISCRIBEDir	dir	=	eNeutralChar;
			if (initialDirection == NULL) {
				size_t	maxToCheck	=	min (fTextLength, 100);	// don't bother checking more than 100 characters, as this CAN be slow
																// in degenerate cases (see SPR#1295). Not such a big deal to get this right
																// in those crazy cases.
				for (size_t i = 0; i < maxToCheck; ++i) {
					dir = myGetInitialUNISCRIBEDir (fRealText[i]);
					if (dir != eNeutralChar) {
						break;
					}
				}
			}
			else {
				dir = (*initialDirection == eLeftToRight)? eLTRChar: eRTLChar;
			}
			if (dir == eRTLChar) {
				scriptState.uBidiLevel = 1;
			}
		}
		Led_Verify (sUniscribeDLL.ScriptItemize (static_cast<const Led_tChar*> (fRealText), fTextLength, fTextLength+1, &scriptControl, &scriptState, scriptItems, &nScriptItems) == S_OK);
		Led_Assert (nScriptItems >= 1);

		Led_SmallStackBuffer<BYTE>	bidiLevels (nScriptItems);
		for (size_t i = 0; i < static_cast<size_t> (nScriptItems); ++i) {
			bidiLevels[i] = scriptItems[i].a.s.uBidiLevel;
		}

		Led_SmallStackBuffer<int>	visualToLogical (nScriptItems);
		Led_SmallStackBuffer<int>	logicalToVisual (nScriptItems);

		Led_Verify (sUniscribeDLL.ScriptLayout (nScriptItems, bidiLevels, visualToLogical, logicalToVisual) == S_OK);

		/*
		 *	Create an array (indexed by virtual segment number) of that segments display start. The first obviously
		 *	starts at zero offset. The subsequent ones are offset from their predecessors by the width of that predecessor
		 *	(a slightly tricky computation - noting that we must map back to logical coords to use the scriptItems array
		 *	and note that the UNISCRIBE API gives you the length of a cell by taking diff between it and following cell start).
		 */
		Led_SmallStackBuffer<size_t>	visualSegStarts (nScriptItems);
		{
			Led_Assert (nScriptItems > 0);
			visualSegStarts[0] = 0;
			for (size_t visIndex = 1; visIndex < static_cast<size_t> (nScriptItems); ++visIndex) {
				size_t  prevLogIdx		=	visualToLogical[visIndex-1];
				size_t	prevItemWidth	=	scriptItems[prevLogIdx+1].iCharPos - scriptItems[prevLogIdx].iCharPos;
				visualSegStarts[visIndex] = visualSegStarts[visIndex-1] + prevItemWidth;
			}
		}

		{
			const Led_tChar*	rT	=	static_cast<Led_tChar*> (fRealText);
			Led_tChar*			vT	=	static_cast<Led_tChar*> (fVirtualText);

			for (size_t i = 0; i < static_cast<size_t> (nScriptItems); ++i) {
				TextDirection	curDir	=	(bidiLevels[i] & 1)?
											eRightToLeft:
											eLeftToRight
									;
				// Construct a very simple one-char wide run elt with appropriate direction info and 
				// to find it in src text or virtual text
				ScriptRunElt	s;
				s.fDirection = curDir;
				s.fRealStart = scriptItems[i].iCharPos;
				s.fRealEnd = scriptItems[i+1].iCharPos;

				#if		qDebug
					{
						for (size_t j = 0; j < static_cast<size_t> (nScriptItems); ++j) {
							Led_Assert (logicalToVisual[visualToLogical[i]] == i);
						}
					}
					size_t	referenceVirtualStart	=	0;
					{
						referenceVirtualStart = 0;
						for (int visIndex = logicalToVisual[i] - 1; visIndex >= 0; --visIndex) {
							Led_Assert (visIndex < nScriptItems);
							size_t  itsLogIdx       =       visualToLogical[visIndex];
							referenceVirtualStart += (scriptItems[itsLogIdx+1].iCharPos - scriptItems[itsLogIdx].iCharPos);
						}
					}
					Led_Assert (referenceVirtualStart == visualSegStarts[logicalToVisual[i]]);
				#endif

				// Now precompute visual segment starts arrays and just use logicalToVisual to find the appropriate
				// visual segment (SPR#1566) - speed tweek - to precompute array, and just use here -- LGP 2003-11-30.
				s.fVirtualStart = visualSegStarts[logicalToVisual[i]];
				s.fVirtualEnd = s.fVirtualStart + (s.fRealEnd - s.fRealStart);
				Led_Assert (s.fVirtualEnd - s.fVirtualStart == s.fRealEnd - s.fRealStart);

				// Check that each run element satisfies these contraints:
				//		o	Same direction
				//		o	Source text abuts (on left or right)
				//		o	Virtual text abuts (on left or right)
				//
				//		If the element satisfies these contraints - then merge it with the current last element,
				//	and otherwise, construct a new element.
				//
				//		NB: This merging behavir isn't really needed. It will work fine without it. ScriptItemize
				//	appears to generate lots of run elements with the same direction and that are adjacent.
				//	I'm not sure why this is useful - at least how I work right now - so we just merge them
				//	together.
				//
				const	bool	kDoMerging	=	true;
				if (kDoMerging) {
					if (fScriptRuns.empty ()) {
						ScriptRunElt	e	=	s;
						e.fRealEnd = e.fRealStart;
						e.fVirtualEnd = e.fVirtualStart;
						fScriptRuns.push_back (e);
					}
					ScriptRunElt&	lastElt	=	fScriptRuns.back ();

					if (lastElt.fDirection == s.fDirection and
						(lastElt.fRealEnd == s.fRealStart or lastElt.fRealStart == s.fRealEnd) and
						(lastElt.fVirtualEnd == s.fVirtualStart or lastElt.fVirtualStart == s.fVirtualEnd)
						) {
						lastElt.fRealStart = min (lastElt.fRealStart, s.fRealStart);
						lastElt.fRealEnd = max (lastElt.fRealEnd, s.fRealEnd);
						lastElt.fVirtualStart = min (lastElt.fVirtualStart, s.fVirtualStart);
						lastElt.fVirtualEnd = max (lastElt.fVirtualEnd, s.fVirtualEnd);
					}
					else {
						fScriptRuns.push_back (s);
					}
				}
				else {
					fScriptRuns.push_back (s);
				}

				if (curDir == eLeftToRight) {
					copy (rT + s.fRealStart, rT + s.fRealEnd, vT + s.fVirtualStart);
				}
				else {
					reverse_copy (rT + s.fRealStart, rT + s.fRealEnd, vT + s.fVirtualStart);
					/*
					 *	Walk through the RTL run and see if we need to mirror any characters.
					 */
					for (Led_tChar* cur = vT + s.fVirtualStart; cur < vT + s.fVirtualStart + (s.fRealEnd - s.fRealStart); ++cur) {
						Led_tChar	mirrorChar	=	'\0';
						if (CharacterProperties::IsMirrored (*cur, &mirrorChar)) {
							*cur = mirrorChar;
						}
					}
				}
			}
		}

		return true;
	}
	return false;
}
#endif

#if		qUseFriBidi
void	TextLayoutBlock_Basic::Construct_FriBidi (const TextDirection* initialDirection)
{
	#if		!qWideCharacters
		#error "Not implemented - only support FriBidi for UNICODE version of Led right now"
	#endif

	Led_Assert (fTextLength <= FRIBIDI_MAX_STRING_LENGTH);

	FriBidiCharType							baseDir	=	FRIBIDI_TYPE_ON;	// http://imagic.weizmann.ac.il/~dov/freesw/FriBidi/ docs indicate FRIBIDI_TYPE_N is a good default value - 2002-11-27
																			// but fribidi_types.h header says FRIBIDI_TYPE_N is obsolete name - use FRIBIDI_TYPE_ON instead
	if (initialDirection != NULL) {
		baseDir = (*initialDirection == eLeftToRight)? FRIBIDI_TYPE_L: FRIBIDI_TYPE_R;
	}
	Led_SmallStackBuffer<FriBidiChar>		srcText (fTextLength);
	Led_SmallStackBuffer<FriBidiChar>		vText (fTextLength + 1);		// fribidi_log2vis NUL-terminates the string
	copy (static_cast<Led_tChar*> (fRealText), fRealText + fTextLength, static_cast<FriBidiChar*> (srcText));
	Led_SmallStackBuffer<FriBidiStrIndex>	posLtoVList (fTextLength);
	Led_SmallStackBuffer<FriBidiLevel>		bidiLevels (fTextLength*2);	// no docs on size - but looking at code it appears it can be up to this big...
																		// LGP 2002-12-04

	bool	result	=	::fribidi_log2vis (srcText, fTextLength, &baseDir,
								vText,
								posLtoVList, NULL,
								bidiLevels
							);
	Led_Assert (result);


	{
		for (size_t i = 0; i < fTextLength; ++i) {
			TextDirection	curDir	=	(bidiLevels[i] & 1)?
										eRightToLeft:
										eLeftToRight
								;
			// Construct a very simple one-char wide run elt with appropriate direction info and 
			// to find it in src text or virtual text
			ScriptRunElt	thisCharElt;
			thisCharElt.fDirection = curDir;
			thisCharElt.fRealStart = i;
			thisCharElt.fRealEnd = thisCharElt.fRealStart + 1;
			thisCharElt.fVirtualStart = posLtoVList[i];
			thisCharElt.fVirtualEnd = thisCharElt.fVirtualStart + 1;

			if (i == 0) {
				ScriptRunElt	s	=	thisCharElt;
				s.fRealEnd = s.fRealStart;
				s.fVirtualEnd = s.fVirtualStart;
				fScriptRuns.push_back (s);
			}
			ScriptRunElt&	lastElt	=	fScriptRuns[fScriptRuns.size ()-1];

			// Check that each run element satisfies these contraints:
			//		o	Same direction
			//		o	Source text abuts (on left or right)
			//		o	Virtual text abuts (on left or right)
			//
			//		If the element satisfies these contraints - then merge it with the current last element,
			//	and otherwise, construct a new element.
			//
			if (lastElt.fDirection == thisCharElt.fDirection and
				(lastElt.fRealEnd == thisCharElt.fRealStart or lastElt.fRealStart == thisCharElt.fRealEnd) and
				(lastElt.fVirtualEnd == thisCharElt.fVirtualStart or lastElt.fVirtualStart == thisCharElt.fVirtualEnd)
				) {
				lastElt.fRealStart = min (lastElt.fRealStart, thisCharElt.fRealStart);
				lastElt.fRealEnd = max (lastElt.fRealEnd, thisCharElt.fRealEnd);
				lastElt.fVirtualStart = min (lastElt.fVirtualStart, thisCharElt.fVirtualStart);
				lastElt.fVirtualEnd = max (lastElt.fVirtualEnd, thisCharElt.fVirtualEnd);
			}
			else {
				fScriptRuns.push_back (thisCharElt);
			}
		}
	}

	copy (static_cast<FriBidiChar*> (vText), vText + fTextLength, static_cast<Led_tChar*> (fVirtualText));
}
#endif

#if		qUseICUBidi
void	TextLayoutBlock_Basic::Construct_ICU (const TextDirection* initialDirection)
{
	//TMPHACK...to start testing...
	UErrorCode	err=	U_ZERO_ERROR;
	UBiDi*		para	=	ubidi_openSized (textLength, 0, &err);
	ubidi_close (para);
	#error "NOT YET IMPLEMENTED (maybe won't be)"
}
#endif

void	TextLayoutBlock_Basic::Construct_Default ()
{
	// Only do something for UNICODE/wide-chars - otherwise - just copy the chars by value...
	copy (static_cast<Led_tChar*> (fRealText), static_cast<Led_tChar*> (fRealText) + fTextLength, static_cast<Led_tChar*> (fVirtualText));
	if (fTextLength != 0) {
		ScriptRunElt	thisCharElt;
		thisCharElt.fDirection = eLeftToRight;
		thisCharElt.fRealStart = 0;
		thisCharElt.fRealEnd = fTextLength;
		thisCharElt.fVirtualStart = 0;
		thisCharElt.fVirtualEnd = fTextLength;
		fScriptRuns.push_back (thisCharElt);
	}
}

void	TextLayoutBlock_Basic::PeekAtRealText_ (const Led_tChar** startText, const Led_tChar** endText) const
{
	Led_RequireNotNil (startText);
	Led_RequireNotNil (endText);
	*startText = fRealText;
	*endText = fRealText + fTextLength;
}

void	TextLayoutBlock_Basic::PeekAtVirtualText_ (const Led_tChar** startText, const Led_tChar** endText) const
{
	Led_RequireNotNil (startText);
	Led_RequireNotNil (endText);
	*startText = fVirtualText;
	*endText = fVirtualText + fTextLength;
}

vector<TextLayoutBlock::ScriptRunElt>	TextLayoutBlock_Basic::GetScriptRuns () const
{
	return fScriptRuns;
}







/*
 ********************************************************************************
 ***************************** TextLayoutBlock_Copy *****************************
 ********************************************************************************
 */
TextLayoutBlock_Copy::TextLayoutBlock_Copy (const TextLayoutBlock& from):
	inherited (),
	fRep ()
{
	/*
	 *	Compute size - and then copy all the data into a continguous block of RAM.
	 */
	size_t					strLength	=	from.GetTextLength ();
	vector<ScriptRunElt>	scriptRuns	=	from.GetScriptRuns ();

	// compute needed size
	size_t	neededSize	=	sizeof (BlockRep);
	neededSize += sizeof (Led_tChar) * strLength;				// fRealText
	neededSize += sizeof (Led_tChar) * strLength;				// fVirtualText
	neededSize += sizeof (ScriptRunElt) * scriptRuns.size ();	// fScriptRuns
	fRep = Led_RefCntPtr<BlockRep> (reinterpret_cast<BlockRep*> (new Byte [neededSize]));

	fRep->fTextLength = strLength;

	fRep->fRealText = reinterpret_cast<Led_tChar*> (fRep.get () + 1);
	{
		const	Led_tChar*	s	=	NULL;
		const	Led_tChar*	e	=	NULL;
		from.PeekAtRealText_ (&s, &e);
		copy (s, e, const_cast<Led_tChar*> (fRep->fRealText));
	}

	fRep->fVirtualText = fRep->fRealText + strLength;
	{
		const	Led_tChar*	s	=	NULL;
		const	Led_tChar*	e	=	NULL;
		from.PeekAtVirtualText_ (&s, &e);
		copy (s, e, const_cast<Led_tChar*> (fRep->fVirtualText));
	}

	fRep->fScriptRuns = reinterpret_cast<const ScriptRunElt*> (fRep->fVirtualText + strLength);
	fRep->fScriptRunsEnd = fRep->fScriptRuns + scriptRuns.size ();
	copy (scriptRuns.begin (), scriptRuns.end (), const_cast<ScriptRunElt*> (fRep->fScriptRuns));
}

TextLayoutBlock_Copy::TextLayoutBlock_Copy (const TextLayoutBlock_Copy& from):
	inherited (from),
	fRep (from.fRep)
{
}

void	TextLayoutBlock_Copy::PeekAtRealText_ (const Led_tChar** startText, const Led_tChar** endText) const
{
	Led_RequireNotNil (startText);
	Led_RequireNotNil (endText);
	*startText = fRep->fRealText;
	*endText = fRep->fRealText + fRep->fTextLength;
}

void	TextLayoutBlock_Copy::PeekAtVirtualText_ (const Led_tChar** startText, const Led_tChar** endText) const
{
	Led_RequireNotNil (startText);
	Led_RequireNotNil (endText);
	*startText = fRep->fVirtualText;
	*endText = fRep->fVirtualText + fRep->fTextLength;
}

vector<TextLayoutBlock_Copy::ScriptRunElt>	TextLayoutBlock_Copy::GetScriptRuns () const
{
	return vector<ScriptRunElt> (fRep->fScriptRuns, fRep->fScriptRunsEnd);
}





/*
 ********************************************************************************
 ************************ TextLayoutBlock_Copy::BlockRep ************************
 ********************************************************************************
 */
void	TextLayoutBlock_Copy::BlockRep::operator delete (void* p)
{
	// Because we allocate using new Byte[] in TextLayoutBlock_Copy::CTOR (see SPR#1596)
	delete[] (reinterpret_cast<char*> (p));
}







/*
 ********************************************************************************
 *********************** TextLayoutBlock_VirtualSubset **************************
 ********************************************************************************
 */
TextLayoutBlock_VirtualSubset::TextLayoutBlock_VirtualSubset (const TextLayoutBlock& subsetOf, size_t start, size_t end):
	inherited (),
	fSubsetOf (subsetOf),
	fStart (start),
	fEnd (end),
	fScriptRuns (),
	fRealText (end-start)
{
	vector<ScriptRunElt>	origRuns		=	fSubsetOf.GetScriptRuns ();
	size_t					offsetSoFar		=	0;
	size_t					subsetLen		=	fEnd - fStart;
	const Led_tChar*		fullRealText	=	fSubsetOf.PeekAtRealText ();
	for (vector<ScriptRunElt>::const_iterator i = origRuns.begin (); i != origRuns.end (); ++i) {
		int		offsetStart	=	(*i).fVirtualStart;
		int		offsetEnd	=	(*i).fVirtualEnd;

		int	runRelStart = max (offsetStart, static_cast<int> (fStart));
		int	runRelEnd = min (offsetEnd, static_cast<int> (fEnd));
		if (runRelStart < runRelEnd) {
			// then this is a run with some stuff in it
			ScriptRunElt	s	=	*i;
			s.fVirtualStart = runRelStart - fStart;
			s.fVirtualEnd = runRelEnd - fStart;

			/*
			 * Since the API of TextLayoutBlock assumes a contiguous piece of RAM for the REAL and Virtual text, and since
			 * the source text is no longer continguous (if we used the full buffer then its length wouldn't match the length
			 * of our virtual buffer), we must make a copy of the real text and use different adjusted offsets. This 'real' text then
			 * isn't so real after all. It might not even be in an order that would have produced the given virtual text. BUT - if one
			 * makes calls to get back the original real text for any given range of Virual text - you WILL at least get back the
			 * right text.
			 */
			size_t		runEltLen	=	runRelEnd - runRelStart;
			copy (fullRealText + s.fRealStart, fullRealText + s.fRealStart + runEltLen, fRealText + offsetSoFar);
			s.fRealStart = offsetSoFar;
			offsetSoFar += runEltLen;
			s.fRealEnd = offsetSoFar;
			fScriptRuns.push_back (s);
		}
	}

	Invariant ();
}

void	TextLayoutBlock_VirtualSubset::PeekAtRealText_ (const Led_tChar** startText, const Led_tChar** endText) const
{
	Led_RequireNotNil (startText);
	Led_RequireNotNil (endText);
	*startText = fRealText;
	*endText = *startText + (fEnd - fStart);
}

void	TextLayoutBlock_VirtualSubset::PeekAtVirtualText_ (const Led_tChar** startText, const Led_tChar** endText) const
{
	Led_RequireNotNil (startText);
	Led_RequireNotNil (endText);
	fSubsetOf.PeekAtVirtualText_ (startText, endText);
	*startText += fStart;
	Led_Assert (fStart <= fEnd);
	Led_Assert (*endText - *startText >= static_cast<int> (fEnd-fStart));	// make sure orig string >= shortened string
	*endText = *startText + (fEnd - fStart);
}

vector<TextLayoutBlock::ScriptRunElt>	TextLayoutBlock_VirtualSubset::GetScriptRuns () const
{
	return fScriptRuns;
}













/*
 ********************************************************************************
 ********** myGetInitialUNISCRIBEDir (AUTOGENERATED memoize of GetInitialUNISCRIBEDir - Jan 12 2003) ***********
 ********************************************************************************
 */
// Hack for SPR#1236
InitialUNISCRIBEDir	myGetInitialUNISCRIBEDir (wchar_t c)
{
	if (65 <= c && c <= 90) {
		return eLTRChar;
	}
	if (97 <= c && c <= 122) {
		return eLTRChar;
	}
	if (170 <= c && c <= 170) {
		return eLTRChar;
	}
	if (181 <= c && c <= 181) {
		return eLTRChar;
	}
	if (186 <= c && c <= 186) {
		return eLTRChar;
	}
	if (192 <= c && c <= 214) {
		return eLTRChar;
	}
	if (216 <= c && c <= 246) {
		return eLTRChar;
	}
	if (248 <= c && c <= 696) {
		return eLTRChar;
	}
	if (699 <= c && c <= 705) {
		return eLTRChar;
	}
	if (720 <= c && c <= 721) {
		return eLTRChar;
	}
	if (736 <= c && c <= 740) {
		return eLTRChar;
	}
	if (750 <= c && c <= 767) {
		return eLTRChar;
	}
	if (848 <= c && c <= 863) {
		return eLTRChar;
	}
	if (880 <= c && c <= 883) {
		return eLTRChar;
	}
	if (886 <= c && c <= 893) {
		return eLTRChar;
	}
	if (895 <= c && c <= 899) {
		return eLTRChar;
	}
	if (902 <= c && c <= 902) {
		return eLTRChar;
	}
	if (904 <= c && c <= 1013) {
		return eLTRChar;
	}
	if (1015 <= c && c <= 1154) {
		return eLTRChar;
	}
	if (1159 <= c && c <= 1159) {
		return eLTRChar;
	}
	if (1162 <= c && c <= 1417) {
		return eLTRChar;
	}
	if (1419 <= c && c <= 1423) {
		return eLTRChar;
	}
	if (1424 <= c && c <= 1424) {
		return eRTLChar;
	}
	if (1442 <= c && c <= 1442) {
		return eRTLChar;
	}
	if (1466 <= c && c <= 1466) {
		return eRTLChar;
	}
	if (1470 <= c && c <= 1470) {
		return eRTLChar;
	}
	if (1472 <= c && c <= 1472) {
		return eRTLChar;
	}
	if (1475 <= c && c <= 1475) {
		return eRTLChar;
	}
	if (1477 <= c && c <= 1547) {
		return eRTLChar;
	}
	if (1549 <= c && c <= 1610) {
		return eRTLChar;
	}
	if (1622 <= c && c <= 1631) {
		return eRTLChar;
	}
	if (1645 <= c && c <= 1647) {
		return eRTLChar;
	}
	if (1649 <= c && c <= 1749) {
		return eRTLChar;
	}
	if (1757 <= c && c <= 1757) {
		return eRTLChar;
	}
	if (1765 <= c && c <= 1766) {
		return eRTLChar;
	}
	if (1774 <= c && c <= 1775) {
		return eRTLChar;
	}
	if (1786 <= c && c <= 1806) {
		return eRTLChar;
	}
	if (1808 <= c && c <= 1808) {
		return eRTLChar;
	}
	if (1810 <= c && c <= 1839) {
		return eRTLChar;
	}
	if (1867 <= c && c <= 1957) {
		return eRTLChar;
	}
	if (1969 <= c && c <= 1983) {
		return eRTLChar;
	}
	if (1984 <= c && c <= 2304) {
		return eLTRChar;
	}
	if (2307 <= c && c <= 2363) {
		return eLTRChar;
	}
	if (2365 <= c && c <= 2368) {
		return eLTRChar;
	}
	if (2377 <= c && c <= 2380) {
		return eLTRChar;
	}
	if (2382 <= c && c <= 2384) {
		return eLTRChar;
	}
	if (2389 <= c && c <= 2401) {
		return eLTRChar;
	}
	if (2404 <= c && c <= 2432) {
		return eLTRChar;
	}
	if (2434 <= c && c <= 2491) {
		return eLTRChar;
	}
	if (2493 <= c && c <= 2496) {
		return eLTRChar;
	}
	if (2501 <= c && c <= 2508) {
		return eLTRChar;
	}
	if (2510 <= c && c <= 2529) {
		return eLTRChar;
	}
	if (2532 <= c && c <= 2545) {
		return eLTRChar;
	}
	if (2548 <= c && c <= 2561) {
		return eLTRChar;
	}
	if (2563 <= c && c <= 2619) {
		return eLTRChar;
	}
	if (2621 <= c && c <= 2624) {
		return eLTRChar;
	}
	if (2627 <= c && c <= 2630) {
		return eLTRChar;
	}
	if (2633 <= c && c <= 2634) {
		return eLTRChar;
	}
	if (2638 <= c && c <= 2671) {
		return eLTRChar;
	}
	if (2674 <= c && c <= 2688) {
		return eLTRChar;
	}
	if (2691 <= c && c <= 2747) {
		return eLTRChar;
	}
	if (2749 <= c && c <= 2752) {
		return eLTRChar;
	}
	if (2758 <= c && c <= 2758) {
		return eLTRChar;
	}
	if (2761 <= c && c <= 2764) {
		return eLTRChar;
	}
	if (2766 <= c && c <= 2816) {
		return eLTRChar;
	}
	if (2818 <= c && c <= 2875) {
		return eLTRChar;
	}
	if (2877 <= c && c <= 2878) {
		return eLTRChar;
	}
	if (2880 <= c && c <= 2880) {
		return eLTRChar;
	}
	if (2884 <= c && c <= 2892) {
		return eLTRChar;
	}
	if (2894 <= c && c <= 2901) {
		return eLTRChar;
	}
	if (2903 <= c && c <= 2945) {
		return eLTRChar;
	}
	if (2947 <= c && c <= 3007) {
		return eLTRChar;
	}
	if (3009 <= c && c <= 3020) {
		return eLTRChar;
	}
	if (3022 <= c && c <= 3133) {
		return eLTRChar;
	}
	if (3137 <= c && c <= 3141) {
		return eLTRChar;
	}
	if (3145 <= c && c <= 3145) {
		return eLTRChar;
	}
	if (3150 <= c && c <= 3156) {
		return eLTRChar;
	}
	if (3159 <= c && c <= 3262) {
		return eLTRChar;
	}
	if (3264 <= c && c <= 3269) {
		return eLTRChar;
	}
	if (3271 <= c && c <= 3275) {
		return eLTRChar;
	}
	if (3278 <= c && c <= 3392) {
		return eLTRChar;
	}
	if (3396 <= c && c <= 3404) {
		return eLTRChar;
	}
	if (3406 <= c && c <= 3529) {
		return eLTRChar;
	}
	if (3531 <= c && c <= 3537) {
		return eLTRChar;
	}
	if (3541 <= c && c <= 3541) {
		return eLTRChar;
	}
	if (3543 <= c && c <= 3632) {
		return eLTRChar;
	}
	if (3634 <= c && c <= 3635) {
		return eLTRChar;
	}
	if (3643 <= c && c <= 3646) {
		return eLTRChar;
	}
	if (3648 <= c && c <= 3654) {
		return eLTRChar;
	}
	if (3663 <= c && c <= 3760) {
		return eLTRChar;
	}
	if (3762 <= c && c <= 3763) {
		return eLTRChar;
	}
	if (3770 <= c && c <= 3770) {
		return eLTRChar;
	}
	if (3773 <= c && c <= 3783) {
		return eLTRChar;
	}
	if (3790 <= c && c <= 3863) {
		return eLTRChar;
	}
	if (3866 <= c && c <= 3892) {
		return eLTRChar;
	}
	if (3894 <= c && c <= 3894) {
		return eLTRChar;
	}
	if (3896 <= c && c <= 3896) {
		return eLTRChar;
	}
	if (3902 <= c && c <= 3952) {
		return eLTRChar;
	}
	if (3967 <= c && c <= 3967) {
		return eLTRChar;
	}
	if (3973 <= c && c <= 3973) {
		return eLTRChar;
	}
	if (3976 <= c && c <= 3983) {
		return eLTRChar;
	}
	if (3992 <= c && c <= 3992) {
		return eLTRChar;
	}
	if (4029 <= c && c <= 4037) {
		return eLTRChar;
	}
	if (4039 <= c && c <= 4140) {
		return eLTRChar;
	}
	if (4145 <= c && c <= 4145) {
		return eLTRChar;
	}
	if (4147 <= c && c <= 4149) {
		return eLTRChar;
	}
	if (4152 <= c && c <= 4152) {
		return eLTRChar;
	}
	if (4154 <= c && c <= 4183) {
		return eLTRChar;
	}
	if (4186 <= c && c <= 5759) {
		return eLTRChar;
	}
	if (5761 <= c && c <= 5786) {
		return eLTRChar;
	}
	if (5789 <= c && c <= 5905) {
		return eLTRChar;
	}
	if (5909 <= c && c <= 5937) {
		return eLTRChar;
	}
	if (5941 <= c && c <= 5969) {
		return eLTRChar;
	}
	if (5972 <= c && c <= 6001) {
		return eLTRChar;
	}
	if (6004 <= c && c <= 6070) {
		return eLTRChar;
	}
	if (6078 <= c && c <= 6085) {
		return eLTRChar;
	}
	if (6087 <= c && c <= 6088) {
		return eLTRChar;
	}
	if (6100 <= c && c <= 6106) {
		return eLTRChar;
	}
	if (6108 <= c && c <= 6143) {
		return eLTRChar;
	}
	if (6159 <= c && c <= 6312) {
		return eLTRChar;
	}
	if (6314 <= c && c <= 8124) {
		return eLTRChar;
	}
	if (8126 <= c && c <= 8126) {
		return eLTRChar;
	}
	if (8130 <= c && c <= 8140) {
		return eLTRChar;
	}
	if (8144 <= c && c <= 8156) {
		return eLTRChar;
	}
	if (8160 <= c && c <= 8172) {
		return eLTRChar;
	}
	if (8176 <= c && c <= 8188) {
		return eLTRChar;
	}
	if (8191 <= c && c <= 8191) {
		return eLTRChar;
	}
	if (8206 <= c && c <= 8206) {
		return eLTRChar;
	}
	if (8207 <= c && c <= 8207) {
		return eRTLChar;
	}
	if (8275 <= c && c <= 8278) {
		return eLTRChar;
	}
	if (8280 <= c && c <= 8286) {
		return eLTRChar;
	}
	if (8292 <= c && c <= 8297) {
		return eLTRChar;
	}
	if (8305 <= c && c <= 8307) {
		return eLTRChar;
	}
	if (8319 <= c && c <= 8319) {
		return eLTRChar;
	}
	if (8335 <= c && c <= 8351) {
		return eLTRChar;
	}
	if (8370 <= c && c <= 8399) {
		return eLTRChar;
	}
	if (8427 <= c && c <= 8447) {
		return eLTRChar;
	}
	if (8450 <= c && c <= 8450) {
		return eLTRChar;
	}
	if (8455 <= c && c <= 8455) {
		return eLTRChar;
	}
	if (8458 <= c && c <= 8467) {
		return eLTRChar;
	}
	if (8469 <= c && c <= 8469) {
		return eLTRChar;
	}
	if (8473 <= c && c <= 8477) {
		return eLTRChar;
	}
	if (8484 <= c && c <= 8484) {
		return eLTRChar;
	}
	if (8486 <= c && c <= 8486) {
		return eLTRChar;
	}
	if (8488 <= c && c <= 8488) {
		return eLTRChar;
	}
	if (8490 <= c && c <= 8493) {
		return eLTRChar;
	}
	if (8495 <= c && c <= 8497) {
		return eLTRChar;
	}
	if (8499 <= c && c <= 8505) {
		return eLTRChar;
	}
	if (8507 <= c && c <= 8511) {
		return eLTRChar;
	}
	if (8517 <= c && c <= 8521) {
		return eLTRChar;
	}
	if (8524 <= c && c <= 8530) {
		return eLTRChar;
	}
	if (8544 <= c && c <= 8591) {
		return eLTRChar;
	}
	if (9014 <= c && c <= 9082) {
		return eLTRChar;
	}
	if (9109 <= c && c <= 9109) {
		return eLTRChar;
	}
	if (9167 <= c && c <= 9215) {
		return eLTRChar;
	}
	if (9255 <= c && c <= 9279) {
		return eLTRChar;
	}
	if (9291 <= c && c <= 9311) {
		return eLTRChar;
	}
	if (9372 <= c && c <= 9449) {
		return eLTRChar;
	}
	if (9471 <= c && c <= 9471) {
		return eLTRChar;
	}
	if (9748 <= c && c <= 9749) {
		return eLTRChar;
	}
	if (9752 <= c && c <= 9752) {
		return eLTRChar;
	}
	if (9854 <= c && c <= 9855) {
		return eLTRChar;
	}
	if (9866 <= c && c <= 9984) {
		return eLTRChar;
	}
	if (9989 <= c && c <= 9989) {
		return eLTRChar;
	}
	if (9994 <= c && c <= 9995) {
		return eLTRChar;
	}
	if (10024 <= c && c <= 10024) {
		return eLTRChar;
	}
	if (10060 <= c && c <= 10060) {
		return eLTRChar;
	}
	if (10062 <= c && c <= 10062) {
		return eLTRChar;
	}
	if (10067 <= c && c <= 10069) {
		return eLTRChar;
	}
	if (10071 <= c && c <= 10071) {
		return eLTRChar;
	}
	if (10079 <= c && c <= 10080) {
		return eLTRChar;
	}
	if (10133 <= c && c <= 10135) {
		return eLTRChar;
	}
	if (10160 <= c && c <= 10160) {
		return eLTRChar;
	}
	if (10175 <= c && c <= 10191) {
		return eLTRChar;
	}
	if (10220 <= c && c <= 10223) {
		return eLTRChar;
	}
	if (11008 <= c && c <= 11903) {
		return eLTRChar;
	}
	if (11930 <= c && c <= 11930) {
		return eLTRChar;
	}
	if (12020 <= c && c <= 12031) {
		return eLTRChar;
	}
	if (12246 <= c && c <= 12271) {
		return eLTRChar;
	}
	if (12284 <= c && c <= 12287) {
		return eLTRChar;
	}
	if (12293 <= c && c <= 12295) {
		return eLTRChar;
	}
	if (12321 <= c && c <= 12329) {
		return eLTRChar;
	}
	if (12337 <= c && c <= 12341) {
		return eLTRChar;
	}
	if (12344 <= c && c <= 12348) {
		return eLTRChar;
	}
	if (12352 <= c && c <= 12440) {
		return eLTRChar;
	}
	if (12445 <= c && c <= 12447) {
		return eLTRChar;
	}
	if (12449 <= c && c <= 12538) {
		return eLTRChar;
	}
	if (12540 <= c && c <= 12880) {
		return eLTRChar;
	}
	if (12896 <= c && c <= 12976) {
		return eLTRChar;
	}
	if (12992 <= c && c <= 42127) {
		return eLTRChar;
	}
	if (42183 <= c && c <= 64284) {
		return eLTRChar;
	}
	if (64285 <= c && c <= 64285) {
		return eRTLChar;
	}
	if (64287 <= c && c <= 64296) {
		return eRTLChar;
	}
	if (64298 <= c && c <= 64829) {
		return eRTLChar;
	}
	if (64832 <= c && c <= 65023) {
		return eRTLChar;
	}
	if (65040 <= c && c <= 65055) {
		return eLTRChar;
	}
	if (65060 <= c && c <= 65071) {
		return eLTRChar;
	}
	if (65095 <= c && c <= 65096) {
		return eLTRChar;
	}
	if (65107 <= c && c <= 65107) {
		return eLTRChar;
	}
	if (65127 <= c && c <= 65127) {
		return eLTRChar;
	}
	if (65132 <= c && c <= 65135) {
		return eLTRChar;
	}
	if (65136 <= c && c <= 65278) {
		return eRTLChar;
	}
	if (65280 <= c && c <= 65280) {
		return eLTRChar;
	}
	if (65313 <= c && c <= 65338) {
		return eLTRChar;
	}
	if (65345 <= c && c <= 65370) {
		return eLTRChar;
	}
	if (65382 <= c && c <= 65503) {
		return eLTRChar;
	}
	if (65511 <= c && c <= 65511) {
		return eLTRChar;
	}
	if (65519 <= c && c <= 65528) {
		return eLTRChar;
	}
	if (65534 <= c && c <= 65534) {
		return eLTRChar;
	}
	return eNeutralChar;
}




#if		qLedUsesNamespaces
}
#endif






// For gnuemacs:
// Local Variables: ***
// mode:c++ ***
// tab-width:4 ***
// End: ***
