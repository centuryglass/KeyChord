#!/usr/bin/perl
### ColourId Element Manager ###################################################
# Provides options for automatically generating, updating, and managing the
# JUCE ColourId element list and the list of configurable UI colour value keys.
################################################################################

use strict;
use warnings;
use lib './project-scripts/ColourID';
use IOUtils;
use lib './project-scripts/ColourID/Menus';
use MainMenu;

my $cache = new IDCache();
IOUtils::cacheCodeFiles($cache);
MainMenu::openMenu($cache);
