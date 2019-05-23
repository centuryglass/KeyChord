# Custom Application Colour Configuration
The [colours.json](../../assets/configuration/colours.json) file is used to store custom UI colour values. Colour values should be provided as ARGB hexadecimal colour value strings.

## UI Element Category Colours
Each UI component colour is categorized using one of eleven different general UI element categories. The colours.json value contains colour values for each of these categories. If a UI element does not have a specific colour value assigned to it in the colours.json file, it takes the colour assigned to its category instead.

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
Colour Value Key         | Colour Category   | Description
------------------------ | ----------------- | ---
"WindowBackground"       | Window background | The application window background colour.
"Chord Text"             | Text              | Default text color for UI labels.
"Inactive Chord Text"    | Text              | Text color for keys that conflict with the current held chord.
"Chord1 (Selected)"      | none              | 
"Chord2 (Selected)"      | none              | 
"Chord3 (Selected)"      | none              | 
"Chord4 (Selected)"      | none              | 
"Chord5 (Selected)"      | none              | 
"Chord1 (Active)"        | none              | 
"Chord2 (Active)"        | none              | 
"Chord3 (Active)"        | none              | 
"Chord4 (Active)"        | none              | 
"Chord5 (Active)"        | none              | 
"Chord1 (Open)"          | none              | 
"Chord2 (Open)"          | none              | 
"Chord3 (Open)"          | none              | 
"Chord4 (Open)"          | none              | 
"Chord5 (Open)"          | none              | 
"Chord1 (Blocked)"       | none              | 
"Chord2 (Blocked)"       | none              | 
"Chord3 (Blocked)"       | none              | 
"Chord4 (Blocked)"       | none              | 
"Chord5 (Blocked)"       | none              | 
"Empty Chord"            | none              | 
"Active Modifier Text"   | none              | 
"Active Modifier Fill"   | none              | 
"Active Modifier Line"   | none              | 
"Inactive Modifier Text" | none              | 
"Inactive Modifier Fill" | none              | 
"Inactive Modifier Line" | none              | 
REMOVED:REMOVED:REMOVED:"Chord1(Active)"      | none              | First chord button, selected.
REMOVED:REMOVED:REMOVED:"Chord2(Active)"      | none              | Second chord button, selected.
REMOVED:REMOVED:REMOVED:"Chord3(Active)"      | none              | Third chord button, selected.
REMOVED:REMOVED:REMOVED:"Chord4(Active)"      | none              | Fourth chord button, selected.
REMOVED:REMOVED:REMOVED:"Chord5(Active)"      | none              | Fifth chord button, selected.
REMOVED:REMOVED:REMOVED:"Chord1(Open)"        | none              | 
REMOVED:REMOVED:REMOVED:"Chord2(Open)"        | none              | 
REMOVED:REMOVED:REMOVED:"Chord3(Open)"        | none              | 
REMOVED:REMOVED:REMOVED:"Chord4(Open)"        | none              | 
REMOVED:REMOVED:REMOVED:"Chord5(Open)"        | none              | 
REMOVED:REMOVED:REMOVED:"Chord1(Blocked)"     | none              | 
REMOVED:REMOVED:REMOVED:"Chord2(Blocked)"     | none              | 
REMOVED:REMOVED:REMOVED:"Chord3(Blocked)"     | none              | 
REMOVED:REMOVED:REMOVED:"Chord4(Blocked)"     | none              | 
REMOVED:REMOVED:REMOVED:"Chord5(Blocked)"     | none              | 