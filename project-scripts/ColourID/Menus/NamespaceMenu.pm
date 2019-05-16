##### NamespaceMenu.pm #########################################################
# Creates the menu used for editing a single ColourId namespace.
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the namespace menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: an IDCache object to use for menu options.
#==============================================================================#

use strict;
use warnings;

package NamespaceMenu;
use lib './project-scripts/ColourID';
use UserInput;
use lib './project-scripts/ColourID/Menus';
use InputMenu;
use ElementMenu;

my $activeNamespace = "";

# Displays the namespace menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my ($namespaceName, $cache) = @_;
    $activeNamespace = $namespaceName;
    my $menu = new InputMenu("", "Return to namespace list menu");
    my $refreshAction = sub
    {
        my $menu = shift;
        $menu->setTitle("Edit Namespace $activeNamespace:");
        $menu->clearOptions();
        $menu->addOption("Rename namespace", sub
        {
            renameNamespace($menu, $cache);
        });
        $menu->addOption("Delete namespace", sub
        {
            deleteNamespace($menu, $cache);
        });
        my $namespace = $cache->findNamespace($activeNamespace);
        if (defined($namespace))
        {
            my @elements = $namespace->getElements();
            foreach my $element (@elements)
            {
                $menu->addOption("Edit Element ".$element->getID()
                        ." (".$element->getName().")",
                        sub { openElementMenu($element, $cache); });
            }
        }
    };
    $menu->setRefreshAction($refreshAction);
    $menu->openMenu();
}


#==============================================================================#
#--- renameNamespace: ---
# Prompts the user to rename the edited namespace, then applies the new name and
# closes the menu if the name is valid and the user chooses to confirm.
#--- Parameters: ---
# $menu:          The NamespaceMenu object.
#
# $cache:         The IDCache where the Namespace will be updated.
#
# $namespaceName: The name of the Namespace to update.
#==============================================================================#
sub renameNamespace
{
    my $menu = shift;
    my $cache = shift;
    my $newName = UserInput::inputText(
            '[a-zA-Z_][a-zA-Z_0-9]*',
            "Enter the new name for Namespace $activeNamespace:",
            undef,
            "Save new name for $activeNamespace?");
    if (!$newName || $newName eq $activeNamespace)
    {
        return;
    }
    my $namespace = $cache->findNamespace($activeNamespace);
    my @elements = $namespace->getElements();
    foreach my $element (@elements)
    {
        my $replacement = $element->withNamespace($newName);
        $cache->removeElement($element);
        $cache->addElement($replacement);
    }
    $activeNamespace = $newName;
}


#==============================================================================#
#--- deleteNamespace: ---
# If the user confirms the action, deletes the edited namespace and closes the
# namespace menu.
#--- Parameters: ---
# $menu:          The NamespaceMenu object.
#
# $cache:         The IDCache where the Namespace will be deleted.
#
# $namespaceName: The name of the Namespace to remove.
#==============================================================================#
sub deleteNamespace
{
    my $menu = shift;
    my $cache = shift;
    print("Delete namespace $activeNamespace?");
    if (!UserInput::confirm())
    {
        return;
    }
    my $namespace = $cache->findNamespace($activeNamespace);
    my @elements = $namespace->getElements();
    foreach my $element (@elements)
    {
        $cache->removeElement($element);
    }
    $menu->closeMenu();
}


#==============================================================================#
#--- openElementMenu: ---
# Opens a menu to edit an individual namespace Element.
#--- Parameters: ---
# $element:  The ColourId Element to edit.
#
# $cache:    The IDCache object where any Element updates will be saved.
#==============================================================================#
sub openElementMenu
{
    my $element = shift;
    my $cache = shift;
    ElementMenu::openMenu($element, $cache);
}
1;
