#!/usr/bin/perl
#
# Recursively finds and replaces text over the entire project directory,
# confirming each change.
#
use strict;
use warnings;
use File::Find;
use File::Slurp;
use FindBin;
use Cwd 'abs_path';

#Text colour codes:
my $red = "\33[41m\33[37m";
my $green = "\33[42m\33[37m";
my $white = "\33[47m\33[30m";
my $reset = "\33[0m";

sub printRed
{
    print("$red$_[0]$reset");
}
sub printGreen
{
    print("$green$_[0]$reset");
}

sub printWhite
{
    print("$white$_[0]$reset");
}


sub rewindLines
{
    my $numLines = shift;
    print("\r\033[K");
    for (my $y = 0; $y < $numLines; $y++)
    {
        print("\33[1A\r\033[K");
    }
}

chdir($FindBin::Bin);
my $projectDir = "../";

my $toFind, my $replace, my $fileApprove, my $ignorePattern;
my $patternFlag = 0;
foreach my $arg (@ARGV)
{
    if ($arg =~ /^-*h (elp)?$/i)
    {
        print("Usage:\n");
        print("./project_scripts/ReplaceAll.pl [flags] \"pattern\" "
                ."\"replacement\"\n");
        print("\t-h/-H/--help/etc: Print this help text and exit.\n");
        print("\t-a: Approve replacements by file instead of line.\n");
        print("\t-i \"pattern\": Ignore files with names that match this"
                ." pattern.\n");
        exit(0);
    }
    if ($patternFlag)
    {
        $ignorePattern = $arg;
        $patternFlag = 0;
        next;
    }
    if ($arg eq "-a")
    {
        $fileApprove = 1;
    }
    elsif ($arg eq "-i")
    {
        $patternFlag = 1;
    }
    elsif (!defined($toFind))
    {
        $toFind = $arg;
    }
    elsif (!defined($replace))
    {
        $replace = $arg;
    }
    else
    {
        die "Unexpected argument $arg\n";
    }
}


#Subdirectories to exclude:
my @excluded = (
    "../build",
    "../debian",
    "../deps",
    "../docs/Doxygen",
    "../JuceLibraryCode",
    "../pack-debian",
    "../.git"
    );

if (!defined($toFind) || !defined($replace))
{
    die("Missing arguments!\n");
}

print("Replacing ");
printRed($toFind);
print(" with ");
printGreen($replace);
print(".\n");

sub readFiles
{
    if (-d) # Stop recursion from continuing under excluded directories.
    {
        my $file = $_;
        foreach my $dir (@excluded)
        {
            if ($File::Find::name =~ /$dir/)
            {
                $File::Find::prune = 1;
                return;
            }
        }
    }
    if (-f)
    {
        my $file = $_;
        if ($ignorePattern && ($file =~ /$ignorePattern/))
        {
            return;
        }
        my $text= read_file($file);
        my @matches = ($text =~ /$toFind/gs);
        my $matchCount = @matches;
        my $approveAll = 0;
        if ($matchCount > 0)
        {
            print("Found $matchCount ".($matchCount > 1 ? "matches" : "match")
                    ." in ");
            printGreen($File::Find::name);
            if ($fileApprove)
            {
                print(". Update file?:(");
                printGreen('y');
                print("/");
                printRed('n');
                print("):");
                my $input = "";
                while ( ($input ne 'y') && ($input ne 'n'))
                {
                    chomp($input = <STDIN>);
                }
                if ($input eq 'y')
                {
                    $approveAll = 1;
                }
                else
                {
                    next;
                }
            }
            print(":\n");
            my $changesMade = 0;
            my $outText = "";
            my $input = "";
            while ($text =~ /^(.*?)($toFind)(.*)$/s)
            {
                my $preMatch = $1;
                my $match = $2;
                my $postMatch = $3;
                if (!$approveAll)
                {
                    print("Found match:\n");
                    my $prePrint = $preMatch;
                    if ($prePrint =~ /\v (.*?)$/)
                    {
                        $prePrint = $1;
                    }
                    my $postPrint = $postMatch;
                    if ($postPrint =~ /^(.*?)\v/)
                    {
                        $postPrint = $1
                    }
                    my $preLength = length($prePrint);
                    my $matchLength = length($match);
                    my $postLength = length($postPrint);
                    my $maxLength = 80;
                    if ( ($preLength + $matchLength + $postLength) > $maxLength)
                    {
                        if ($matchLength > $maxLength)
                        {
                            $prePrint = "";
                            $postPrint = "";
                        }
                        elsif ( ( ($preLength + $matchLength) > $maxLength)
                                && ( ($postLength + $matchLength) > $maxLength))
                        {
                            my $contextLength = ($maxLength - $matchLength) / 2;
                            $prePrint = substr($prePrint, -$contextLength);
                            $postPrint = substr($postPrint, 0, $contextLength);
                        }
                        elsif ($preLength > $postLength)
                        {
                            $prePrint = substr($prePrint,
                                    - ($maxLength - $matchLength -$postLength));
                        }
                        else
                        {
                            $postPrint = substr($postPrint, 0,
                                    ($maxLength - $matchLength - $preLength));
                        }
                    }


                    print($prePrint);
                    printRed($match);
                    print("$postPrint\n");

                    print($prePrint);
                    printGreen("$replace");
                    print("$postPrint\n");

                    print("Change match? (");
                    printGreen("y"); print ("es/");
                    printRed("n");   print ("o/");
                    printWhite("s"); print ("kip file/");
                    printGreen("c"); print ("hange all in file/");
                    printRed("q");   print ("uit):");
                    chomp($input = <STDIN>);
                }
                else
                {
                    $input = 'y';
                }


                if ($input eq "c")
                {
                    $approveAll = 1;
                    $input = 'y';
                }
                if ($input eq "y")
                {
                    $changesMade++;
                    $outText = $outText.$preMatch.$replace;
                }
                elsif ($input eq "n")
                {
                    $outText = $outText.$preMatch.$match;
                }
                elsif ($input eq "s")
                {
                    rewindLines(3);
                    last;
                }
                elsif ($input eq "q")
                {
                    rewindLines(5);
                    exit;
                }
                else
                {
                    print("Invalid option $input selected.\n");
                }
                $text = $postMatch;
                rewindLines(5);
            }
            if ($changesMade)
            {
                print("Writing $changesMade changes back to $file.\n");
                my $fileText = "$outText$text";
                length($fileText) > 0 or die "Empty output!\n";
                write_file($file, $fileText);
            }
        }
    }
}
find(\&readFiles,$projectDir);
