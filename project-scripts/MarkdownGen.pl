#!/usr/bin/perl
#
# Generates markdown files with links to all header files within a directory.
#
use strict;
use warnings;
use File::Find;
use File::Slurp;
use lib './project-scripts';
use Paths;

my $projectDir = Paths::PROJECT_DIR;

if ( (scalar @ARGV) < 2)
{
    die("Usage: ./MarkdownGen.pl \"Path/To/Module/Dir\""
            ." \"MarkdownOutput.md\"\n");
}
my $moduleDir = $ARGV[0];
my $markdownFile = $ARGV[1];
my $moduleName = substr($moduleDir, rindex($moduleDir, '/') + 1);

my $markdownText = "# $moduleName Module Documentation\n\n";

sub findFiles
{
    my $fullPath = $File::Find::name;
    if ($fullPath =~ /^.+\.h$/)
    {
        my $relativePath = "../../".$fullPath;
        my $name = $_;
        $name = substr($name, 0, length($name) - 2);
        $name =~ s/_/::/g;
        $markdownText = $markdownText."#### [$name]($relativePath)\n\n";
    }
}
find(\&findFiles, $moduleDir);
write_file($markdownFile, $markdownText);
