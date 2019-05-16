##### ExportMenu.pm ############################################################
# Opens the menu providing ColourId export options.                            #
################################################################################

#==============================================================================#
#--- openMenu: ---
# Displays the export menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
#--- Parameters: ---
# $cache: The IDCache source of data to export.
#==============================================================================#

use strict;
use warnings;

package ExportMenu;
use File::Slurp;
use lib './project-scripts/ColourID';
use IOUtils;
use lib './project-scripts/ColourID/Menus';
use InputMenu;

# Displays the export menu, repeatedly accepting input and running the menu
# action with the selected option parameter until the user enters 'q'.
sub openMenu
{
    my $cache = shift;
    my $menu = new InputMenu(
            "Colour data export options:",
            "Return to main menu:",
            \&export);
    $menu->addOption("Export Element object declarations.",
            sub { return IOUtils::getDeclarationText($cache); });
    $menu->addOption("Export <JSONKey, Element> map definition.",
            sub { return IOUtils::getKeyMapDeclaration($cache); });
    $menu->addOption("Export default colours.json file.",
            sub { return IOUtils::getDefaultColourConfig($cache); });
    $menu->addOption("Export markdown colour key table.",
            sub { return IOUtils::getMarkdownIDTable($cache); });
    $menu->openMenu();
}

#==============================================================================#
#--- export: ---
# Exports IDCache data to file or to stdout.
#--- Parameters: ---
# $dataFunction:  A function that takes no parameters and returns the text that
#                 should be exported.
#
# [$outPath]:     An optional path where exported data should be saved. If no
#                 path is given data is exported to stdout.
#==============================================================================#
sub export
{
    my $dataFunction = shift;
    my $outPath = UserInput::inputText(undef,
            "Enter an export path, or press enter to export to stdout:");
    my $exportData = $dataFunction->();
    if ($outPath && write_file($outPath, $exportData))
    {
        print("Exported to $outPath.\n");
    }
    else
    {
        print("Exporting to stdout:\n$exportData\n");
    }
}
1;
