#!/usr/bin/perl
# Finds all matches for a regular expression in each project file, and offers
# to open vim at each match for editing.
use strict;
use warnings;
use File::Find;
use File::Slurp;
use lib './project-scripts';
use Paths;

my $expression = shift @ARGV;
my @matchLines;
if (! $expression)
{
    die("Usage: ./project-scripts/EditMatches.pl EXPRESSION\n");
}

sub readIfFile
{
    my $fileName = shift;
    my $action = shift;
    if (-f $fileName)
    {
        my $text = read_file($fileName);
        $action->($text);
    }
}



sub findMatches
{
    readIfFile($File::Find::name, sub
    {
        my $text = shift;
        my $line = 0;
        my $char = 0;
        while ($text =~ /^(.{$char,}?)($expression)/s)
        {
            my $pre = $1;
            my $match = $2;
            $line = split('\n', $pre);
            $char = length($pre) + length($match);
            my $matchLine = $pre;
            $matchLine =~ s/.*\v//s;
            $matchLine = $matchLine.$match;
            push(@matchLines,
                    {
                        _file  => $File::Find::name,
                        _line  => $line,
                        _match => $matchLine
                    });
            $line += split('\n', $match);
        }
    });
}
find(\&findMatches, Paths::SOURCE_DIR);
find(\&findMatches, Paths::TEST_DIR);
find(\&findMatches, Paths::SCRIPT_DIR);


my $lineCount = @matchLines + 1;
my $skipFile = "";
foreach my $lineMatch (@matchLines)
{
    my $file = $lineMatch->{_file};
    my $line = $lineMatch->{_line};
    my $matchText = $lineMatch->{_match};
    $lineCount--;
    if ($file eq $skipFile)
    {
        next;
    }
    my $input = '';
    while ($input eq '')
    {
        print("$lineCount lines remaining.\n");
        print("Next file:$file\n");
        print("Line match: $matchText\n Edit? "
                ."([y]es, [n]o, [s]kip file, [q]uit):\n");
        chomp($input = <STDIN>);
        if ($input =~ /[^ynsq]/)
        {
            $input = '';
        }
    }

    if ($input eq 'q')
    {
        die("Quitting.\n");
    }
    elsif ($input eq 's')
    {
        $skipFile = $file;
        next;
    }
    elsif ($input eq 'n')
    {
        next;
    }
    elsif ($input ne 'y')
    {
        $input = '';
    }
    system("vim +$line \"$file\" -c 'set colorcolumn=81'");
}
