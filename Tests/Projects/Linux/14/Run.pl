#!/usr/bin/perl
require "../../../ScriptsLib/SimpleRun.pl";

local $testNum = "14";
local $testName = "Foundation::IO::Networking";

local $date = `sh -c date`;
chomp ($date);
print "Starting regression test #[$testNum] $testName Test: $date ------------------------------\r\n";

DoRunSimpleTestArgv ($ARGV[0], "[$testNum] $testName Test", "../../../../Builds/Platform_Linux/", "Test$testNum");

local $date = `sh -c date`;
chomp ($date);
print "End of Regression Test #[$testNum] $testName Test: $date -------------------------------\r\n";
