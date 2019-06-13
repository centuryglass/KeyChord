# Component Module Documentation
The Component module provides classes that create all interface elements displayed within the application.

#### [Component\::MainView](../../Source/GUI/Component/Component_MainView.h)
MainView holds and arranges all other Component objects within the application's window.

#### [Component\::KeyGrid](../../Source/GUI/Component/Component_KeyGrid.h)
KeyGrid provides a basis for Component classes that react to user input by drawing text. KeyGrid uses the [Text](./Text.md) module to store and render text.

#### [Component\::CharsetDisplay](../../Source/GUI/Component/Component_CharsetDisplay.h)
CharsetDisplay is a KeyGrid class that displays all characters in the active character set, highlighting the selected character.

#### [Component\::ChordKeyDisplay](../../Source/GUI/Component/Component_ChordKeyDisplay.h)
ChordKeyDisplay is a KeyGrid class that represents the application's [Chord](../../Source/GUI/Input/Input_Chord.h) input keys, highlighting the ones that are currently held down.

#### [Component\::ChordPreview](../../Source/GUI/Component/Component_ChordPreview.h)
ChordPreview is a KeyGrid class that displays the Chord key combination for each character in the active character set, highlighting selected or partially selected chords.

#### [Component\::InputView](../../Source/GUI/Component/Component_InputView.h)
InputView is a KeyGrid class that displays all input recorded by KeyChord that is waiting to be sent to the target window.

#### [Component\::HelpScreen](../../Source/GUI/Component/Component_HelpScreen.h)
HelpScreen is a KeyGrid class that displays the key bindings used to control KeyChord.
