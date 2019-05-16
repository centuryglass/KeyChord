#!/usr/bin/perl
# Finds and prints dead links within all markdown documentation files.
use strict;
use warnings;
use File::Find;
use File::Slurp;
use lib './project-scripts';
use Paths;

sub findMatches
{
    if ($File::Find::name =~ /md$/)
    {
        my @lines = read_file($File::Find::name);
        my @badLinks;
        for (my $i = 0; $i < scalar @lines; $i++)
        {
            my $line = $lines[$i];
            my @links = ($line =~ /
                    (?<!!)          # Match isn't following '!' (images)
                    \[[^\]]*\]      # Match the bracketed link text
                    \(([^)]*)\)     # Match and capture the link path
                    /gx);
            foreach my $link (@links)
            {
                if ($link =~ /^http/)
                {
                    if (system("curl --head $link > /dev/null 2>&1") == 0)
                    {
                        next;
                    }
                    else 
                    {
                        die ("\"$link\" was not a valid webpage.\n");
                    }
                }
                elsif (-f $link)
                {
                    next;
                }
                push(@badLinks, { _link => $link, _line => $i });
            }
        }

        my $badLinkCount = @badLinks;
        if ($badLinkCount > 0)
        {
            print("\nFound $badLinkCount link(s) in $File::Find::name:\n");
            foreach my $link (@badLinks)
            {
                print("\tLine $link->{_line}: $link->{_link}\n");
            }
        }
    }
}
find(\&findMatches, Paths::DOC_DIR);
