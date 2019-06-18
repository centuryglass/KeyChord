# Input Module Documentation
The Input module processes all user input, controlling how it is interpreted by the application or forwarded to the target window.

#### [Input\::Chord](../../Source/GUI/Input/Input_Chord.h)
Each Chord object represents a single input command created by holding down a combination of chord input keys.

#### [Input\::ChordReader](../../Source/GUI/Input/Input_ChordReader.h)
ChordReader processes all chord input events, tracking when chord entry keys are held down, and notifying registered ChordReader::Listener objects when the set of held chord keys changes, or a chord value is selected. ChordReader also passes on any key events unrelated to chord entry to its listeners without modification.

#### [Input\::Controller](../../Source/GUI/Input/Input_Controller.h)
Controller is a ChordReader\::Listener that determines how all keyboard input events should be used to control the application. It interacts with Component module objects to update the displayed input state, with the Output module to buffer and send text to the targeted application, and with the main Application object to move the window or close the application.

## Key Submodule
The Key submodule defines the set of keyboard keys used to control KeyChord. All key bindings are defined within the keyBindings.json configuration file.

#### [Input\::Key\::Binding](../../Source/GUI/Input/Key/Input_Key_Binding.h)
Binding objects represent a single association between a key input code and an assigned action. Each binding stores the juce::Identifier of its associated action, the JUCE key description used to detect the key input event, a name describing the key, and a single [Text\::CharValue](../../Source/GUI/Text/Text_Values.h) character used to represent the key.

#### [Input\::Key\::JSONKeys](../../Source/GUI/Input/Key/Input_Key_JSONKeys.h)
The JSONKeys namespace defines JUCE Identifier objects representing all application action types that may be mapped to key events.

#### [Input\::Key\::ConfigFile](../../Source/GUI/Input/Key/Input_Key_ConfigFile.h)
ConfigFile objects access the configuration file resource to load Binding objects for any of the actions defined in Input\::Key\::JSONKeys.

#### [Input\::Key\::JSONResource](../../Source/GUI/Input/Key/Input_Key_JSONResource.h)
JSONResource handles all direct access to the key binding configuration file, and ensures bindings are cached and available as long as they are needed.


