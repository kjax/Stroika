/*
 * Copyright(c) Sophist Solutions, Inc. 1990-2011.  All rights reserved
 */
#ifndef	_Stroika_Foundation_Configuration_Private_Defaults_Execution_Exceptions_h_
#define	_Stroika_Foundation_Configuration_Private_Defaults_Execution_Exceptions_h_	1

	/*
	 * This file is a private Stroika implementation detail, and shouldn't be included directly. 
	 * Its used by StroikaConfig.h - optionally included there -
	 * to help implement the set of Stroika public configuration defines.
	 */




	/*
	 * See DOCS in Stroika/Foundation/Execution/Thread.h
	 */
	#if		!defined (qUseThreads_WindowsNative)
		#if		qCompilerAndStdLib_Supports_threads
			// On Windows - its quite unclear if one is better off using the stdc++ implementation (since it doesnt yet exist its hard to tell what features it
			// will have) or the stdc++ implemeentation. Luckily - there is only one for unix & one for windoze now, so its clear in the very short term...
			//		--LGP 2011-09-01
			//#define	qUseThreads_WindowsNative	qPlatform_Windows
			#define	qUseThreads_WindowsNative	0
		#else
			#define	qUseThreads_WindowsNative	qPlatform_Windows
		#endif
	#endif
	#if		!defined (qUseThreads_StdCPlusPlus)
		#if		qCompilerAndStdLib_Supports_threads
			#define	qUseThreads_StdCPlusPlus	!qUseThreads_WindowsNative
		#else
			#define	qUseThreads_StdCPlusPlus	0
		#endif
	#endif

	/*
	 * See DOCS in Stroika/Foundation/Execution/Thread.h
	 */
	#if		!defined (qUseThreads)
		#define	qUseThreads	(qUseThreads_WindowsNative || qUseThreads_StdCPlusPlus)
	#endif


#endif	/*_Stroika_Foundation_Configuration_Private_Defaults_Execution_Exceptions_h_*/