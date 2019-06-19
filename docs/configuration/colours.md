# Custom Application Colour Configuration
The [colours.json](../../assets/configuration/colours.json) file is used to store custom UI colour values. Colour values should be provided as ARGB hexadecimal colour value strings.

## Chord Key Colours
Chord key colors are used by the [Component\::ChordPreview](../../Source/GUI/Component/Component_ChordPreview.h) and [Component\::ChordKeyDisplay](../../Source/GUI/Component/Component_ChordKeyDisplay.h) classes to display chord keys and chord previews.

#### Chord Colour Key Prefixes
For each chord key color type, a different color is defined for each chord key number and for unused key values.
 Key Prefix | Description
------------| ---
 "Chord1 "  | The start of a colour value representing to the first chord key.
 "Chord2 "  | The start of a colour value representing to the second chord key.
 "Chord3 "  | The start of a colour value representing to the third chord key.
 "Chord4 "  | The start of a colour value representing to the fourth chord key.
 "Chord5 "  | The start of a colour value representing to the fifth chord key.
 "Unused "  | The start of a colour value representing an unused key within a chord preview.

#### Chord Colour Key Types
For each chord key and chord preview, different colors are selected depending on which chord keys are held.
 Key          | Description
------------- | ---
 "(Selected)" | Colours used to represent the chord that would be entered if all chord keys were immediately released.
 "(Active)"   | Colours used to represent held keys in chords that aren't selected, but could be if additional chord keys were held down.
 "(Open)"     | Colours used to represent unheld keys in chords that aren't selected, but could be if additional chord keys were held down.
 "(Blocked)"  | Colours used to represent keys in chords that cannot be selected unless one or more held keys are released first.

## UI Element Category Colours
Besides chord key colors, each UI component colour is categorized using one of eleven different general UI element categories. The colours.json value contains colour values for each of these categories. If a UI element does not have a specific colour value assigned to it in the colours.json file, it takes the colour assigned to its category instead.

 Category Key             | Description
------------------------- | ---
 "Window Background"      | Contains background colour values for windows and other content panes.
 "Widget Background"      | Contains background colour values for miscellaneous widget components.
 "Widget"                 | Contains colour values for miscellaneous widget components.
 "Widget(off)"            | Contains colour values for widgets that are deactivated or in an "off" state.
 "Menu Background"        | Contains background colour values for menu components.
 "Outline"                | Contains outline colour values for unfocused UI components.
 "FocusedOutline"         | Contains outline colour values for focused UI components.
 "Text"                   | Contains colour values for displayed text elements.
 "Highlighted text"       | Contains colour values for highlighted text elements.
 "Text field"             | Contains colour values for text within an editable text field.
 "Highlighted text field" | Contains colour values for highlighted text within an editable text field.

## Other Colour Values
Colour Value Key           | Colour Category        | Description
-------------------------- | ---------------------- | ---
"Main Window Background"   | Window background      | The KeyChord window's background colour.
"Active chord key text"    | Text                   | The colour used to print characters when at least one of their chord keys are held down.
"Inactive Chord Text"      | Text                   | The colour used to print characters when none of their chord keys are held down.
"Input preview text"       | Text                   | The colour used to print buffered text input waiting to be sent to the targeted application window.
"Input preview background" | Text field             | The background colour shown behind the text input buffer component.
"Input preview outline"    | Outline                | The colour of the the outline drawn around the text input buffer component.
"Input preview highlight"  | Highlighted text field | The colour drawn behind text in the text input buffer component.
