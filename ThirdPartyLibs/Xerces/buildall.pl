#!/usr/bin/perl 

my $BLD_TRG = $ARGV[0];
if ($BLD_TRG eq '') {
	$BLD_TRG = 'Build';
}


my $BASENAME	=	"xerces-c-3.1.1";

my $EXTRACTED_DIRNAME	=	$BASENAME;
my $trgDirName	=			$BASENAME;
my $SLINKDIRNAME	=		$BASENAME;

# DoCreateSymLink - this isn't currently used, and doesn't work well with windows (sets sys file attribute which casues
# DOS delete file issues) - and slink doesnt really work except in cygwin tools
my $DoCreateSymLink = 0;

if ("$^O" eq "linux") {
	$DoCreateSymLink = 1;
}


print (">>>>>>>>******************** STARTING ThirdPartyLibs/Xerces ******************\n");

if ((lc ("$BLD_TRG") eq "clean") || (lc ("$BLD_TRG") eq "clobber")) {
	system ("rm -rf $trgDirName CURRENT");
	exit (0);
}
if (lc ("$BLD_TRG") eq "rebuild") {
	system ("rm -rf $trgDirName CURRENT");
}

if (-e "CURRENT/src/xercesc/dom/impl/DOMLocatorImpl.hpp") {
	print ("already up to date\n");
	goto DONE;
	exit (0);
}

print ("Extracting Xerces...\n");

system ("rm -rf $trgDirName CURRENT");
system ("tar xf Origs/$BASENAME.tar.gz 2> /dev/null");
sleep(1);  # hack cuz sometimes it appears command not fully done writing - and we get sporadic failures on next stop on win7
system ("mv $EXTRACTED_DIRNAME CURRENT");
sleep(1);  # hack cuz sometimes it appears command not fully done writing - and we get sporadic failures on next stop on win7
if ($DoCreateSymLink) {
	system ("ln -s CURRENT $SLINKDIRNAME");
}

print ("Patching Xerces...\n");
system ("patch -t CURRENT/projects/Win32/VC10/xerces-all/XercesLib/XercesLib.vcxproj Patches/XercesLib.vcxproj.PATCH");


sub RunAndPrint
{
	my $cmd2Run = $_[0];
	print ("$cmd2Run...\n");
	my $result = system ($cmd2Run);
	if ($result != 0) {
		print "Run result = $result\r\n";
	}
}


if ("$^O" eq "linux") {
	print ("Configurating Xerces...\n");
	#NB: we disable ICO and CURL because these gave some problem with Gentoo (link error), and
	#	not worth tracking down further cuz I don't think we need either -- LGP 2011-09-27
	system ("cd CURRENT ; ./configure --enable-static --disable-shared --without-icu --without-curl");
}

print ("Building Xerces...\n");
if ("$^O" eq "linux") {
	system ("cd CURRENT ; make -s all");
}
else {
	my $EXTRA_MSBUILD_ARGS = "/nologo /v:quiet /clp:Summary";
	require "../../Library/Projects/VisualStudio.Net-2010/SetupBuildCommonVars.pl";
	
	chdir ("CURRENT/Projects/Win32/VC10/xerces-all/XercesLib");
		RunAndPrint ("MSBuild.exe $EXTRA_MSBUILD_ARGS XercesLib.vcxproj /p:Configuration=\"Static Debug\",Platform=Win32 /target:$BLD_TRG");
		RunAndPrint ("MSBuild.exe $EXTRA_MSBUILD_ARGS XercesLib.vcxproj /p:Configuration=\"Static Release\",Platform=Win32 /target:$BLD_TRG");
		RunAndPrint ("MSBuild.exe $EXTRA_MSBUILD_ARGS XercesLib.vcxproj /p:Configuration=\"Static Debug\",Platform=x64 /target:$BLD_TRG");
		RunAndPrint ("MSBuild.exe $EXTRA_MSBUILD_ARGS XercesLib.vcxproj /p:Configuration=\"Static Release\",Platform=x64 /target:$BLD_TRG");
	chdir ("../../../../../../");

	# cleaning needless objs (leave libs)
	RunAndPrint ("rm -rf 'CURRENT/Build/Win32/VC10/Static Debug/obj/'");
	RunAndPrint ("rm -rf 'CURRENT/Build/Win32/VC10/Static Release/obj/'");
	RunAndPrint ("rm -rf 'CURRENT/Build/Win64/VC10/Static Debug/obj/'");
}	RunAndPrint ("rm -rf 'CURRENT/Build/Win64/VC10/Static Release/obj/'");


system ("perl checkall.pl");

DONE:
print (">>>>>>>>******************** ENDING ThirdPartyLibs/Xerces ******************\n");

