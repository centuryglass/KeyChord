##### EnumMenu.pm ##############################################################
# Constructs the ColourId enum loading menu, parsing ColourId enums and
# selectively using them to populate or update an IDCache object.
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the enum menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: an IDCache object to use for menu options.
#==============================================================================#

use strict;
use warnings;

package EnumMenu;
use lib './project-scripts/ColourID';
use EnumSearch;
use UserInput;
use Paths;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

my $projectDir = Paths::PROJECT_DIR;

# Displays the main menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "ColourId enum search options:",
            "Return to main menu",
            \&loadEnums,
            $cache);
    $menu->addOption("Scan project Source directory", "$projectDir/Source");
    $menu->addOption("Scan JUCE library files", "$projectDir/deps/JUCE");
    $menu->openMenu();
}

#==============================================================================#
#--- loadEnums: ---
# Scans for JUCE ColourId enums, loads them as ColourEnum objects, and
# selectively adds them to an IDCache.
#--- Parameters: ---
# $dir:    The directory to search recursively for ColourId enums.
#
# $cache:  The IDCache object where accepted new ColourId values will be
#          saved.
#==============================================================================#
sub loadEnums
{
    my $dir = shift;
    my $cache = shift;
    my @enums = EnumSearch::findEnums($dir);
    foreach my $enum (@enums)
    {
        my $enumName = $enum->getNamespaceName();
        my @colourNames = $enum->getNames();
        my @colourIds = $enum->getIDs();

        my %matchNames;
        # Check if namespace already exists:
        if (defined($cache->getNamespace($enumName)))
        {
            $matchNames{$enumName} = 1;
        }

        # Check if enum IDs exists under any other namespaces:
        foreach my $id (@colourIds)
        {
            my $element = $cache->findElement($id);
            if (defined($element))
            {
                my $namespaceName = $element->getNamespace();
                if ($namespaceName)
                {
                    $matchNames{$namespaceName} = 1;
                }
            }
        }
        my @matches = keys(%matchNames);
        foreach my $match (@matches)
        {
            my $namespace = $cache->getNamespace($match);
            # If names differ, use enum namespace name, or cached namespace
            # name?
            my $overrideName = undef;
            my $conflictMoveOnly = undef;
            if ($match ne $enumName)
            {
                print("Namespace $match conflicts with enum $enumName.\n");
                print("Choose an option:\n");
                print("1. Move all Elements into $match Namespace.\n");
                print("2. Move all Elements into $enumName Namespace.\n");
                print("3. Keep conflicting Elements in $match Namespace.\n");
                print("4. Move conflicting Elements into "
                        ."$enumName Namespace.\n");
                my $selection = UserInput::checkInput('1', '2', '3', '4');
                if ( ($selection % 2) == 0)
                {
                    $overrideName = $enumName;
                }
                else
                {
                    $overrideName = $match;
                }
                $conflictMoveOnly = ($selection > 2);
            }
            # For each individual ID in the enum:
            for (my $i = 0; $i < (scalar @colourIds); $i++)
            {
                my $id = $colourIds[$i];
                my $name = $colourNames[$i];
                my $cachedElement = $cache->findElement($id);
                my $updatedName = $name;
                # If ID is not cached, ask if it should be added:
                if (!defined($cachedElement))
                {
                    print("Add non-cached enum element $name?");
                    if (UserInput::confirm())
                    {
                        my $namespace = $enumName;
                        if (!$conflictMoveOnly && $overrideName)
                        {
                            $namespace = $overrideName;
                        }
                        my $addedElement = new Element($namespace, $name, $id);
                    }
                }
                else
                {
                    if ($cachedElement->getName() ne $name)
                    {
                        print("Element $name is cached as "
                                .$cachedElement->getName()
                                .", replace cached name?");
                        if (!UserInput::confirm())
                        {
                            $updatedName = $cachedElement->getName();
                        }
                    }
                    my $updatedElement = new Element(
                            $overrideName || $cachedElement->getNamespace(),
                            $updatedName,
                            $id,
                            $cachedElement->getCategory(),
                            $cachedElement->getKey(),
                            $cachedElement->getDefaultColour());
                    $cache->addElement($updatedElement);
                }
            }

            # For each individual ID in the namespace:
            # If ID is not in the enum, ask if it should be removed.
            my @nsElements = $namespace->getElements();
            foreach my $nsElement (@nsElements)
            {
                if (! $enum->contains($nsElement->getID()))
                {
                    print("Remove cached element ".$nsElement->getName()
                            ." that's missing from enum?");
                    if (UserInput::confirm())
                    {
                        $cache->removeElement($nsElement);
                    }
                }
            }
        }
        # If no matches exist, ask to add as a new namespace:
        if ( (scalar @matches) == 0)
        {
            print("Add uncached namespace $enumName?");
            if (UserInput::confirm())
            {
                # Create and add elements for each enum value.
                for (my $i = 0; $i < (scalar @colourIds); $i++)
                {
                    my $id = $colourIds[$i];
                    my $name = $colourNames[$i];
                    $cache->addElement(new Element($enumName, $name, $id));
                }
            }
        }
    }
}

1;
