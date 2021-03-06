#!/usr/bin/perl

my $DBG32_LIB;
my $REL32_LIB;
my $DBG64_LIB;
my $REL64_LIB;

if ("$^O" eq "linux") {
	#
	#for now seems to build just one lib...
	#
	$REL32_LIB = "CURRENT/src/.libs/libxerces-c.a";
	$DBG32_LIB = $REL32_LIB;
	$DBG64_LIB = $REL32_LIB;
	$REL64_LIB = $REL32_LIB;
}
else {
	$REL32_LIB = "CURRENT/Build/Win32/VC10/Static Release/xerces-c_static_3.lib";
	$DBG32_LIB = "CURRENT/Build/Win32/VC10/Static Debug/xerces-c_static_3D.lib";
	$REL64_LIB = "CURRENT/Build/Win64/VC10/Static Release/xerces-c_static_3.lib";
	$DBG64_LIB = "CURRENT/Build/Win64/VC10/Static Debug/xerces-c_static_3D.lib";
}

if (! (-e "$REL32_LIB")) {
	print ("Echo [FAILED] - $REL32_LIB is missing\n");
	exit (1);
}
if (! (-e "$DBG32_LIB")) {
	print ("Echo [FAILED] - $DBG32_LIB is missing\n");
	exit (1);
}
if (! (-e "$REL64_LIB")) {
	print ("Echo [FAILED] - $REL64_LIB is missing\n");
	exit (1);
}
if (! (-e "$DBG64_LIB")) {
	print ("Echo [FAILED] - $DBG64_LIB is missing\n");
	exit (1);
}
print ("[SUCCEEDED]\n");
