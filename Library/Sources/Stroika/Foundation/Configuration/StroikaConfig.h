/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Configuration_StroikaConfig_h_
#define	_Stroika_Foundation_Configuration_StroikaConfig_h_	1

/*
 *	Since this include file CAN define #defines which are required before including system includes, its required
 *	that THIS file be included before all others (like a precomp.h precompiled header file).
 *
 *	This is the ONLY file this is true of. Otherwise, we generally include system files before other Stroika files etc.
 *
 *
 * Used to declare these in vcproj file automatically, but better to declare here to assure
 * synchonized among all projects that include the Stroika project (and for cross-platform makefiles etc)
 */



/*
 * At some point - we will probably want to have THIS file autogenerated from a CONFIGURE (like gcc configure) script, and have it
 * #include the appropriate base/default files, and provide any appropriate overrides. But for now - this is good enuf.
 *
 *		-- LGP 2011-06-08
 */


#include	"Private/Defaults_Configuration_Common_.h"

#include	"Private/Defaults_Characters_TChar_.h"
#include	"Private/Defaults_Execution_Assertions_.h"



////CREATE SEP FILES FOR MAJOR DEFINE





/*
@CONFIGVAR:		qKeepTimedCacheStats
@DESCRIPTION:	<p>Defines whether or not we capture statistics (for debugging purposes) in @'TimedCache<KEY,RESULT>'.
			This should be ON by default - iff @'qDebug' is true.</p>
 */
#ifndef	qKeepTimedCacheStats
#define	qKeepTimedCacheStats		qDebug
#endif





/*
@CONFIGVAR:		qKeepLRUCacheStats
@DESCRIPTION:	<p>Defines whether or not we capture statistics (for debugging purposes) in @'LRUCache<ELEMENT>'.
			This should be ON by default - iff @'qDebug' is true.</p>
 */
#ifndef	qKeepLRUCacheStats
#define	qKeepLRUCacheStats		qDebug
#endif






// BELOW DEFINES NEED TOGO IN ANOTHER FILE - LIKE STROIKACONFIG_qDebug


	/*
	 * SEE ALSO Stroika/Foundation/Debug/Trace.h
	 *
		*	So we can distribute near-release versions of HF to users, and get feedback - a trace file
		*	to help debug problems we cannot reproduce.
		*/
	#ifndef	qTraceToFile
		#define	qTraceToFile	0
	#endif



	/*
	 * SEE ALSO Stroika/Foundation/Debug/Trace.h
	 *
	@CONFIGVAR:		qDefaultTracingOn
	@DESCRIPTION:	<p>Led contains a limited amount of pre-built tracing code. This could be expanded someday,
				depending on how useful people find it. This defaults to being on only for Windows and if @'qDebug' is
				on (windows only cuz thats the only place I've implemented the trace message emitter so far).</p>
					<p>See also @'qDebug' and @'DebugTrace'</p>
		*/
	#ifndef	qDefaultTracingOn
		#if		qDebug
			#define	qDefaultTracingOn	1
		#else
			#define	qDefaultTracingOn	0
		#endif
	#endif



	/*
	 * SEE ALSO Stroika/Foundation/Memory/BlockAllocated.h
	 *
	@CONFIGVAR:		qAllowBlockAllocation
	@DESCRIPTION:	<p>Allow use of block-allocation. The main reason to
		disable it indescriminantly is for debugging purposes (looking for
		memory leaks). But others may have other reasons.</p>
			<p>Defaults to true.</p>
		*/
	//#define	qAllowBlockAllocation						0
	//#define	qAllowBlockAllocation						1
	#ifndef	qAllowBlockAllocation
		#define	qAllowBlockAllocation						1
	#endif



#endif	/*_Stroika_Foundation_Configuration_StroikaConfig_h_*/
