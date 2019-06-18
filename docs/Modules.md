## KeyChord Modules

KeyChord divides its functionality into modules. Each Module has its own source directory, makefile, and namespace.

---
### Main
The main application code files responsible for starting up and shutting down KeyChord.

#### [Main](../Source/Main.cpp)
Immediately starts up the JUCE Application object when the program is started.

#### [Application](../Source/Application.cpp)
Initializes the program on launch, shuts everything down on program termination, and provides interfaces for controlling the main application window or closing the application.

#### [MainWindow](../Source/MainWindow.cpp)
Initializes and represents the application's window.

### GUI
Modules that create the application's user interface.

#### [Component](./modules/Component.md)
Provides all UI element classes displayed within the application's window.

#### [Input](./modules/Input.md)
Processes user input, using it to control the application or passing it to another application.

#### [Output](./modules/Output.md)
Buffers and sends key data to another application.

#### [Text](./modules/Text.md)
Manages the keyboard's configurable character sets and text rendering capabilities.

#### [Theme](./modules/Theme.md)
Controls the general appearance of the application. This module is responsible for selecting configurable UI colours.

---
### Files
Modules that find, manage, and process different types of files.

#### [Assets](./modules/Assets.md)
Locates and loads application asset files.

#### [Config](./modules/Config.md)
Reads data from the user's configuration files, writes changes back to those files, and shares configuration data and updates with other modules.

---
### Framework Modules
Modules used to design and build other modules.

#### [Locale](./modules/Locale.md)
Provides localized display text throughout the application.

#### [SharedResource](./modules/SharedResource.md)
A system for safely creating, accessing, and destroying shared application resources.

#### [Windows](./modules/Windows.md)
Creates, finds, tracks, and controls open windows.

#### [Util](./modules/Util.md)
Miscellaneous support classes and utility functions.

---
### Development:
Modules used for testing and improving application design.

#### [Testing](./modules/Testing.md)
Tools used to build application test classes.

#### [Debug](./modules/Debug.md)
Tools for testing application performance and locating errors.

