#!/usr/bin/perl
### CodeFormat.pl #############################################################
# Applies project format conventions to all C++ project code files within a 
# directory.
#
#--- Usage: ---
# !./project-scripts/CodeFormat.pl [OPTIONS] [PATH]
# 
# All paths should be either absolute paths, or relative to the project source
# directory.
#
#--- Options: ---
# -t/-test:     Create logs of all changes that would be made, without changing
#               any code files.
#
# -d/-default:  Scan default project directories in addition to any provided
#               path.
#
# -h/-help:     Print help text and exit.
#
#--- Format rules applied: ---
# 1. With the exception of javadoc-style documentation blocks, always use line
#    comments instead of block comments. This makes it simpler to temporarily
#    disable code using block comments.
#
# 2. When empty or used as a call operator, do not put spaces between opening
#    parentheses and the previous token. In any other circumstances, place one
#    space between an opening parenthesis and the previous token.
#
# 3. Always use Unix-style [0xA] line breaks instead of Windows-style [0xD 0xA]
#    line breaks.
#
# 4. Don't place whitespace characters at the ends of lines.
#
# 5. Always use four spaces instead of the literal tab character.
#
# 6. With the exception of comment blocks, line indentation size should always
#    be in multiples of four.
#
#--- TODO: ---
# - Implement as many additional rules as possible from the project style guide
#   within this script. 
# - Make sure to test each new rule carefully for unusual edge cases.
# - Identify and create exceptions for common ways the rules should be broken to
#   make code more readable. 
# - Add more options for selectively checking and enforcing specific rules.
#==============================================================================#


use strict;
use warnings;
use File::Find;
use File::Slurp;
use FindBin;
use Cwd 'abs_path';
use lib './project-scripts/';
use Paths;
use BlockSearch;

use constant TRUE  => 1;
use constant FALSE => 0;

# Match any valid C++ identifier characters:
my $varChar = '[a-zA-Z0-9_]';

# If true, formatting will be applied directly to project files. If false, all
# formatting changes that would be made are saved to text files in the current
# working directory.
my $testFormats = FALSE;

# Project directory where format rules should be recursively applied:
my $sourcePath;

foreach my $arg (@ARGV)
{
    if ($arg =~ /^-?-?h(elp)?/i)
    {
        print("Usage: !./project-scripts/codeFormat.pl [OPTIONS] [PATH]\n");
        print("\t-t/-test:     Log changes without updating files.\n");
        print("\t-d/-default:  Use default project directories in addition to"
                ." any provided path .\n");
        print("\t-h/--help:    Print this help text.\n");
        exit(0);
    }
    elsif ($arg eq '-t')
    {
        $testFormats = 1;
        if (! $sourcePath)
        {
            $sourcePath = Paths::PROJECT_DIR;
        }
    }
    elsif ($arg eq '-d' || $arg eq '-default')
    {
        $sourcePath = Paths::PROJECT_DIR;
    }
    else
    {
        if ($testFormats || ! $sourcePath)
        {
            $sourcePath = $arg;
        }
    }
}

# Subdirectories to exclude:
my @excluded =
(
    "../build",
    "../debian",
    "../deps",
    "../docs/Doxygen",
    "../JuceLibraryCode",
    "../pack-debian",
    "../.git"
);

# If testing, map format rule expressions to the file name where the changes
# that rule would make should be logged.
my %ruleFiles;

