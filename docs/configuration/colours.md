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

## Image Colour Values
Image0, Image1, Image2, Image3, and Image4 define default colour values to replace in images with configurable colours. When a list of custom colours is applied to an image, the first custom colour replaces all pixels in the source image set to the colour value defined by Image0, the second custom colour replaces all pixels set to the colour value defined by Image1, and so on. Changing these values will prevent custom colours from being applied properly to default image assets, so in most circumstances they shouldn't be altered.

## Other Colour Values
Colour Value Key                          | Colour Category        | Description
----------------------------------------- | ---------------------- | ---
"Text button(off) background"             | Widget background      | The background colour of deactivated text buttons.
"Text button background"                  | Widget                 | The background colour of normal text buttons.
"Label background"                        | Window background      | The fill colour used behind text label components.
"Label text"                              | Text                   | The colour used to draw label text.
"Label outline"                           | Outline                | The colour of rectangular outline to draw around text labels.
"Combo box text"                          | Text                   | The colour to draw text that's labeling a combo box.
"Combo box background"                    | Menu background        | The colour filled in behind combo box components.
"Combo box outline"                       | Outline                | The colour of outline to draw around a combo box.
"Combo box arrow"                         | Text                   | The colour to draw the arrow on the right side of the combo box.
"Combo box focused outline"               | Focused outline        | The colour of outline to draw around the combo box while it is focused.
"Slider background"                       | Widget background      | The colour of the background drawn behind sliders.
"Slider thumb"                            | Widget                 | The colour of the oval or circle on the slider that is dragged to change the slider's position or value.
"Slider track"                            | Widget (off)           | The colour of the track representing the full range of a slider component.
"Alert window background"                 | Window background      | The background colour used by popup alert windows.
"Alert window text"                       | Text                   | The text colour used to draw popup alert window text.
"Alert window outline"                    | Focused outline        | The colour of the outline drawn around popup alert windows.
"Image0"                                  | none                   | First image colour to replace with a custom colour.
"Image1"                                  | none                   | Second image colour to replace with a custom colour.
"Image2"                                  | none                   | Third image colour to replace with a custom colour.
"Image3"                                  | none                   | Fourth image colour to replace with a custom colour.
"Image4"                                  | none                   | Fifth image colour to replace with a custom colour.
"List editor background"                  | Menu background        | The background colour filled in behind the list editor component.
"List editor item"                        | Text field             | The background fill colour for individual unselected items in the list editor.
"List editor selected item"               | Highlighted text field | The background fill colour for the selected item in the list editor.
"List editor text"                        | Text                   | The colour used to draw list editor text.
"File selection window"                   | Window background      | The background colour used by the file selection window.
"File selection text"                     | Text                   | The colour used to draw the text on the file selection window.
"Overlay spinner background"              | Widget background      | The background colour filled in behind the full-screen loading spinner overlay.
"Overlay spinner text"                    | Text                   | The colour used to draw the loading text on the loading spinner overlay.
"Page background"                         | Window background      | The colour drawn behind application pages.
"Colour picker checkerboard(light)"       | Widget                 | The light colour used in the checkerboard pattern drawn behind colour picker colours.
"Colour picker checkerboard(dark)"        | Widget (off)           | The dark colour used in the checkerboard pattern drawn behind colour picker colours.
"Colour picker colour outline"            | Outline                | The colour used to draw outlines around colour picker buttons.
"Colour picker focused colour outline"    | Focused outline        | The colour used to draw outlines around focused colour picker buttons.
"Switch background"                       | Widget background      | The fill colour used by the oval background of the switch component.
"Switch handle"                           | Widget                 | The fill colour used for the handle on the switch component when the switch is on.
"Switch handle(off)"                      | Widget (off)           | The fill colour used for the handle on the switch component when the switch is off.
