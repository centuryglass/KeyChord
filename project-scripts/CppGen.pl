#!/usr/bin/perl

## CppGen.pl ###################################################################
#  Generates formatted .cpp source files from header files.                    #
#                                                                              #
#-- Description: --------------------------------------------------------------#
#  Given a header file path, CppGen creates a .cpp file defining the values    #
# declared in that header. The new .cpp file is then formatted according to my #
# personal preferences.                                                        #
#                                                                              #
#-- Usage: --------------------------------------------------------------------#
#  ./CppGen.pl [path to input header file] [path to output .cpp file]          #
#                                                                              #
#  If no output path is given, the input path with the extension changed from  #
# .h to .cpp will be used.                                                     #
################################################################################

use strict;
use warnings;
use File::Slurp;
use List::Util qw[max];
use lib './project-scripts';
use BlockSearch;

use constant FALSE => 0;
use constant TRUE  => 1;

# Maximum line length:
use constant MAX_LINE_LENGTH => 80;

# Number of spaces per indent:
use constant INDENT_WIDTH => 4;

###### Useful regular expressions: ######
# Matches a valid identifier
my $idMatch = qr/[A-Za-z_][A-Za-z_0-9]*/;

# Matches a valid identifier, including an optional namespace.
my $fullIDMatch = qr/(?:$idMatch\:\:)*$idMatch/;

# Matches the name of any type of code block that has its own namespace.
my $namedBlockTypeMatch = qr/class|namespace|struct/;

my $inFileName  = $ARGV[0];
my $outFileName = $ARGV[1];
if (!defined($inFileName))
{
    die "usage: perl cpp_from_h.pl  [input file] [output file]\n";
}
if (!defined($outFileName))
{
    $outFileName = $inFileName;
    $outFileName =~ s/\..+$//;
    $outFileName = "$outFileName.cpp";
}

# Comments are only copied if they directly precede a function. Store formatted
# comments here while waiting to see if they're followed by a valid function.
my $pendingCommentBlock = "";

# Tracks types that are declared within a particular namespace, by mapping type
# names to their namespace names.
my %typeNamespaces;

################################################################################
# sub trim ($string)                                                           #
#------------------------------------------------------------------------------#
# Trims leading and trailing whitespace from a string.                         #
#------------------------------------------------------------------------------#
# Parameters:                                                                  #
#   $string: A string to trim.                                                 #
#------------------------------------------------------------------------------#
# Returns: The same string, with all leading and trailing whitespace removed.  #
################################################################################
sub trim
{
    my $string = shift;
    $string =~ s/^\s+//;
    $string =~ s/\s+$//;
    return $string;
}

################################################################################
# sub formatComments ($commentBlock)                                           #
#------------------------------------------------------------------------------#
#  Formats a comment block string.                                             #
#------------------------------------------------------------------------------#
# Parameters:                                                                  #
#   $commentBlock: The comment block text.                                     #
#------------------------------------------------------------------------------#
# Returns:                                                                     #
#   -The comment, with all javadoc fields stripped away, and reformatted to    #
#    fit the entire page width.                                                #
################################################################################
sub formatComments
{
    my $commentBlock = shift;
    if (!defined($commentBlock))
    {
        return "";
    }

    # Remove @brief field name before function summaries.
    $commentBlock =~ s/\s*\@brief\s*/ /g;

    # Remove all other javadoc fields
    $commentBlock =~ s/\n\s*\*\s*@.*?\*\/\n*/\n *\//gs;

    # Remove comment block characters, newlines, and excess whitespace.
    $commentBlock =~ s/^\s*\/\*+//g;   # Trim comment block start
    $commentBlock =~ s/\*+\/\s*$//g;   # Trim comment block end
    $commentBlock =~ s/\v\s*\*\s*/ /g; # Remove comment block asterisks
    $commentBlock =~ s/\v/ /g;         # Remove any remaining newlines
    $commentBlock =~ s/\s\s+/ /g;      # Remove repeated whitespace
    $commentBlock = trim($commentBlock);

    # If no content remains, return the empty string.
    if ($commentBlock =~ /^\s*$/)
    {
        return "";
    }

    # Re-wrap the comment text in a comment block, adding new line breaks to
    # use the whole page width.
    my $formattedComment = "\n"; # Keep one empty line above the block
    my $lineStart = 0;
    my $spaceIdx = 0;
    for (my $i = 0; $i < length($commentBlock); $i++)
    {
        my $blockChar = substr($commentBlock, $i, 1);
        my $lineLength = $i - $lineStart + 3;
        if ($lineLength < MAX_LINE_LENGTH && $blockChar =~ /\s/)
        {
            $spaceIdx = $i;
        }
        if ($lineLength > MAX_LINE_LENGTH || $blockChar =~ /\n/)
        {
            $formattedComment = $formattedComment."\n// "
                .substr($commentBlock, $lineStart, $spaceIdx - $lineStart);
            $spaceIdx++;
            $lineStart = $spaceIdx;
        }
    }
    if ($lineStart < length($commentBlock))
    {
        return $formattedComment."\n// ".substr($commentBlock, $lineStart);
    }
    return $formattedComment;
}

