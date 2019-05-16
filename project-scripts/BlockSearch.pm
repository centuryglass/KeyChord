### BlockSearch.pm #############################################################
# Finds different types of code blocks in C++ files.
################################################################################

##### Functions: #####

#==============================================================================#
#--- nextBlockSymbol: ---
# Finds the next symbol in a string that marks the beginning or end of a block.
#--- Parameters: ---
# $text:              The string to search.
#
# [$startIndex]:      Optionally used to ignore all matches before this index.
#
# [$validBlockTypes]: Block type flags indicating which block types should be
#                     used and which should be ignored. By default, this
#                     includes all block types except angle brackets.
#
# [$validEdgeTypes]:  Edge type flags indicating which edge types will be used
#                     and which will be ignored. By default, this includes all
#                     edge types.
#--- Returns: ---
# $index:             The index of the block string in the text, or undef if no
#                     block symbol is found.
#
# [$blockType]:      The type of block encountered, if one is found.
#
# [$edgeType]:       The edge type of the encountered block character, if one is
#                    found.
#==============================================================================#

#==============================================================================#
#--- findBlock: ---
# Finds the next code block within a string.
#--- Parameters: ---
# $text:          The string to search.
#
# [$startIndex]:  Optionally used to ignore all matches before this index.
#
# [$validTypes:]  Optionally search for certain types of block, using any
#                 combination of block flags.
#--- Returns: ---
# $blockType:     The type of block encountered, or undef if no block is found.
#
# [$index]:       The index of the block's first character in the string, if one
#                 is found.
#
# [$length]:      The length of the code block, if one is found.
#==============================================================================#

use strict;
use warnings;

package BlockSearch;

use constant FALSE => 0;
use constant TRUE  => 1;

##### Block type flags: #####
use constant DOUBLE_QUOTED       => 0b0000001;
use constant SINGLE_QUOTED       => 0b0000010;
use constant COMMENT_BLOCK       => 0b0000100;
use constant SQUARE_BRACKET      => 0b0001000;
use constant CURLY_BRACKET       => 0b0010000;
use constant PARENTHESES         => 0b0100000;
use constant ANGLE_BRACKET       => 0b1000000;

# Allows any block type:
use constant ALL_BLOCK_TYPES     => 0b1111111;

# Default block types used:
# Angle brackets are ignored by default to prevent conflicts with greater than/
# less than operators.
use constant DEFAULT_BLOCK_TYPES => 0b0111111;

my @blockTypes =
(
    DOUBLE_QUOTED,
    SINGLE_QUOTED,
    COMMENT_BLOCK,
    SQUARE_BRACKET,
    CURLY_BRACKET,
    PARENTHESES,
    ANGLE_BRACKET
);

#### Block edge string type flags: ####
#  Edge strings mark the beginning or the end of a block. Edge type indicates if
#  an edge string marks the beginning of a block, the end of a block, or both.

use constant OPEN_EDGE  => 0b001; # Used to open a block.
use constant CLOSE_EDGE => 0b010; # Used to close a block.
use constant DUAL_EDGE  => 0b100; # Used for both opening and closing blocks.
use constant ANY_EDGE   => 0b111;

##### Map block types to their properties: #####
my %blockNames;    # Maps block type to descriptive type name.
my %blockOpen;     # Maps block type to block opening string.
my %blockClose;    # Maps block type to block closing string.
my %allowsNesting; # Maps block type to whether it can contain other blocks.


my %edgeBlockTypes;  # Map block edge strings to their block types.
my %edgeStringTypes; #Map block edge strings to their edge types.

##### Initialize block type properties: #####
sub initType
{
    my ($blockType, $name, $openStr, $closeStr, $allowNesting) = @_;
    $blockNames{$blockType}     = $name;
    $blockOpen{$blockType}      = $openStr;
    $blockClose{$blockType}     = $closeStr;
    $allowsNesting{$blockType}  = $allowNesting;

    $edgeBlockTypes{$openStr}  = $blockType;
    $edgeBlockTypes{$closeStr} = $blockType;

    if ($openStr eq $closeStr)
    {
        $edgeStringTypes{$openStr} = DUAL_EDGE;
    }
    else
    {
        $edgeStringTypes{$openStr}  = OPEN_EDGE;
        $edgeStringTypes{$closeStr} = CLOSE_EDGE;
    }
}
initType(DOUBLE_QUOTED,  "Double Quoted",  "\"", "\"", FALSE);
initType(SINGLE_QUOTED,  "Single Quoted",  "'",  "'",  FALSE);
initType(COMMENT_BLOCK,  "Comment Block",  "/*", "*/", FALSE);
initType(SQUARE_BRACKET, "Square Bracket", "[",  "]",  TRUE);
initType(CURLY_BRACKET,  "Curly Bracket",  "{",  "}",  TRUE);
initType(PARENTHESES,    "Parentheses",    "(",  ")",  TRUE);

# Be careful to avoid conflict with greater than/less than operators!
initType(ANGLE_BRACKET,  "Angle Bracket",  "<",  ">",  TRUE);


