# Text Module Documentation
The Text module manages the keyboard's configurable character sets and provides text rendering capabilities.

KeyChord draws text from a custom pixel font image converted into binary data by the [BitFontGen script](../../project-scripts/BitFontGen.pl) and stored in [Text\::BinaryFont](../../Source/GUI/Text/Text_BinaryFont.cpp). To accomodate this choice, text values are stored using a modified version of the ISO 8859 encoding. Most character values are unchanged, but some unique characters are added or moved to reflect their position within the font data. These adjustments are defined in the [Text\::Values namespace](../../Source/Gui/Text/Text_Values.h).

This module was solely implemented for my own amusement, and I would not recommend using this approach in any serious project. In basically any other application, you'll be better off using a proper font rendering library.