# All rule hashes to apply using formatFile:
my @formatRules =
(
    # Convert non-javadoc multiline comments to individual comment lines:
    {
        _description   =>  'Convert non-javadoc multiline comments to'
                           .'individual comment lines.',

        _expression    =>  '\/\*'     # Comment block opening
                           .'[^*].*?' # Comment block content
                                      # (excluding javadoc /** blocks)
                           .'\*\/',   # Comment block close

        _replaceAction =>  sub 
        {
            my ($pre, $match, $post) = @_;

            # Cut block comment markers from match:
            $match = substr($match, 2);
            $match = substr($match, 0, -2);

            # Copy indentation from last line before the match:
            my $indentSize = length($pre);
            if ($pre =~ /.*\v+(.*?)$/s)    
            {
                $indentSize = length($1)
            }
            my $indent = ' ' x $indentSize;

            # Rebuild the comment one line at a time.
            my $replacement = "";
            my @lines = split("\n", $match);
            foreach my $line (@lines)
            {
                # Remove block comment symbols:
                $line =~ s/^\s*\*?\s*//;
                # Set appropriate line breaks and indentation:
                if (length($replacement) > 0)
                {
                    $replacement = $replacement."\n";
                    if (length($line) > 0)
                    {
                        $replacement = $replacement.$indent;
                    }
                }
                # Add '//' line comment symbol to nonempty lines:
                if (length($line) > 0)
                {
                    $replacement = $replacement."// ".$line;
                }
            }
            # Remove newlines from the end of the block:
            $replacement =~ s/\n+$//;
            return $replacement;
        }
    },
    # Put no spaces before parentheses used as call operators or empty
    # parentheses, one space before all other open parentheses unless they're
    # the first thing on the line.
    {
        _description   =>  'Add appropriate spacing before parentheses.',

        _expression    =>  '\h*\(',

        _replaceAction =>  sub 
        {
            my ($pre, $match, $post) = @_;

            # Save the current amount of space before the parentheses:
            my $parenSpace = substr($match, 0, -1);

            # Preserve original space for parentheses at the start of a line or
            # in a comment line:
            if ($pre =~ /
                    (\v|^)                 # Match line break or start of text,
                    ([^\v]* \/\/ [^\v*])?  # optional single comment line,
                    $                      # and nothing else before the match.
                    /x)
            {
                return $match;
            }
            
            # Preserve original space for parentheses in a quote or comment
            # block:
            my $text = $pre.$match.$post;
            my $matchIndex = length($pre);
            my $matchEnd   = $matchIndex + length($match);
            my $lastBlockEnd = 0;
            while ($lastBlockEnd < $matchIndex)
            {
                my ($type, $blockIndex, $blockLength)
                        = BlockSearch::findBlock($text, $lastBlockEnd,
                        (BlockSearch::DOUBLE_QUOTED 
                        | BlockSearch::SINGLE_QUOTED 
                        | BlockSearch::COMMENT_BLOCK));
                if (! defined($type))
                {
                    last;
                }
                $lastBlockEnd = $blockIndex + $blockLength + 1;
                if ($blockIndex < $matchIndex && $lastBlockEnd >= $matchEnd)
                {
                    return $match;
                }
            }

            # Don't put spaces before empty parentheses:
            if ( ($post =~ /^\s*\)/)
            # Don't put spaces before template class/function call blocks:
                || ($pre =~ /<(?:$varChar|:)*\*?>\s*$/)
            # Don't put spaces after other bracket types:
                || ($pre =~ /[()\[\]\{\}]\s*$/))
            {
                $parenSpace = '';
            }

            # Put spaces before parentheses that follow control blocks or other
            # keywords:
            elsif (($pre =~ /(?<!$varChar)(if|while|for|do|return|case)$/x)
            # Put spaces before parentheses that follow some special character
            # and couldn't be a call operator:
                    || ($pre =~ /(?<!$varChar)$/))
            {
                $parenSpace = " ";
            }
            else
            {
                # Assume all other parentheses are call operators, and should
                # not be preceded with a space:
                $parenSpace = "";
            }
            return $parenSpace.'(';
        }
    },
    {
        _description   =>  'Remove empty spaces from the ends of lines.',
        _expression    =>  '\h+\n',
        _replaceAction =>  sub { return "\n"; }
    },
    {
        _description   =>  'Replace Windows-style newlines.',
        _expression    =>  '',
        _replaceAction =>  sub { return ""; }
    },
    {
        _description   =>  'Never use literal tab characters.',
        _expression    =>  '\t',
        _replaceAction =>  sub { return ""; }
    },
    {
        _description   =>  'Line indentation should be in multiples of four'
                           .'spaces, except in javadoc comment blocks.',

        _expression    =>  '\n\h+(?!\*)',

        _replaceAction => sub
        {
            my ($pre, $match, $post) = @_;
            my $numSpaces = length($match) - 1;
            my $offset = $numSpaces % 4;
            if ($offset == 1)
            {
                $match = substr($match, 0, -$offset);
            }
            elsif ($offset > 1)
            {
                $match = $match.(' ' x (4 - $offset));
            }
            return $match;
        }
    },
    # Place two empty lines between function definitions.
    {
        _description   => 'Place two empty lines between function definitions.',

        _expression    => '\}\n+\/\/',

        _replaceAction => sub { return "}\n\n\n//"; }
    }
);

