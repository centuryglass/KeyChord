##### UpdateMenu.pm ############################################################
# Constructs the menu used for updating project files with new data.           #
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the update menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: The IDCache object source of updated colour data.
#==============================================================================#

use strict;
use warnings;

package UpdateMenu;
use File::Slurp;
use lib './project-scripts/ColourID/';
use IOUtils;
use Paths;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

# Displays the update menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "Project file update options:",
            "Exit to main menu:",
            undef,
            $cache);
    $menu->addOption("Update all colour files.",
            sub
            {
                updateIDHeader($cache);
                updateKeyFile($cache);
                updateDefaultConfig($cache);
                updateConfigDoc($cache);
            });
    $menu->addOption("Update ColourId header file",
            \&updateIDHeader);
    $menu->addOption("Update Colour JSON key file",
            \&updateKeyFile);
    $menu->addOption("Update default colours.json file",
            \&updateDefaultConfig);
    $menu->addOption("Update colour key documentation table",
            \&updateConfigDoc);
    $menu->openMenu();
}


#==============================================================================#
#--- updateIDHeader: ---
# Writes changes in cached Elements to the header file where all project
# Elements are defined.
#--- Parameters: ---
# $cache: The IDCache object that provides all updated Element data.
#==============================================================================#
sub updateIDHeader
{
    my $cache = shift;
    if (!defined($cache))
    {
        die "UpdateMenu::updateIDHeader: Invalid cache!\n";
    }
    my $headerPath = Paths::COLOUR_ID_HEADER;
    if (! -f $headerPath)
    {
        print("Unable to find ColourIds header file!\n"
                ."Path tested:$headerPath\n");
        return;
    }
    my $sourceText = read_file($headerPath);

    my $updatedText;

    #find text before the start of element namespaces:
    if ($sourceText =~ /
            ^ (.*?)\h*                  # Capture from the start of the file
            namespace\ \w+\s+           # until just before the first namespace
            {\s+static\ const\ Element  # starting with an Element declaration.
            /sx)
    {
        $updatedText = $1;
    }
    else
    {
        print("Unable to find start of Element declarations, cancelling.\n");
        return;
    }

    # Remove and replace old element declarations:
    if ($sourceText =~ /
            .*                      # Match beginning of file
            static\ const\ Element  # Match start of last Element declaration
            [^}]+}\v*               # Match remaining Element namespace
            (\v.*?)$                # Capture all text after last namespace
            /sx)
    {
        $sourceText = $1;
    }
    else
    {
        print("Unable to find end of Element declarations, cancelling.\n");
        return;
    }
    $updatedText = $updatedText.IOUtils::getDeclarationText($cache);

    # Scan for the start of the element array, add text between the Element
    # declarations and the complete array to the updated text:
    if ($sourceText =~ /(.*allElements =\s+).*/s)
    {
        $updatedText = $updatedText.$1;
    }
    else
    {
        print("Unable to find start of Element array, cancelling.\n");
        return;
    }

    # Build array contents:
    my $arrayText = "{\n";
    my @elements = $cache->getElements();
    for (my $i = 0; $i < (scalar @elements); $i++)
    {
        if ($i > 0)
        {
            $arrayText = "$arrayText,\n";
        }
        $arrayText = $arrayText."    &".$elements[$i]->getFullName();
    }
    $arrayText = $arrayText."\n};";
    $updatedText = $updatedText.$arrayText;

    # Save text after element array:
    if ($sourceText =~ /
        allElements\ =\s+ # Match start of array definition
        {[^}]*?};         # Match end of array definition
        (.*)$             # Capture all remaining text
        /xs)
    {
        $updatedText = $updatedText.$1;
    }
    else
    {
        print("Unable to find text after full Element array, cancelling.\n");
        return;
    }
    if (write_file($headerPath, $updatedText))
    {
        print("Updated header file.\n");
    }
    else
    {
        print("Failed to write to header file.\n");
    }
}


