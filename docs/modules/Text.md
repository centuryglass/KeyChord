# Text Module Documentation
The Text module manages the keyboard's configurable character sets and provides text rendering capabilities.

KeyChord draws text from a custom pixel font image converted into binary data by the [BitFontGen script](../../project-scripts/BitFontGen.pl) and stored in [Text\::BinaryFont](../../Source/GUI/Text/Text_BinaryFont.cpp). To accomodate this choice, text values are stored using a modified version of the ISO 8859 encoding. Most character values are unchanged, but some unique characters are added or moved to reflect their position within the font data. These adjustments are defined in the [Text\::Values namespace](../../Source/Gui/Text/Text_Values.h).

This module was mainly implemented for my own amusement, and I wouldn't recommend using this approach in any serious project. In basically any other circumstances you'll be better off using a proper font rendering library.

#### [Text\::CharTypes](../../Source/GUI/Text/Text_CharTypes.h)
CharTypes defines the data type and container types used by the Text module to store printable characters.

#### [Text\::Values](../../Source/GUI/Text/Text_Values.h)
The Values namespace defines all printable character values that are not standard ISO 8859 values, and provides functions for checking the properties of character values and converting them to and from other formats.

#### [Text\::BinaryFont](../../Source/GUI/Text/Text_BinaryFont.h)
BinaryFont stores and shares the data used to determine how to print each character. Characters are accessed as rows of binary data, where active bits represent the locations where pixels should be drawn.

#### [Text\::Painter](../../Source/GUI/Text/Text_Painter.h)
The Painter namespace uses BinaryFont data and JUCE graphics to draw characters within the application's window.

## CharSet Submodule
Using five chord keys, up to 31 different key codes may be entered. In order to provide a more complete set of keys, KeyChord uses four character sets. Each character set may also have an alternate set of shifted values. This expands the maximum number of key codes to 248, more than enough to match the typical physical keyboard.

With the exception of the modifier character set, all other character sets are fully configurable. Character sets are defined in the charSets.json configuration file. This file is used to set each character set's displayed name, the types of characters in each character set, the order they're displayed in, their shifted values, and the order that chord combinations are assigned to them.

#### [Text\::CharSet\::Cache](../../Source/GUI/Text/CharSet/Text_CharSet_Cache.h)
Stores a character set's data, loaded from the CharSet configuration file. This includes the set's full character list, shifted character list, and the set of chord keys assigned to each character.

#### [Text\::CharSet\::Type](../../Source/GUI/Text/CharSet/Text_CharSet_Type.h)
CharSet::Type defines the four types of available character set.

#### [Text\::CharSet\::ConfigFile](../../Source/GUI/Text/CharSet/Text_CharSet_ConfigFile.h)
ConfigFile objects access the character set configuration resource to read character set names and get CharSet\::Cache objects.

#### [Text\::CharSet\::JSONResource](../../Source/GUI/Text/CharSet/Text_CharSet_JSONResource.h)
JSONResource handles all direct access to the JSON character set configuration file.

#### [Text\::CharSet\::JSONKeys](../../Source/GUI/Text/CharSet/Text_CharSet_JSONKeys.h)
JSONKeys defines all key strings used to store data in the JSON character set configuration file.