#==============================================================================#
#--- applyFormat: ---
# Applies a rule to find and replace sections of text.
#--- Parameters: ---
# $text:            The block of text to apply the format change to.
#
# $fileName:        The name of the file the text was loaded from.
#
# $rule:            A hash reference defining a format rule to apply.
#                   Hash rule keys:
#                     _description:    A brief description of the rule.
#
#                     _expression:     A regular expression matching text that
#                                      may be replaced. It should contain no
#                                      capture groups.
#
#                     _replaceAction:  A function that returns a replacement for
#                                      the match. It will be passed all text
#                                      before the match, the full match text,
#                                      and all text after the match as
#                                      parameters.
#--- Returns: ---
# $formattedText:   The $text parameter, with all matches for $expression
#                   processed by $replaceAction.
#
# $changesMade:     The number of changes made to the $text parameter.
#==============================================================================#
sub applyFormat
{
    my ($text, $fileName, $rule) = @_;
    my $expression = $rule->{_expression};
    my $replaceAction = $rule->{_replaceAction};
    my $changesMade = 0;
    my $processedText = "";
    while ($text =~ /^(.*?)($expression)(.*)$/s)
    {
        my $preMatch = $processedText.$1;
        my $match = $2;
        my $postMatch = $3;
        my $replacement = $replaceAction->($preMatch, $match, $postMatch);
        $processedText = $preMatch.$replacement;
        $text = $postMatch;
        if ($replacement ne $match)
        {
            $changesMade++;

            # TESTING: save changes made by each rule to a change log file.
            if ($testFormats)
            {
                my $description = $rule->{_description};
                my $ruleFile;
                if (exists($ruleFiles{$description}))
                {
                    $ruleFile = $ruleFiles{$description};
                }
                else
                {
                    my $ruleNum = scalar keys %ruleFiles;
                    $ruleFile = Paths::PROJECT_DIR.'/formatTest_'.$ruleNum
                            .'_ruleLog.txt';
                    $ruleFiles{$description} = $ruleFile;
                    write_file($ruleFile, "Rule: $description\n"
                            ."Expression: $expression\n");
                }
                # Add limited sections of text before and after the match to
                # provide context:
                my $shortPre = substr($preMatch, -80);
                my $shortPost = substr($postMatch, 0, 80);
                $shortPre =~ s/.*\v//gs;
                $shortPost =~ s/\v.*//gs;
                my $before = $shortPre.$match.$shortPost;
                my $after = $shortPre.$replacement.$shortPost;
                my $div = "\n[".('-' x 70);
                $before =~ s/(\n|^)(?!$)/\n\$ /g;
                $after =~ s/(\n|^)(?!$)/\n\$ /g;
                append_file($ruleFile, "\n".('#' x 80)."\nFile: $fileName"
                        ."\n$div$before$div\n->$div$after$div");
            }
        }
    }
    return $processedText.$text, $changesMade;
}


#==============================================================================#
#--- formatFile: ---
# Loads a file and applies a list of format changes, saving those changes back
# to the source file. if not in testing mode.
#--- Parameters: ---
# $filePath:  A path to a C++ code file to format.
#
# @ruleList:  A list of hash references defining format rules to apply. See the
#             documentation for the applyFormat function for a description of
#             valid rule hash references.
#==============================================================================#
sub formatFile
{
    my $filePath = shift;
    my @ruleList = @_;

    my $text = read_file($filePath);
    my $changesMade = 0;
    foreach my $rule (@ruleList)
    {
        my ($updatedText, $newChanges) = applyFormat($text, $filePath, $rule);
        $changesMade += $newChanges;
        $text = $updatedText;
    }
    if ($changesMade > 0)
    {
        print("Writing $changesMade changes back to $filePath.\n");
        length($text) > 0 or die "Empty output!\n";
        if (! $testFormats)
        {
            write_file($filePath, $text);
        }
    }
}


#==============================================================================#
#--- findAndFormat: ---
# Processes a file located by the File::Find module, formatting valid C++ code
# files with the formatFile function, and pruning excluded directories from the
# list of files to search.
#--- Parameters: ---
# $file: The name of a file found by File::Find.
#==============================================================================#
sub findAndFormat
{
    my $file = $_;
    if (-d $file) # Stop recursion from continuing under excluded directories.
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
    if (-f $file && ($file =~ /\.(cpp|h)$/))
    {
        formatFile($file, @formatRules);
    }
}

# Ensure the script is running in the project's main directory: 
chdir("$FindBin::Bin/..");
# Run recursive formatting search:
find(\&findAndFormat, $sourcePath);
