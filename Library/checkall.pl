#!/usr/bin/perl

print ("Checking Library...\n");
if ("$^O" eq "linux") {
	system ("cd Projects/Linux; perl checkall.pl");
}
else {
	system ("cd Projects/VisualStudio.Net-2010; perl checkall.pl");
}