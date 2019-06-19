# Configuration Guide
This page links to guides to all configuration files used by this application. These files store all configurable options as JSON data. These files are all stored in the `~/XDG_CONFIG_HOME/KeyChord` directory, or `~/.config/KeyChord` if the XDG_CONFIG_HOME environment variable is not defined. 

#### [KeyChord General Configuration](./configuration/config.md):
Setting general configuration options using config.json.

#### [Character Set Configuration](./configuration/charSets.md):
Configuring displayed keyboard character sets using charSets.json.

#### [Keyboard Binding Configuration](./configuration/charSets.md):
Configuring application control keys using charSets.json.

#### [Colour Configuration](./configuration/colours.md):
Configuring application UI colors using colours.json.

## Default Configuration Files
Default versions of each configuration file are located in `KeyChord/assets/configuration`. On installation, these files are copied to `/usr/share/KeyChord/configuration`. Any values not defined in user configuration files will be copied in from these default files.

## Alternate Configuration Files
Several alternate configuration file defaults are provided as examples:

#### [Alternate Character Set Configuration](./configuration/charSets_alt.md):
This example extends the default letter character set from the Latin alphabet to the Dano-Norwegian alphabet.

#### [Alternate GameShell Key Configuration](./configuration/keyBindings_alt.md):
This example provides an alternate set of keybindings for the GameShell that do not use the light key module.

#### [PC Key Configuration](./configuration/keyBindings_pc.md):
This example provides a set of keybindings useful for testing KeyChord on a standard PC keyboard.