################################################################################
# sub splitParams ($paramStr)                                                  #
#------------------------------------------------------------------------------#
#  Splits a parameter list string on all comma characters that aren't within   #
# sub-blocks.                                                                  #
#------------------------------------------------------------------------------#
# Parameters:                                                                  #
#   $paramListStr: A comma-separated parameter list string, enclosed in        #
#                  parentheses.                                                #
#------------------------------------------------------------------------------#
# Returns: An array of all parameter values.                                   #
################################################################################
sub splitParams
{
    my $paramListStr = shift;
    my @params;
    if (!defined($paramListStr))
    {
        return @params;
    }

    # Find all inner blocks within the list:
    my @innerBlockRanges;
    my $blockSearchIdx = 1;
    while ($blockSearchIdx < length($paramListStr))
    {
        my ($nestedType, $nestedStart, $nestedLen) = BlockSearch::findBlock
            ($paramListStr, $blockSearchIdx, BlockSearch::ALL_BLOCK_TYPES);
        if (!defined($nestedStart) || !defined($nestedLen))
        {
            last;
        }
        my $blockRange =
        {
            'start' => $nestedStart,
            'end' => $nestedStart + $nestedLen
        };
        push(@innerBlockRanges, $blockRange);
        $blockSearchIdx = $blockRange->{'end'} + 1;
    }

    # Finds the first index of a character not within an inner block:
    my $indexOutsideBlocks = sub
    {
        my ($searchChar, $startIndex) = @_;
        my $matchIndex = index($paramListStr, $searchChar, $startIndex);
        foreach my $blockRange (@innerBlockRanges)
        {
            if ($matchIndex == -1)
            {
                return -1;
            }
            elsif ($matchIndex < $blockRange->{'start'})
            {
                return $matchIndex;
            }
            elsif ($matchIndex <= $blockRange->{'end'})
            {
                $matchIndex = index($paramListStr, $searchChar,
                        $blockRange->{'end'})
            }
        }
        return $matchIndex;
    };

    my $paramStart = 1;
    while ($paramStart < length($paramListStr) - 1)
    {
        my $paramEnd = $indexOutsideBlocks->(',', $paramStart);
        if ($paramEnd == -1) # No remaining commas, this is the last parameter.
        {
            $paramEnd = length($paramListStr) - 1;
        }
        # Look for an '=' character not within a nested block to cut default
        # parameter values.
        my $equalsIdx = $indexOutsideBlocks->('=', $paramStart);

        my $paramLength;
        if ($equalsIdx >= 0 && $equalsIdx < $paramEnd)
        {
            $paramLength = $equalsIdx - $paramStart
        }
        else
        {
            $paramLength = $paramEnd - $paramStart;
        }

        my $paramStr = substr($paramListStr, $paramStart, $paramLength);
        $paramStr = trim($paramStr);
        $paramStart = $paramEnd + 1;
        if (length($paramStr) > 0)
        {
            push(@params, $paramStr);
        }
    }
    return @params;
}

