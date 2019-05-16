#!/usr/bin/perl
#
# Finds all .cpp and .h project files, and lists them alphabetically by name,
# removing file extensions and duplicates.
#
use strict;
use warnings;
use File::Find;
use File::Slurp;
use lib './project-scripts';
use Paths;

my $projectDir = Paths::PROJECT_DIR;
my $sourceDir = "$projectDir/Source";
my $testDir = "$projectDir/Tests";
my %names;

my ($dirName) = ($projectDir =~ /.*\/(.*?$)/s);

sub findModules
{
    my $name = $File::Find::name;
    if ($name =~ /^.+\.(h|cpp)$/)
    {
        $name =~ s/\..*//;
        $name =~ s/.*$dirName//g;

        $names{$name} = 1;
    }
}
find(\&findModules, $sourceDir);
find(\&findModules, $testDir);

my @names = sort( keys(%names));

foreach my $name (@names)
{
    print("$name\n");
}