#==============================================================================#
#--- updateKeyFile: ---
# Writes changes in cached Element JSON keys to the header file where all
# ColourID keys are defined.
#--- Parameters: ---
# $cache: The IDCache object that provides all updated key data.
#==============================================================================#
sub updateKeyFile
{
    my $cache = shift;
    my $keyPath = Paths::COLOUR_KEY_SOURCE;
    my $fileText = read_file($keyPath);

    my $startText, my $endText;

    # Matches the key/element map declaration:
    my $mapNameMatch
        = 'static const std::map<juce::Identifier, const Element\*> keyMap';

    # Find file text before key map:
    if ($fileText =~ /(.*)$mapNameMatch/s)
    {
        $startText = $1;
    }
    else
    {
        print("Failed to find start of colour map, cancelling.\n");
        return;
    }

    # Find file text after key map:
    if ($fileText =~ /$mapNameMatch[^;]+;(.*)$/s)
    {
        $endText = $1;
    }
    else
    {
        print("Failed to find end of colour map, cancelling.\n");
        return;
    }

    my $map = IOUtils::getKeyMapDeclaration($cache);
    $fileText = $startText.$map.$endText;

    # Cut the DOS-style newlines it adds to this one particular file, for
    # whatever reason:
    $fileText =~ s///g;
    # TODO: Figure out why it's doing that and stop it instead of just cleaning
    #       up after it.

    if (write_file($keyPath, $fileText))
    {
        print("Updated JSON key file.\n");
    }
    else
    {
        print("Failed to update JSON key file.\n");
    }
}


#==============================================================================#
#--- updateDefaultConfig: ---
# Writes changes to the list of ColourId keys and default colour values to the
# default colour configuration file.
#--- Parameters: ---
# $cache: The IDCache object that provides all updated key and colour data.
#==============================================================================#
sub updateDefaultConfig
{
    my $cache = shift;
    my $configText = IOUtils::getDefaultColourConfig($cache);
    if (write_file(Paths::DEFAULT_COLOUR_CONFIG, $configText))
    {
        print("Updated default json file.\n");
    }
    else
    {
        print("Failed to write to default json file.\n");
    }
}

#==============================================================================#
#--- updateConfigDoc: ---
# Adds new key values to the ones documented in the colour configuration file,
# and marks removed keys.
#--- Parameters: ---
# $cache: The IDCache object that provides all updated key and colour data.
#==============================================================================#
sub updateConfigDoc
{
    my $cache = shift;
    my $docPath = Paths::DOC_DIR."/configuration/colours.md";
    my $docText = read_file($docPath);

    my ($mainDoc, $colourTable) 
            = ($docText =~ /^(.*\n)Colour Value Key[^"]+(.*)$/s);
    if(! $mainDoc || ! $colourTable)
    {
        print("Failed to find colour key table.\n");
        return;
    }

    my $updatedTable = IOUtils::getMarkdownIDTable($cache);

    my @oldLines = split("\n", $colourTable);
    my @newLines = split("\n", $updatedTable);

    # Remove table header from newlines:
    my $header = "$newLines[0]\n$newLines[1]\n";
    splice(@newLines, 0, 2);

    # Finds a key in the list of old table lines:
    my $findLine = sub
    {
        my $key = shift;
        for (my $i = 0; $i < (scalar @oldLines); $i++)
        {
            if (substr($oldLines[$i], 0, length($key)) eq $key)
            {
                return $i;
            }
        }
        return undef;
    };

    foreach my $line (@newLines)
    {
        my ($key) = ($line =~ /^(\".+?\")/s);
        if (! $key)
        {
            print("Warning: failed to find valid key in new table line.\n"
                    ."failed line: ".$line."\n");
            next;
        }
        my $oldIndex = $findLine->($key);
        if (defined($oldIndex))
        {
            my ($oldDescription) = ($oldLines[$oldIndex] =~ /\|\s*([^|]*?)$/s);
            splice(@oldLines, $oldIndex, 1);
            if (defined($oldDescription))
            {
                $line = $line.$oldDescription;
                next;
            }
            else
            {
                print("Key $key is not new, but is missing a description.\n");
            }
        }
        print("Enter description for $key, or press enter to skip:");
        chomp(my $description = <STDIN>);
        $line = $line.$description;
    }
    # Mark removed lines for removal, but don't actually remove them or else
    # it'll throw away the old description for any colour that just had its key
    # changed.
    foreach my $removed (@oldLines)
    {
        push(@newLines, "REMOVED:".$removed);

    }

    if (write_file($docPath, $mainDoc.$header.join("\n", @newLines)))
    {
        print("Updated colour key documentation file.\n");
    }
    else
    {
        print("Failed to write to key documentation file.\n");
    }
}
1;
