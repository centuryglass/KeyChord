##### ColourEnum.pm ############################################################
# Finds juce::ColourId enums in header files.                                  #
################################################################################

##### Functions: #####

#==============================================================================#
#--- extractColourEnums: ---
# Extracts and returns ColourId enums from a block of text.
#--- Parameters: ---
# $namespace: The namespace name to assign to all located enums.
#
# $text:      A string to search for ColourId enum declarations.
#--- Return: ---
# An array of all located enum declarations, parsed into ColourEnum objects.
#==============================================================================#

#==============================================================================#
#--- fileToNamespaceName: ---
# Uses the name of a file to generate a suitable name for the namespace that
# will hold the file's ColourId Element objects.
#--- Parameters: ---
# $filename: A C++ header file name.
#--- Return: ---
# A CamelCase name with all underscores, file extensions, and JUCE library
# prefixes removed.
#==============================================================================#

#==============================================================================#
#--- findEnums: ---
# Recursively searches all header files under a directory for ColourID enum
# definitions, and returns all located enums parsed as
# will hold the file's ColourId Element objects.
#--- Parameters: ---
# $path: A directory path to recursively search under.
#--- Return: ---
# An array of ColourEnum objects parsed from all files under the directory path.
#==============================================================================#

use strict;
use warnings;

package EnumSearch;
use File::Find;
use File::Slurp;
use lib './project-scripts/ColourID/DataObjects';
use ColourEnum;
use IDCache;

# Extracts and returns ColourId enums from a block of text.
sub extractColourEnums
{
    my $namespace = shift;
    my $text = shift;
    my @enums = ($text =~ /enum\s+ColourIds\s*\{.*?\}/gs);
    for (my $i = 0; $i < (scalar @enums); $i++)
    {
        $enums[$i] = new ColourEnum($namespace, $enums[$i]);
    }
    return @enums;
}

# Uses the name of a file to generate a suitable name for the namespace that
# will hold the file's ColourId Element objects.
sub fileToNamespaceName
{
    my $filename = shift;
    $filename =~ s/juce (r)?//g;
    $filename =~ s/.*\///g;
    $filename =~ s/\.h$//g;
    for (my $i = 0; $i < (length($filename) - 1); $i++)
    {
        my $char = substr($filename, $i, 1);
        if ($char eq "_")
        {
            $filename = substr($filename, 0, $i).uc($char)
                    .substr($filename, $i + 1);
        }
    }
    $filename =~ s/^_//g;
    return ucfirst($filename);
}

# Recursively finds ColourId enums in headers within a directory
sub findEnums
{
    my $path = shift;
    my @enums;
    my $headerSearch = sub
    {
        my $filename = $File::Find::name;
        if ($filename && $filename =~ /.*\.h$/)
        {
            my $namespace = fileToNamespaceName($filename);
            my $headerFile = read_file($filename);
            push(@enums, extractColourEnums($namespace, $headerFile));
        }
    };
    find($headerSearch, $path);
    print("EnumSearch::findEnums: Found ".(scalar @enums)." ColourId enums in "
            .$path.".\n");
    return @enums;
}
1;
