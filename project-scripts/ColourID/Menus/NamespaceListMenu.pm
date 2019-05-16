##### NamespaceListMenu.pm #####################################################
# Constructs the namespace management menu.
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the namespace list menu, repeatedly accepting input and running the
# menu action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: an IDCache object to use for menu options.
#==============================================================================#

use strict;
use warnings;

package NamespaceListMenu;
use lib './project-scripts/ColourID/Menus';
use InputMenu;
use NamespaceMenu;

# Displays the namespace list menu, repeatedly accepting input and running the
# menu action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "Edit ColourId namespaces:",
            "Return to main menu",
            \&NamespaceMenu::openMenu,
            $cache);
    my $refreshAction = sub
    {
        print("Running refresh...\n");
        my $menu = shift;
        $menu->clearOptions();
        my @namespaceNames = $cache->getNamespaceNames();
        foreach my $name (@namespaceNames)
        {
            $menu->addOption($name, $name);
        }
    };
    $menu->setRefreshAction($refreshAction);
    $menu->openMenu();
}
1;
