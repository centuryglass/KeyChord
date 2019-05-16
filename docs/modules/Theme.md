# Theme Module Documentation
The Theme module handles tasks related to the general appearance of the application. Its primary responsibility is loading UI images and colours from JSON configuration files.

#### [Theme\::LookAndFeel](../../Source/GUI/Theme/Theme_LookAndFeel.h)
LookAndFeel controls how the JUCE library draws UI components. This sets the application font and mouse cursor, and defines custom drawing routines for several UI components. LookAndFeel also loads and applies configurable UI colour values.

## Theme Colours
The Colour submodule loads and sets UI component colour values from the colours.json configuration file.

#### [Theme\::Colour\::JSONResource](../../Source/GUI/Theme/Colour/Theme_Colour_JSONResource.h)
JSONResource defines the [SharedResource](./SharedResource.md) class instance used to manage the colours.json file.

#### [Theme\::Colour\::ColourIds](../../Source/GUI/Theme/Colour/Theme_Colour_ColourIds.h)
The JUCE library assigns a unique ID to each UI component colour value. The ColourIds namespace provides all of these IDs in a single file.

#### [Theme\::Colour\::UICategory](../../Source/GUI/Theme/Colour/Theme_Colour_UICategory.h)
UICategory defines a set of broad UI component colour categories. Each JUCE ColourId value is assigned to one of these categories, and the JSONResource stores colour values for each category. If no specific colour is provided by the JSONResource for a given ColourId, it uses the colour provided by its UICategory.

#### [Theme\::Colour\::JSONKeys](../../Source/GUI/Theme/Colour/Theme_Colour_JSONKeys.h)
JSONKeys provides the list of all colour keys that should be present in the colours.json configuration file. It also converts between ColourId and JSON key values, and looks up the generic colour category assigned to any ColourId or JSON key.

#### [Theme\::Colour\::ConfigFile](../../Source/GUI/Theme/Colour/Theme_Colour_ConfigFile.h)
ConfigFile objects connect to the JSONResource to lookup or change colour values by ColourId value or JSON key.

#### [Theme\::Colour\::ConfigListener](../../Source/GUI/Theme/Colour/Theme_Colour_ConfigListener.h)
ConfigListener objects may track any number of ColourId values. Whenever a value a ConfigListener tracks is updated by the JSONResource, the listener will be notified via the ConfigListener\::colourChanged function.

#### [Theme\::Colour\::ListenerInterface](../../Source/GUI/Theme/Colour/Theme_Colour_ListenerInterface.h)
ListenerInterface is an abstract interface inherited by ConfigListener, used by the JSONResource to send updates to relevant listener objects when the colour values it stores are updated.

#### [Theme\::Colour\::ConfigPage](../../Source/GUI/Theme/Colour/Theme_Colour_ConfigPage.h)
ConfigPage provides UI controls for updating application colour values within a page component.

## Theme Images
The Image submodule loads image assets with optional customized colours from the imageAssets.json configuration file.

#### [Theme\::Image\::JSONResource](../../Source/GUI/Theme/Image/Theme_Image_JSONResource.h)
JSONResource defines the [SharedResource](./SharedResource.md) class instance used to manage the imageAssets.json file.

#### [Theme\::Image\::AssetList](../../Source/GUI/Theme/Image/Theme_Image_AssetList.h)
AssetList objects store a set of image paths and custom image colours loaded from the JSON configuration file. Each AssetList object stores image paths and colours for a single UI component.

#### [Theme\::Image\::JSONKeys](../../Source/GUI/Theme/Image/Theme_Image_JSONKeys.h)
JSONKeys provides all JSON key values used to load individual image files and component image asset lists from the imageAssets.json file.

#### [Theme\::Image\::ConfigFile](../../Source/GUI/Theme/Image/Theme_Image_ConfigFile.h)
ConfigFile objects connect to the JSONResource to load AssetList objects for specific components, get individual image paths defined in the imageAssets file, or to change the image path values saved for specific images.

#### [Theme\::Image\::ConfigListener](../../Source/GUI/Theme/Image/Theme_Image_ConfigListener.h)
ConfigListener objects track any number of individual image keys defined in JSONKeys. These objects are notified by the JSONResource whenever an image path they track is updated.

#### [Theme\::Image\::Component](../../Source/GUI/Theme/Image/Theme_Image_Component.h)
Component is a template class used to create subclasses of [Widgets\::DrawableImage](../../Source/GUI/Widgets/Widgets_DrawableImage.h) that use image assets and colours provided by an Image\::AssetList.
