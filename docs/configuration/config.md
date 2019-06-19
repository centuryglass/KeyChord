
# KeyChord General Configuration
The [config.json](../../assets/configuration/config.json) file stores the state the application will be in when launched. All values currently defined here may be only be set to true or false. Changing the application state using bound keys will update these values, so that they always reflect the last state of the application.

If any miscellaneous configuration values are added to Keychord later, they will be defined in config.json and documented on this page.
Key             | Description
--------------- | ---
"minimized"     | Whether the KeyChord window starts in minimized mode, where the window is smaller and doesn't show chord previews.
"snapToBottom"  | Whether the KeyChord window should be placed on the bottom edge of the display or the top.
"immediateMode" | Whether KeyChord should immediately send all typed chord values to the targeted window, or buffer them until the send key is pressed.
