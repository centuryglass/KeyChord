# Custom Key Binding Configuration
The [keyBindings.json](../../assets/configuration/keyBindings.json) file sets the keyboard keys used to control KeyChord. Key names are defined using JUCE KeyPress description strings. See the [JUCE KeyPress class](https://github.com/WeAreROLI/JUCE/blob/5.0.0/modules/juce_gui_basics/keyboard/juce_KeyPress.cpp) for the full list of valid key strings. Bindings using modifier keys are not currently supported.  Actions without a valid bound key will be ignored, and not listed on the KeyChord help screen.

## Key Binding JSON objects
Each key binding action is defined using its own JSON object with the following format:

JSON Key   | Description
---------- | ---
"key"      | The name of the keyboard key that will trigger the bound action.
"name"     | A name used to describe the key in the KeyChord help screen.
"charName" | A character value used to represent the key in the help screen or chord key preview. Character names follow the same rules as [custom character values](./charSets.md).

## Configurable Keys/Actions

Key/Action                       | Description
-------------------------------- | ---
"Chord1"                         | First chorded input key.
"Chord2"                         | Second chorded input key.
"Chord3"                         | Third chorded input key.
"Chord4"                         | Fourth chorded input key.
"Chord5"                         | Fifth chorded input key.
"Select next character set"      | Cycles through all available character sets.
"Toggle shifted characters"      | Switches between using normal and shifted character values.
"Select main character set"      | Selects the main character set. By default, this set contains the Latin alphabet and the space character.
"Select alternate character set" | Selects the alternate character set. By default, this set contains the numeric characters and symbol keys.
"Select special character set"   | Selects the special character set. By default, this set contains return, backspace, escape, tab, the arrow keys, and the function keys.
"Select modifier set"            | Selects the modifier character set. This set only contains control, alt, super(command/windows), and shift.
"Backspace"                      | Deletes the last character in the input buffer, or sends the backspace key in immediate input mode.
"Clear all"                      | Clears the input character.
"Send text"                      | Sends all characters in the input buffer to the target application window, or sends the return key in immediate input mode.
"Close and send"                 | Sends all characters in the input buffer to the target application window, and closes KeyChord.
"Close"                          | Closes KeyChord, discarding any buffered input.
"Toggle immediate mode"          | Turns immediate mode on or off. In immediate mode, characters entered using the chord keys are immediately sent to the target application window.
"Show help"                      | Shows the help screen, where all key bindings are listed.
"Toggle window edge"             | Switches the KeyChord window between the top and bottom display edges.
"Toggle minimize"                | Enables or disables minimized display mode. When minimized, assigned chords and chord key previews will not be shown, and the KeyChord window will use less of the display.
