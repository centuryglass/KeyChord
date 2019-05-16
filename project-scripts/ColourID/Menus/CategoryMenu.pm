##### CategoryMenu.pm ##########################################################
# Constructs the colour category management menu.                              #
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the category menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: The cache object used to save category updates.
#==============================================================================#

use strict;
use warnings;

package CategoryMenu;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

# Displays the category menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "ColourId category sorting:",
            "Return to main menu:",
            undef,
            $cache);
    $menu->addOption("Assign categories to uncategorized Elements",
            sub
            {
                my @uncategorized;
                my @elements = $cache->getElements();
                foreach my $element (@elements)
                {
                    if ($element->getCategory()->getValue() == Category::NONE)
                    {
                        push(@uncategorized, $element);
                    }
                }
                assignCategories($cache, @uncategorized);
            });
    $menu->addOption("Reassign all Element categories",
            sub
            {
                my @elements = $cache->getElements();
                assignCategories($cache, @elements);
            });
    $menu->openMenu();
}

# Assigns a category to a single Element object.
sub assignCategory
{
    my $cache = shift;
    my $element = shift;
    my $defaultOption = shift;
    my @validOptions = ("\n", "q");
    print("\n".$element->getFullName.":\n select a category.\n");
    for (my $i = 0; $i < Category::NUM_TYPES; $i++)
    {
        my $category = new Category($i);
        print($i.". ".$category->getTypeName().".\n");
        push(@validOptions, $i);
    }
    print("[enter]: Repeat option $defaultOption.\n");
    print("q: Cancel assignment:");
    my $selection = UserInput::checkInput(@validOptions);
    if ($selection ne 'q')
    {
        if ($selection eq "\n")
        {
            $selection = $defaultOption;
        }
        my $assignedElement = $element->withCategory(new Category($selection));
        $cache->removeElement($element);
        $cache->addElement($assignedElement);
    }
    return $selection;
}

# Assigns categories to a list of cached Element objects.
sub assignCategories
{
    my $cache = shift;
    my @elements = @_;
    my $numElements = @elements;
    if ($numElements == 0)
    {
        print("No elements to sort.\n");
        return;
    }
    print("Sort $numElements elements by 1:ID, 2:full name, or "
            ."3:element name?:");
    my $sortOrder = UserInput::checkInput('1', '2', '3');
    if ($sortOrder == 2)
    {
        @elements = sort {$a->getFullName() cmp $b->getFullName()}  @elements;
    }
    elsif ($sortOrder == 3)
    {
        @elements = sort {$a->getName() cmp $b->getName()}  @elements;
    }
    my $lastSelection = 0;
    foreach my $element (@elements)
    {
        $lastSelection = assignCategory($cache, $element, $lastSelection);
        if ($lastSelection eq 'q')
        {
            return;
        }
    }
}
1;
