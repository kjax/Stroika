#!/usr/bin/perl 
require "../../../ScriptsLib/SimpleRun.pl";

local $testNum = "7";
local $testName = "Date/Time";

local $date = `sh -c date`;
chomp ($date);
print "Starting regression test #[$testNum] $testName Test: $date ------------------------------\r\n";

DoRunSimpleTestArgv ($ARGV[0], "[$testNum] $testName Test", "../../../../Builds/Platform_Linux/", "Test7");

local $date = `sh -c date`;
chomp ($date);
print "End of Regression Test #[$testNum] $testName Test: $date -------------------------------\r\n";
