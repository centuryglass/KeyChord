##### KeyMenu.pm ###############################################################
# Constructs the JSON colour key editing menu.                                 #
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the key menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: The cache object used to read and write JSON keys
#==============================================================================#

use strict;
use warnings;

package KeyMenu;
use lib './project-scripts/ColourID';
use UserInput;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

# Displays the category menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "JSON configuration key management\nSelect a key to edit:",
            "Return to main menu:",
            \&editKey,
            $cache);
    my $refreshAction = sub
    {
        my $menu = shift;
        $menu->clearOptions();
        my @keys = $cache->getElementKeys();
        foreach my $key (@keys)
        {
            my $element = $cache->findElement($key);
            $menu->addOption($key, $element);
        }
    };
    $menu->setRefreshAction($refreshAction);
    $menu->openMenu();
}

sub editKey
{
    my $element = shift;
    my $cache = shift;
    print("1. Edit key\n2. Delete key\nq. Return to key menu:");
    my $selection = UserInput::checkInput('1', '2', 'q');
    if ($selection == 1)
    {
        print("Enter new key value:");
        my $newKey = UserInput::inputText('.+');
        $cache->assignKey($element->getFullName(), $newKey);
    }
    elsif ($selection == 2)
    {
        my $replacement = $element->withKey("");
        $cache->removeElement($element);
        $cache->addElement($replacement);
    }
}
1;
