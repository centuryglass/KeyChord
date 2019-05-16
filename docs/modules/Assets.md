# Assets Module Documentation
The Assets module locates and loads application asset files. Default asset files are stored in /usr/share/[project-name], but custom asset files may be loaded from other directories.

#### [Assets](../../Source/Files/Assets/Assets.h)
Assets provides functions to find and loads application asset files, preferring matches found in the default application data directory. Assets provides functions to load generic file objects, juce\::Image objects from image files, or juce\::var objects from JSON files.

#### [Assets\::XDGDirectories](../../Source/Files/Assets/Assets_XDGDirectories.h)
XDGDirectories follows the XDG base directory specification to determine the most appropriate directories to use when locating or saving different file types.

#### [Assets\::JSONFile](../../Source/Files/Assets/Assets_JSONFile.h)
JSONFile objects read from and write to a single JSON data file. All file data access is type checked.

#### [Assets\::XPMLoader](../../Source/Files/Assets/Assets_XPMLoader.h)
XPMLoader loads XPM image files into juce\::Image objects.



