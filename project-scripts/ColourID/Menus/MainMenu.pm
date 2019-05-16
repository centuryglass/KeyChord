##### MainMenu.pm ##############################################################
# Constructs the main ColourId management menu.                                #
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the main menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: an IDCache object to use for menu options.
#==============================================================================#

use strict;
use warnings;

package MainMenu;
use lib './project-scripts/ColourID/Menus';
use InputMenu;
use EnumMenu;
use NamespaceListMenu;
use KeyMenu;
use CategoryMenu;
use ExportMenu;
use UpdateMenu;


# Displays the main menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "ColourId management tool options:",
            "Exit program:");
    $menu->addOption("Scan project for new ColourId enums",
            sub { EnumMenu::openMenu($cache); });
    $menu->addOption("Find next open ID value",
            sub
            {
                my @elements = $cache->getElements();
                my $lastID = hex($elements[-1]->getID()) + 0x100;
                $lastID = sprintf("0x%x", $lastID);
                print("Start the next new ColourId enum at $lastID\n");
            });
    $menu->addOption("Edit Element namespaces",
            sub { NamespaceListMenu::openMenu($cache); });
    $menu->addOption("Edit custom colour JSON keys",
            sub { KeyMenu::openMenu($cache); });
    $menu->addOption("Assign UI categories",
            sub { CategoryMenu::openMenu($cache); });
    $menu->addOption("Export cached data",
            sub { ExportMenu::openMenu($cache); });
    $menu->addOption("Update project files",
            sub { UpdateMenu::openMenu($cache); });
    $menu->openMenu();
}
1;
