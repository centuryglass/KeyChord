##### Paths.pm: ################################################################
# Defines project colour file paths
################################################################################
use strict;
use warnings;

package Paths;
use FindBin;
use Cwd;

# Project script directory:
use constant SCRIPT_DIR 
        => $FindBin::Bin;

# Main project directory:
use constant PROJECT_DIR
        => Cwd::abs_path(SCRIPT_DIR.'/..');

# Main source code directory:
use constant SOURCE_DIR
        => PROJECT_DIR."/Source";

# Test code directory:
use constant TEST_DIR
        => PROJECT_DIR."/Tests";

# Documentation directory:
use constant DOC_DIR
        => PROJECT_DIR."/docs";

# Theme::Colour directory:
use constant COLOUR_DIR
        => SOURCE_DIR."/GUI/Theme/Colour";

# ColourId header file:
use constant COLOUR_ID_HEADER
        => COLOUR_DIR."/Theme_Colour_ColourIds.h";

# JSON colour key source file:
use constant COLOUR_KEY_SOURCE
        => COLOUR_DIR."/Theme_Colour_JSONKeys.cpp";

# Default colour configuration file:
use constant DEFAULT_COLOUR_CONFIG
        => PROJECT_DIR."/assets/configuration/colours.json";

# Make sure the script isn't running from somewhere strange, as that will
# result in incorrect path values;
if(SCRIPT_DIR ne PROJECT_DIR.'/project-scripts')
{
    die("Paths.pm: Unexpected directories found, make sure your script is "
            ."running within the main project script directory.\n"
            ."Actual script directory: ".SCRIPT_DIR."\n"
            ."This script will now exit.\n");
}
1;
