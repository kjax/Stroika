#!/usr/bin/perl 

my $BLD_TRG = $ARGV[0];
if ($BLD_TRG eq '') {
	$BLD_TRG = 'Build';
}

system ("cd Projects/VisualStudio.Net-2010; perl buildall.pl $BLD_TRG");