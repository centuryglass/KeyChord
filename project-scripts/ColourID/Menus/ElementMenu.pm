##### ElementMenu.pm ###########################################################
# Creates the menu used for editing a single ColourId element
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the element menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $element:  The element object to edit.
#
# $cache:    The IDCache object where changes will be saved.
#==============================================================================#

use strict;
use warnings;

package ElementMenu;
use lib './project-scripts/ColourID';
use UserInput;
use lib './project-scripts/ColourID/DataObjects';
use IDCache;
use Category;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

my $activeElement;

# Displays the element menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my ($element, $cache) = @_;
    $activeElement = $element;
    my $menu = new InputMenu("", "Return to namespace menu:");
    my $refreshAction = sub
    {
        my $menu = shift;
        $menu->clearOptions();
        my $title = "Element ".$activeElement->getFullName()
                ."\nID: ".$activeElement->getID()
                ."\nCategory: ".$activeElement->getCategory()->getTypeName();
        if ($activeElement->getKey())
        {
            $title = $title."\nKey: ".$activeElement->getKey();
        }
        if (length($activeElement->getDefaultColour()) > 0)
        {
            $title = $title."\nDefault colour: "
                    .$activeElement->getDefaultColour();
        }
        $menu->setTitle($title."\nEdit colour Element:");
        $menu->addOption("Rename Element", sub
                { renameElement($menu, $activeElement, $cache); });
        $menu->addOption("Remove Element", sub
                { removeElement($menu, $activeElement, $cache); });
        $menu->addOption("Set category", sub
                { setCategory($menu, $activeElement, $cache); });
        $menu->addOption("Set key", sub
                { setKey($menu, $activeElement, $cache); });
        $menu->addOption("Set default color", sub
                { setDefaultColour($menu, $activeElement, $cache); });
    };
    $menu->setRefreshAction($refreshAction);
    $menu->openMenu();
}


#==============================================================================#
#--- renameElement: ---
# Allows the user to assign a new name to an Element, and closes the
# ElementMenu.
#--- Parameters: ---
# $menu:     The ElementMenu that called this function.
#
# $element:  The element object to rename.
#
# $cache:    The IDCache object where changes will be saved.
#==============================================================================#
sub renameElement
{
    my $menu = shift;
    my $element = shift;
    my $cache = shift;
    my $newName = UserInput::inputText();
    if ($newName)
    {
        my $replacement = $element->withName($newName);
        $cache->removeElement($element);
        $cache->addElement($replacement);
        $activeElement = $replacement;
    }
}


#==============================================================================#
#--- removeElement: ---
# Allows the user to delete an Element, and closes the ElementMenu.
#--- Parameters: ---
# $menu:     The ElementMenu that called this function.
#
# $element:  The element object to remove.
#
# $cache:    The IDCache object where changes will be saved.
#==============================================================================#
sub removeElement
{
    my ($menu, $element, $cache) = @_;
    $cache->removeElement($element);
    $menu->closeMenu();
}


#==============================================================================#
#--- setCategory: ---
# Allows the user to change an element's UI Category.
#--- Parameters: ---
# $menu:     The ElementMenu that called this function.
#
# $element:  The element object to update.
#
# $cache:    The IDCache object where changes will be saved.
#==============================================================================#
sub setCategory
{
    my $menu = shift;
    my $element = shift;
    my $cache = shift;
    my @validOptions = ('q');
    print("Select a category:\n");
    for (my $i = 0; $i < Category::NUM_TYPES; $i++)
    {
        my $category = new Category($i);
        print("$i: ".$category->getTypeName()."\n");
        push(@validOptions, $i);
    }
    print("q: Cancel choice:");
    my $selection = UserInput::checkInput(@validOptions);
    if ($selection ne 'q')
    {
        my $selectedCategory = new Category($selection);
        my $replacement = $element->withCategory($selectedCategory);
        $cache->removeElement($element);
        $cache->addElement($replacement);
        $activeElement = $replacement;
    }
}


#==============================================================================#
#--- setKey: ---
# Allows the user to change an element's JSON key.
#--- Parameters: ---
# $menu:     The ElementMenu that called this function.
#
# $element:  The element object to update.
#
# $cache:    The IDCache object where changes will be saved.
#==============================================================================#
sub setKey
{
    my $menu = shift;
    my $element = shift;
    my $cache = shift;
    my $newKey = UserInput::inputText('.*', "Enter the new element key:");
    if ($newKey)
    {
        $cache->assignKey($element->getID(), $newKey);
        $activeElement = $element->withKey($newKey);
    }
}


#==============================================================================#
#--- setDefaultColour: ---
# Allows the user to change or remove an element's default Colour value.
#--- Parameters: ---
# $menu:     The ElementMenu that called this function.
#
# $element:  The element object to update.
#
# $cache:    The IDCache object where changes will be saved.
#==============================================================================#
sub setDefaultColour
{
    my $menu = shift;
    my $element = shift;
    my $cache = shift;
    my $newColour = UserInput::inputText('^$|^(?:0[xX])?[0-9a-fA-F]{8}$',
            "Enter the new default color:");
    my $replacement = $element->withDefaultColour($newColour);
    $cache->removeElement($element);
    $cache->addElement($replacement);
    $activeElement = $replacement;
}
1;
