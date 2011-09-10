#!/usr/bin/perl

my $DBG_LIB;
my $REL_LIB;

if ("$^O" eq "linux") {
#for now seems to build just one lib...
	$REL_LIB = "CURRENT/src/.libs/libxerces-c.a";
	$DBG_LIB = $REL_LIB;
}
else {
	$REL_LIB = "CURRENT/Build/Win32/VC10/Static Release/xerces-c_static_3.lib";
	$DBG_LIB = "CURRENT/Build/Win32/VC10/Static Debug/xerces-c_static_3D.lib";
}

if (! (-e "$REL_LIB")) {
	print ("Echo [FAILED] - $REL_LIB is missing\n");
	exit (1);
}
if (! (-e "$DBG_LIB")) {
	print ("Echo [FAILED] - $DBG_LIB is missing\n");
	exit (1);
}
print ("[SUCCEEDED]\n");