################################################################################
# sub formatFunction                                                           #
# ($functionStart, $paramStr, $postParamTxt, $namespace)                       #
#------------------------------------------------------------------------------#
# Creates a formatted function definition using a function declaration.        #
#------------------------------------------------------------------------------#
# Parameters:                                                                  #
#   $functionStart: The function's type and name.                              #
#   $paramStr:      The function's parameter list.                             #
#   $postParamTxt:  Any remaining part of the declaration after the parameters.#
#   $namespace:     The namespace that contains the function.                  #
#------------------------------------------------------------------------------#
# Returns: The formatted empty function definition.                            #
################################################################################
sub formatFunction
{
    my ($functionStart, $paramStr, $postParamTxt, $namespace) = @_;
    my $functionType;
    my $functionName;

    $functionStart =~ s/.*;//gs;    # Remove extra captured code lines
    $functionStart =~ s/\v/ /g;     # Remove all line breaks
    $functionStart =~ s/\s\s+/ /g;  # Remove repeated spaces
    $functionStart =~ trim($functionStart);

    # If this is a valid function, $functionStart should contain type and name.
    if ($functionStart =~ /(^.+)\h+($fullIDMatch)\h*$/s)
    {
        $functionType = trim($1);
        $functionName = trim($2);

        # Check if the type belongs in a namespace:
        my @typeWords = split(/\s+/, $functionType);
        foreach my $typeWord (@typeWords)
        {
            if (exists $typeNamespaces{$typeWord})
            {
                $typeWord = $typeNamespaces{$typeWord}.'::'.$typeWord;
                $functionType = join(' ', @typeWords);
                last;
            }
        }
    }
    else
    {
        # Check if this is a constructor or destructor:
        ($functionName) = ($functionStart =~ /(~?$fullIDMatch)\h*$/);
        my ($localNamespace) = ($namespace =~ /($idMatch)$/);
        if (defined($functionName) && defined($localNamespace)
                && $functionName =~ /$localNamespace$/)
        {
            $functionType = "";
        }
    }
    if (!defined($functionType) || !defined($functionName))
    {
        # Missing type or title, this isn't actually a function declaration.
        return "";
    }

    # Prepend the namespace onto the title if it isn't empty.
    if (length($namespace) > 0)
    {
        $functionName = $namespace.'::'.$functionName;
    }

    # Split the parameter list, dividing on commas that aren't within nested
    # blocks, and removing default parameter values.
    my @params = splitParams($paramStr);

    # Put function back together, adding newlines and indentation where needed.
    my $formattedFunction;
    if (length($functionType) == 0)
    {
        $formattedFunction = "\n$functionName";
    }
    elsif ( (length($functionType) + length($functionName) + 1)
            > MAX_LINE_LENGTH)
    {
        $formattedFunction = "\n$functionType\n$functionName";
    }
    else
    {
        $formattedFunction = "\n$functionType $functionName";
    }
    my $paramLength = 0;
    foreach my $param (@params)
    {
        $paramLength += length($param) + 2;
    }
    $paramLength = max($paramLength, 2);
    my $indent = ' ' x (INDENT_WIDTH * 2);

    # Gets the length of the last line of the formatted function string:
    my $lastLineLength = sub
    {
        if ($formattedFunction =~ /\n ([^\n]*)/s)
        {
            return length($1);
        }
        return length($formattedFunction);
    };

    # Try putting the entire parameter list on one line:
    if ($paramLength < MAX_LINE_LENGTH)
    {
        # Put param list on a new line if needed, or if it would allow params
        # and post-parameter text to fit on one line:
        my $lineLength = $paramLength + $lastLineLength->();
        if (length($postParamTxt) > 0)
        {
            $lineLength += length($postParamTxt) + 1;
        }
        if ($lineLength >= MAX_LINE_LENGTH)
        {
            $formattedFunction = $formattedFunction."\n";
        }
        my $paramStr = "";
        foreach my $param (@params)
        {
            if (length($paramStr) > 0)
            {
                $paramStr = $paramStr.", ";
            }
            $paramStr = $paramStr.$param;
        }
        $formattedFunction = $formattedFunction.'('.$paramStr.')';
    }

    # Couldn't fit parameters on one line, try to spread them between two lines:
    else
    {
        $formattedFunction = $formattedFunction.'(';
        my $functionBackup = $formattedFunction;
        my $numLines = 1;
        foreach my $param (@params)
        {
            # Find the length of the parameter, the space before the parameter
            # (if not the first parameter), and the comma or closing paren
            # after the parameter.
            my $paramLength = length($param) + 1;
            if ($param ne $params[0])
            {
                $paramLength++;
                $formattedFunction = $formattedFunction.',';
            }

            if ( ($lastLineLength->() + $paramLength) > MAX_LINE_LENGTH)
            {
                $numLines++;
                if ($numLines > 2)
                {
                    last;
                }
                $formattedFunction = $formattedFunction."\n$indent$param";
            }
            else
            {
                $formattedFunction = $formattedFunction." $param";
            }
        }

        # Can't fit the parameters within two lines, use one line per param.
        if ($numLines >= 2)
        {
            $formattedFunction = $functionBackup;
            foreach my $param (@params)
            {
                if ($param ne $params[0])
                {
                    $formattedFunction = $formattedFunction.',';
                }
                $formattedFunction = $formattedFunction."\n$indent$param";
            }
        }
        $formattedFunction = $formattedFunction.")";
    }

    # Add post-parameter keywords (const, etc.)
    if (length($postParamTxt) > 0 && ! ($postParamTxt =~ /^\s*$/))
    {
        $postParamTxt =~ s/^\s*//;
        if ( ($lastLineLength->() + length($postParamTxt) + 1)
                > MAX_LINE_LENGTH)
        {
            $formattedFunction = $formattedFunction."\n$indent$postParamTxt";
        }
        else
        {
            $formattedFunction = $formattedFunction." $postParamTxt";
        }
    }

    # Add empty function block:
    $formattedFunction = $formattedFunction."\n{\n}";
    return $formattedFunction;
}