# Finds the next symbol in a string that marks the beginning or end of a block.
sub nextBlockSymbol
{
    my ($text, $startIndex, $validBlockTypes, $validEdgeTypes) = @_;
    if (!defined($startIndex))
    {
        $startIndex = 0;
    }
    if (!defined($text) || length($text) == 0
            || $startIndex >= length($text))
    {
        return undef;
    }
    if (!defined($validBlockTypes))
    {
        # Only search for <> blocks when specifically requested.
        $validBlockTypes = DEFAULT_BLOCK_TYPES;
    }
    if (!defined($validEdgeTypes))
    {
        $validEdgeTypes = ANY_EDGE;
    }

    while (TRUE)
    {
        my $searchText = substr($text, $startIndex);
        if ($searchText =~ /
                (^.*?) # Capture all text before match
                (
                    ["'[\]{}\(\)<>]   # Match single-character edge strings
                    |  \/\*         # Match comment block openings
                    |  \*\/         # Match comment block closings
                )/gsx)
        {
            my $preMatch = substr($text, 0, $startIndex).$1;
            my $match = $2;
            my $matchIndex = length($preMatch);

            # Make sure match isn't escaped or commented out:
            my $lastLine = $preMatch;
            $lastLine =~ s/^.*\n+(?!$)//gs;
            if(! (($lastLine =~ /(^|[^\\])\/\//)
                    || ($lastLine =~ /(^|[^\\])\\$/)))
            {
                my $edgeType  = $edgeStringTypes{$match};
                my $blockType = $edgeBlockTypes{$match};
                if ((defined($edgeType) && defined($blockType))
                        && (($edgeType & $validEdgeTypes) != 0)
                        && (($blockType & $validBlockTypes) != 0))
                {
                    return $matchIndex, $blockType, $edgeType;
                }
            }
            $startIndex = $matchIndex + 1;
        }
        else
        {
            return undef;
        }
    }
}

# Finds the next code block within a string.
sub findBlock
{
    my ($text, $startIndex, $validTypes) = @_;
    if (!defined($startIndex))
    {
        $startIndex = 0;
    }
    if (!defined($validTypes))
    {
        $validTypes = DEFAULT_BLOCK_TYPES;
    }

    my %openBlockCounts; # Maps block type to open block count
    my $outerBlockIndex; # Start index of the first encountered block
    my $outerBlockType;  # Block type of the first encountered block

    # Sets which block types are considered valid.
    # When within a non-nesting block, set this to ignore all other block edge
    # characters until the block is closed.
    my $currentValidBlockTypes = $validTypes;

    # Sets which edge types are considered valid.
    # Use this to ignore closing edges before the first block is found, and to
    # ignore opening edges when within a non-nesting block.
    my $currentValidEdgeTypes = OPEN_EDGE | DUAL_EDGE;

    foreach my $type (@blockTypes)
    {
        $openBlockCounts{$type} = 0;
    }

    # Check if all encountered blocks have been closed.
    my $allBlocksClosed = sub
    {
        if (! defined($outerBlockIndex) || ! defined($outerBlockType))
        {
            return FALSE;
        }
        foreach my $type (@blockTypes)
        {
            if ($openBlockCounts{$type} != 0)
            {
                return FALSE;
            }
        }
        return TRUE;
    };

    # Checks if an encountered type is valid.
    my $isValidType = sub
    {
        my $foundType = shift;
        my $flags = shift;
        return ($foundType & $flags) != 0;
    };

    my ($edgeStrIdx, $blockType, $edgeType) 
            = nextBlockSymbol(
                    $text,
                    $startIndex,
                    $currentValidBlockTypes,
                    $currentValidEdgeTypes);
    while (defined($edgeStrIdx))
    {
        my $edgeString = ($edgeType == CLOSE_EDGE) 
                ? $blockClose{$blockType} : $blockOpen{$blockType};
        my $edgeLength = length($edgeString);

        if (!defined($outerBlockIndex))
        {
            $outerBlockIndex = $edgeStrIdx;
            $outerBlockType  = $blockType;
            $currentValidEdgeTypes = ANY_EDGE;
        }
        if ($edgeType == OPEN_EDGE || (($edgeType == DUAL_EDGE)
                    && ($openBlockCounts{$blockType} == 0)))
        {
            $openBlockCounts{$blockType}++;
            if (!$allowsNesting{$blockType})
            {
                $currentValidBlockTypes = $blockType;
                if($edgeType == DUAL_EDGE)
                {
                    $currentValidEdgeTypes = DUAL_EDGE;
                }
                else
                {
                    $currentValidEdgeTypes = CLOSE_EDGE;
                }
            }

        }
        else
        {
            if ($openBlockCounts{$blockType} <= 0)
            {
                # Unbalanced block!
                die("Unbalanced edge $edgeString!\n");
                return undef;
            }
            $openBlockCounts{$blockType}--;
            $currentValidBlockTypes = $validTypes;
            $currentValidEdgeTypes = ANY_EDGE;
        }
        if ($allBlocksClosed->())
        {
            my $length = $edgeStrIdx + $edgeLength - $outerBlockIndex;
            return $outerBlockType, $outerBlockIndex, $length;
        }
        $edgeStrIdx += $edgeLength;
        ($edgeStrIdx, $blockType, $edgeType) = nextBlockSymbol($text,
                $edgeStrIdx, $currentValidBlockTypes, $currentValidEdgeTypes);
    }
    if (defined($outerBlockType) && !$allBlocksClosed->())
    {
        print("Unclosed blocks after $startIndex:\n");
        foreach my $type (@blockTypes)
        {
            my $count = $openBlockCounts{$type};
            if ($count > 0)
            {
                print "[$blockNames{$type}, $count]\n";
            }
            die "$text\n\n";
        }
    }
    return undef;
}
1;
