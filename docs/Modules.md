## JUCE Template Modules

The JUCE Project Template divides its functionality into modules. Each Module has its own source directory, makefile, and namespace.

---
### GUI
Modules that create the application's user interface.

#### [Layout](./modules/Layout.md)
Arranged UI components within the window and animates transitions between different layouts.

#### [Widgets](./modules/Widgets.md)
Basic UI components that may be reused throughout the application.

#### [Page](./modules/Page.md)
Manages the layout of all UI pages, and handles transitions between those pages.

#### [Theme](./modules/Theme.md)
Controls the general appearance of the application in all ways unrelated to layout. This module is responsible for selecting configurable UI image assets and colours.

---
### Files
Modules that find, manage, and process different types of files.

#### [Assets](./modules/Assets.md)
Locates and loads application asset files.

#### [Config](./modules/Config.md)
Reads data from the user's configuration files, writes changes back to those files, and shares configuration data and updates with other modules.

---
### System Modules
Modules for interacting with system applications, hardware, and the operating system.

#### [Process](./modules/Process.md)
Creates and manages new child processes.

#### [Hardware](./modules/Hardware.md)
Monitors and controls simpler system hardware devices.

#### [Windows](./modules/Windows.md)
Creates, finds, tracks, and controls open windows.

---
### Framework Modules
Modules used to design and build other modules.

#### [Locale](./modules/Locale.md)
Provides localized display text throughout the application.

#### [SharedResource](./modules/SharedResource.md)
A system for safely creating, accessing, and destroying shared application resources.

#### [Util](./modules/Util.md)
Miscellaneous support classes and utility functions.

---
### Development:
Modules used for testing and improving application design.

#### [Testing](./modules/Testing.md)
Tools used to build application test classes.

#### [Debug](./modules/Debug.md)
Tools for testing application performance and locating errors.