if (-e $outFileName)
{
    my $input = "";
    while ($input ne "Y\n")
    {
        print "Replace existing file $outFileName?(Y/n):";
        $input = <STDIN>;
        if ($input ne "Y\n")
        {
            die "Cancelled.\n";
        }
    }
}

my $headerFile = read_file($inFileName);

sub initialClean
{
    #remove excess indentation
    $headerFile =~ s/\n\s+/\n/gs;

    #remove access specifiers
    $headerFile =~ s/\n+(public|private|protected)+:\s*\n+//g;

    #remove any section between two semicolons that does not
    #contain parentheses or curly brackets
    $headerFile =~ s/;[^;()\{\}]*;/;/g;

    #remove include statements and other misc preprocessor commands
    $headerFile =~ s/(^|\n+\s*)#[^\v]+//gs;

    #remove virtual, static, final, and override keywords
    $headerFile =~ s/virtual|static|final|override//g;

    #remove using specifiers
    $headerFile =~ s/using.*?\n/\n/g;

    #remove repeated newlines
    $headerFile =~ s/\v\v+/\n/gs;

}
initialClean();

# As the header file is processed, the resulting cpp file text will be placed
# in this variable.
my $cppOutput = "";

################################################################################
# sub processBlock                                                             #
# ($startIndex = 0, $endIndex = length ($headerFile), $namespace = "")         #
#------------------------------------------------------------------------------#
# Recursively processes a code block within $headerFile, appending generated   #
# .cpp code to $cppOutput.                                                     #
#------------------------------------------------------------------------------#
# Parameters:                                                                  #
#   $startIndex:  The index to start searching for the next code block.        #
#                                                                              #
#   $endIndex:    An optional endIndex; blocks after the endIndex will be      #
#                 ignored.                                                     #
#                                                                              #
#   $namespace:   A namespace to add before function definitions.              #
#------------------------------------------------------------------------------#
# Returns: The index in the headerFile of the first character past the end of  #
#          the processed block.                                                #
################################################################################
sub processBlock
{
    my $startIndex = shift;
    my $endIndex = shift;
    my $namespace  = shift;
    if (!defined($startIndex))
    {
        $startIndex = 0;
    }
    if (!defined($endIndex))
    {
        $endIndex = length($headerFile);
    }
    if (!defined($namespace))
    {
        $namespace = "";
    }

    # Find the next comment block, {} block, or () block.
    my $searchTypes = BlockSearch::COMMENT_BLOCK | BlockSearch::CURLY_BRACKET
        | BlockSearch::PARENTHESES;
    my ($blockType, $blockIndex, $blockLength) = BlockSearch::findBlock
            ($headerFile, $startIndex, $searchTypes);

    if (!defined($blockType) || !defined($blockIndex)
            || !defined($blockLength) || ($blockIndex >= $endIndex))
    {
        return length($headerFile);
    }

    my $preBlock  = substr($headerFile, $startIndex,
            $blockIndex - $startIndex);
    my $blockText = substr($headerFile, $blockIndex, $blockLength);
    my $blockEndIdx = $blockIndex + $blockLength;

    if ($blockType == BlockSearch::COMMENT_BLOCK)
    {
        $pendingCommentBlock = formatComments($blockText);
        return $blockEndIdx;
    }

    # Possible class/namespace/enum/struct: recursively process contents.
    if ($blockType == BlockSearch::CURLY_BRACKET)
    {
        $pendingCommentBlock = "";
        my $recurseIndex = $blockIndex + 1;
        my $recurseNamespace = "";
        if ($preBlock
                =~ /($namedBlockTypeMatch|enum class|enum)\s+($fullIDMatch)/)
        {
            if (length($namespace) > 0)
            {
                $recurseNamespace = $namespace.'::';
            }
            $recurseNamespace = $recurseNamespace.$2;
            #print "Found namespace $recurseNamespace\n";
        }
        while ($recurseIndex < $blockEndIdx)
        {
            # Look for type declarations within the namespace, but outside of
            # nested blocks.
            if (length($recurseNamespace) > 0)
            {
                my ($nextBlockType, $nextBlockIdx) = BlockSearch::findBlock
                ($blockText, $recurseIndex, $searchTypes);

                if (!defined($nextBlockIdx) || $nextBlockIdx > $blockEndIdx)
                {
                    $nextBlockIdx = $blockEndIdx;
                }
                my $blockContent = substr($headerFile, $recurseIndex,
                        $nextBlockIdx - $recurseIndex);
                my @types = ($blockContent =~
                        /(?:$namedBlockTypeMatch|enum)\s+($fullIDMatch)/g);
                my @typedefs = ($blockContent =~
                        /typedef[^\v]*?($fullIDMatch);/g);
                push(@types, @typedefs);
                foreach my $type (@types)
                {
                    #print "Found type $type in namespace $recurseNamespace\n";
                    $typeNamespaces{$type} = $recurseNamespace;
                }
            }

            $recurseIndex
                = processBlock($recurseIndex, $blockEndIdx, $recurseNamespace);
        }
        return $blockEndIdx;
    }

    # Create function definition from function declaration.
    elsif ($blockType == BlockSearch::PARENTHESES)
    {
        # Remove any extra lines caught in the pre-block text:
        $preBlock =~ s/.*;\n*//gs;

        # Ignore functions defined in the header.
        my $nextBraceIdx = index($headerFile, '{', $blockEndIdx);
        my $nextSemicolonIdx = index($headerFile, ';', $blockEndIdx);
        if ( ( ($nextBraceIdx < $nextSemicolonIdx) && ($nextBraceIdx >= 0))
                || ($nextSemicolonIdx < 0))
        {
            $pendingCommentBlock = "";
            return $blockIndex + $blockLength;
        }


        # Ignore pure virtual functions.
        chomp(my $postParams = substr($headerFile, $blockEndIdx,
                ($nextSemicolonIdx - $blockEndIdx)));
        if ($postParams =~ /=\s*0/)
        {
            $pendingCommentBlock = "";
            return $nextSemicolonIdx;
        }

        my $formattedFunction = formatFunction($preBlock, $blockText,
                $postParams, $namespace);

        if (length($formattedFunction) > 0)
        {
            # Add only one newline above the function.
            $cppOutput =~ s/\v*$//gs;
            # Put empty line and comment block before function:
            $formattedFunction = "\n".$pendingCommentBlock.$formattedFunction;
            $pendingCommentBlock = "";
            # Copy function to output:
            $cppOutput = $cppOutput.$formattedFunction;
            return $nextSemicolonIdx;
        }
        $pendingCommentBlock = "";
    }
    return $blockEndIdx;
}

my $parseIndex = 0;
while ($parseIndex < length($headerFile))
{
    $parseIndex = processBlock($parseIndex);
}

# Include the source header:

my $includeFile = $inFileName;
my $lastPathCharIdx = rindex($includeFile, "/");
if ($lastPathCharIdx >= 0)
{
    $includeFile = substr($includeFile, $lastPathCharIdx + 1);
}

my $includeStatement = "#include \"$includeFile\"";
# Make sure the #include statement is followed by a new line:
if ($cppOutput =~ /^\v?[^\v]/)
{
    $includeStatement = $includeStatement."\n";
}
$cppOutput = $includeStatement.$cppOutput;

write_file($outFileName, $cppOutput);
