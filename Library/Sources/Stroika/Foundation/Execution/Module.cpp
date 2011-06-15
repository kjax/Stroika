/*
 * Copyright(c) Records For Living, Inc. 2004-2011.  All rights reserved
 */
#include	"../StroikaPreComp.h"

#include	"../IO/FileUtils.h"

#include	"Module.h"

using	namespace	Stroika::Foundation;
using	namespace	Stroika::Foundation::Execution;


tstring	Execution::GetEXEDir ()
{

	//MOVE ROUTINE GETEXEDIR () here from IO
	return IO::GetEXEDir ();
}

tstring	Execution::GetEXEPath ()
{
	//MOVE ROUTINE GetEXEPath () here from IO
	return IO::GetEXEPath ();
}
