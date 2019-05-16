# Layout Configuration File
The [layout.json](../../assets/configuration/layout.json) configuration file sets the positions and sizes of UI components that shouldn't be managed using a relative group layout. Application font sizes are also defined here.

## Font sizes
JUCE-Template projects use three font size keys, "small text", "medium text", and "large text". Font sizes may be defined as either a fraction of the smallest window dimension, or as a fixed size in pixels. In most circumstances, text components will use the largest of these font sizes that would still allow their text to fit within the component bounds. If none of the configurable sizes would fit, these values are ignored and an even smaller font size is selected automatically.

## Component Layouts
Component layouts each define a UI component's position and size. X-position and width are defined as fractions of the application window's width, and Y-position and height are defined as fractions of the window height. Layouts may not need to provide all values, as some components may override parts of the layout anyway.

Layout Key                  | Layout Values Used  | Description
--------------------------- | ------------------- | ---
"popup editor menu"         | width, height       | A popup editor component.
"left arrow button"         | x, y, width, height | The directional navigation button pointing left.
"right arrow button"        | x, y, width, height | The directional navigation button pointing right.
"up arrow button"           | x, y, width, height | The directional navigation button pointing up.
"down arrow button"         | x, y, width, height | The directional navigation button pointing down.
"loading spinner"           | width, height       | The animated loading spinner widget.
