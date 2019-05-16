##### CacheBuilder.pm ##########################################################
# Caches ColourID elements loaded from C++ definitions in a file.              #
################################################################################

##### Functions: #####

#==============================================================================#
#--- processFile: ---
# Parses Element declarations, definitions, keys, or default colours from a file
# and adds them to an IDCache object.
#--- Parameters: ---
# $cache:  The cache object where parsed elements will be added.
#
# $path:   The path of the file to parse.
#==============================================================================#

use strict;
use warnings;

package CacheBuilder;
use lib './project-scripts/ColourID/DataObjects';
use Element;
use Category;
use IDCache;
use File::Slurp;
use Scalar::Util qw(blessed);


# Parses Element declarations, definitions, keys, or default colours from a file
# and adds them to an IDCache object.
sub processFile
{
    my $cache = shift;
    my $path = shift;
    if ($path =~ /json$/i)
    {
        parseJSON($cache, $path);
        return;
    }
    my $file = read_file($path) or die("Failed to read $path\n");

    my @elements;
    # Parse definitions that are also declarations:
    my @innerNamespaces = ($file =~ /
            namespace\ \w+\s+{ # match any namespace
            [^{]*?}            # that contains no inner scopes /xgs);
    foreach my $namespace (@innerNamespaces)
    {
        my $namespaceName;
        if ($namespace =~ /namespace (\w+)/)
        {
            $namespaceName = $1;
        }
        else
        {
            print("CacheBuilder::ProcessFile: Invalid namespace found.\n");
            print("namespace= $namespace\n");
            next;
        }
        my @elementDefs = ($namespace =~ /
                static\ const\ Element\  # Match Element declaration type
                [a-zA-Z_][a-zA-Z0-9_]*   # Match valid Element name
                \s*\(.*?\)\s*;           # Match parameters
                /gsx);
        foreach my $definition (@elementDefs)
        {
            my $element = new Element($namespaceName, $definition);
            if (defined($element))
            {
                push(@elements, $element);
            }
        }
    }

    # parse config key definitions:
    if ($file =~ /
            map\s*<\s*juce::Identifier\s*,
            \s*const\s*Element\s*\*>                     # Match key map type
            [^{]+                                        # Match map name
            { ([^;]+)};                                   # Match map contents
            /sx)
    {
        my $keyMap = $1;
        my @keyList = ($keyMap =~ /{[^}]+}/gs);
        my $numKeys = @keyList;
        if ($numKeys > 0)
        {
            print("Found key map with $numKeys key (s) defined in $path\n");
        }
        foreach my $keyPair (@keyList)
        {
            if ($keyPair =~ /
                    {\s*
                    "(.*?)"  # Capture key string
                    \s*,\s*
                    & (.*?)   # Capture associated Element
                    \s*}
                    /sx)
            {
                my $key = $1;
                my $elementName = $2;
                $cache->assignKey($elementName, $key);
            }
            else
            {
                print("Found invalid <key, element> pair $keyPair\n");
            }
        }
    }

    my $numElements = @elements;
    if ($numElements > 0)
    {
        print("Found $numElements elements to cache in $path.\n");
    }
    foreach my $element (@elements)
    {
        if (defined($element) && blessed($element)
                && blessed($element) eq 'Element')
        {
            $cache->addElement($element);
        }
    }
}

# Parse default colour values from a JSON file.
sub parseJSON
{
    my $cache = shift;
    my $path = shift;
    my $file = read_file($path);
    my @configValues = ($file =~ /
            "[^"]+" # match quoted key
            \s*:\s*
            "[^"]+" # match quoted value
            /xgsi);
    my $numValues = @configValues;
    if ($numValues > 0)
    {
        print("Found $numValues default colour values in $path.\n");
    }
    foreach my $value (@configValues)
    {
        if ($value =~ /
                "(.+?)"           # capture quoted key
                \s*:\s*
                "(0x[0-9a-f]{8})" # capture quoted hex value
                /xsi)
        {
            my $key = $1;
            my $colour = $2;
            my $element = $cache->findElement($key);
            if (defined($element))
            {
                my $updated = $element->withKey($key);
                $updated = $updated->withDefaultColour($colour);
                $cache->removeElement($element);
                $cache->addElement($updated);
            }
        }
        else
        {
            print("No match: $value\n");
        }
    }
}
1;
