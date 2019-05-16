#!/usr/bin/perl
#
# Generates empty unit test classes for project source files.
#
# Use: ./TestGen.pl "path/in/Source/dir/to/file.cpp" "Test Category Name"

use strict;
use warnings;
use File::Slurp;
use lib './project-scripts';
use Paths;

my $projectRoot = Paths::PROJECT_DIR;

my $path = $ARGV[0] or die "Missing source file path!\n";
my $category = $ARGV[1] or die "Missing test category!\n";
my $className;
if ($path =~ /\/(.+)\.cpp$/)
{
    $className = $1;
}
else
{
    die "Failed to parse classname from file path $path\n";
}
my $testClassName = $className."Test";
my $header = $path;
$header =~ s/cpp/h/;

my $testFile = qq
[#include "JuceHeader.h"
#include "$header"

class $testClassName : public juce::UnitTest
{
public:
    $testClassName() : juce::UnitTest("$category\::$className Testing",
            "$category") {}

    void runTest() override
    {
    }
};

static $testClassName test;
];

my $subDir = $path;
$subDir =~ s/^.*Source\///;
my $testPath =  "$projectRoot/Tests/$subDir";
write_file($testPath, $testFile) or die "Failed to write to $testPath\n";
