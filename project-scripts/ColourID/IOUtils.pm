##### IOUtils.pm ###############################################################
# Reads, prints, and writes ColourId data.
################################################################################

##### Functions: #####

#==============================================================================#
#--- cacheCodeFiles: ---
# Populates an IDCache with data from project code files.
#--- Parameters: ---
# $cache: The IDCache object to update.
#==============================================================================#

#==============================================================================#
#--- getDeclarationText: ---
# Gets all cached Element C++ declarations as a single string.
#--- Parameters: ---
# $cache: The IDCache object to read.
#--- Returns: ---
# C++ declaration text for all cached Element objects.
#==============================================================================#

#==============================================================================#
#--- getKeyMapDefinition: ---
# Gets a C++ map declaration  mapping all Element JSON key values to their
# element names.
#--- Parameters: ---
# $cache: The IDCache object to read.
#--- Returns: ---
# C++ text declaring the full static const colourIds map.
#==============================================================================#

#==============================================================================#
#--- getDefaultColourConfig: ---
# Gets the full text of the default colours.json configuration file, updated
# with changes to cached ColourId data.
#--- Parameters: ---
# $cache: The IDCache object to read.
#--- Returns: ---
# The full contents of the updated JSON file.
#==============================================================================#

#==============================================================================#
#--- getMarkdownIDTable: ---
# Generates the markdown table used to document configurable colour keys.
#--- Parameters: ---
# $cache: The IDCache object to read.
#--- Returns: ---
# A nicely formatted markdown table with empty description fields
#==============================================================================#

use strict;
use warnings;

package IOUtils;
use lib './project-scripts';
use Paths;
use lib './project-scripts/ColourID';
use CacheBuilder;
use File::Slurp;
use Scalar::Util qw(blessed);

# Populates an IDCache with data from project code files.
# cache:  The IDCache to update.
sub cacheCodeFiles
{
    my $cache = shift;
    my $headerFile = Paths::COLOUR_ID_HEADER;
    my $keyFile    = Paths::COLOUR_KEY_SOURCE;
    my $jsonFile   = Paths::DEFAULT_COLOUR_CONFIG;

    if (-f $headerFile)
    {
        CacheBuilder::processFile($cache, $headerFile);
    }
    if (-f $keyFile)
    {
        CacheBuilder::processFile($cache, $keyFile);
    }
    if (-f $jsonFile)
    {
        CacheBuilder::processFile($cache, $jsonFile);
    }
}

# Gets all cached Element C++ declarations as a single string.
sub getDeclarationText
{
    my $cache = shift;
    my @namespaces = $cache->getNamespaceNames();
    my $declarations = "";
    foreach my $name (@namespaces)
    {
        my $namespace = $cache->getNamespace($name);
        $declarations = $declarations.$namespace->getDeclarationText();
    }
    return $declarations;
}

# Gets a C++ map declaration mapping all Element JSON key values to their
# element names.
sub getKeyMapDeclaration
{
    my $cache = shift;
    my $indent = " " x 4;
    my $mapText = "static const std::map<juce::Identifier, "
            ."const Element*> keyMap\n{\n";
    my @keys = $cache->getElementKeys();
    foreach my $key (@keys)
    {
        my $element = $cache->findElement($key);
        my $fullName = $element->getFullName();
        if ($key ne $keys[0])
        {
            $mapText = $mapText.",\n";
        }
        $mapText = $mapText."$indent\{\"$key\",\n"
            .($indent x 3)."&$fullName}";
    }
    return $mapText."\n};";
}

# Gets the full text of the default colours.json configuration file, updated
# with changes to cached ColourId data.
sub getDefaultColourConfig
{
    my $cache = shift;
    my $filePath = './assets/configuration/colours.json';
    # Store JSON variables in [key, value] array reference pairs:
    my @orderedPairs;

    my $defaultFile = read_file($filePath);
    # For each category key, find its value in the file, and add the key/value
    # pair to @orderedPairs.
    for (my $i = 0; $i < Category::NUM_TYPES; $i++)
    {
        my $category = new Category($i);
        my $key = $category->getKey();
        my $value = "";
        my $regexKey = $key;
        $regexKey =~ s/([()])/\\$1/g; #Parens need to be escaped!
        if ($defaultFile =~ /"$regexKey"\s*:\s*"(0x[0-9a-f]{8})"/i)
        {
            $value = $1;
        }
        push(@orderedPairs, [$key, $value]);
    }
    push(@orderedPairs, undef);

    my @keys = $cache->getElementKeys();
    my $lastNamespace = "";
    foreach my $key (@keys)
    {
        my $element = $cache->findElement($key);
        if ($element->getNamespace() ne $lastNamespace)
        {
            $lastNamespace = $element->getNamespace();
            # Add undef to @orderedPairs to divide namespaces:
            push(@orderedPairs, undef);
        }
        push(@orderedPairs, [$key, $element->getDefaultColour()]);
    }

    my $maxKeyLength = 0;
    foreach my $kvPair (@orderedPairs)
    {
        if (defined($kvPair))
        {
            my $keyLength = length($kvPair->[0]);
            if ($keyLength > $maxKeyLength)
            {
                $maxKeyLength = $keyLength;
            }
        }
    }

    my $indent = ' ' x 4;
    my $jsonText;
    foreach my $kvPair (@orderedPairs)
    {
        if (!defined($jsonText))
        {
            $jsonText = "{\n";
        }
        elsif (! ($jsonText =~ /\n\n$/))
        {
            $jsonText = $jsonText.",\n";
        }
        if (defined($kvPair))
        {
            my $key = $kvPair->[0];
            my $value = $kvPair->[1];
            # Add extra space so values are aligned:
            my $spacing = ' ' x ($maxKeyLength - length ($key));
            $jsonText = "$jsonText$indent\"$key\"$spacing : \"$value\"";
        }
        else
        {
            $jsonText = $jsonText."\n";
        }
    }
    return $jsonText."\n}";
}

# Generates the markdown table used to document configurable colour keys.
sub getMarkdownIDTable
{
    sub maxLength
    {
        my $maxLength = 0;
        foreach my $string (@_)
        {
            my $testLength = length($string);
            if ($testLength > $maxLength)
            {
                $maxLength = $testLength;
            }
        }
        return $maxLength;
    }
    sub paddedToLength
    {
        my $text = shift;
        my $maxLength = shift;
        return $text.(' ' x ($maxLength - length ($text)));
    }
    my $cache = shift;
    my @keys = $cache->getElementKeys();
    my @categories;
    foreach my $key (@keys)
    {
        my $element = $cache->findElement($key);
        $key = "\"$key\"";
        push(@categories, $element->getCategory()->getKey());
    }

    my $keyHeader = "Colour Value Key";
    my $keyLength = maxLength($keyHeader, @keys);
    my $categoryHeader = "Colour Category";
    my $categoryLength = maxLength($categoryHeader, @categories);

    my $buildRow = sub
    {
        my ($c1, $c2, $c3) = @_;
        my $divider = " | ";
        my $row = paddedToLength($c1, $keyLength).$divider
            .paddedToLength($c2, $categoryLength).$divider;
        if (defined($c3))
        {
            $row = $row.$c3;
        }
        return $row."\n";
    };
    my $output = $buildRow->($keyHeader, $categoryHeader, 'Description')
        .$buildRow->('-' x $keyLength, '-' x $categoryLength, "---");

    for (my $i = 0; $i < (scalar @keys); $i++)
    {
        $output = $output.$buildRow->($keys[$i], $categories[$i]);
    }
    return $output;
}
1;
