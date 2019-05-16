# Adding and Updating Configuration Files
The [Config module](../modules/Config.md) creates and manages application configuration files. This guide explains all necessary steps when adding new configuration files, or adding values to existing files. For adding new values to the colour configuration file, use the [ColourId update guide](./AddColourIds.md) instead.

### Adding a New Configuration File
1. Create a default version of the configuration file in `[ProjectDir]/assets/configuration`. Use the [example default file](../templates/config/example.json) as a template.
2. Create the file's key definition header in an appropriate module, following the [example key header template](../templates/config/ExampleKeys.h).
3. Create the internal configuration resource class, following the [ExampleResource header](../templates/config/ExampleResource.h) and [source](../templates/config/ExampleResource.cpp) templates.
4. Create the configuration resource handler class, following the [ExampleFile header](../templates/config/ExampleFile.h) and [source](../templates/config/ExampleFile.cpp) templates.
5. Create the configuration file listener class, following the [ExampleListener header](../templates/config/ExampleListener.h) and [source](../templates/config/ExampleListener.cpp) templates.
6. Create any necessary classes for storing JSON object data, using the [ExampleObject header](../templates/config/ExampleObject.h) and [source](../templates/config/ExampleObject.cpp) files as examples.
7. Add all new source files to the appropriate module makefile in `[ProjectDir]/makefiles`.
8. Document all new header files in the appropriate module documentation in `[ProjectDir]/docs/modules`.
9. Create a new markdown file in `[ProjectDir]/docs/configuration` documenting all options set by the new JSON file.

### Adding New Values to Existing Files
1. Use the [configuration file guides](../Configuration.md) to select an appropriate file to update.
2. Find the appropriate default JSON file in `[ProjectDir]/assets/configuration`, and add keys and default values for all new values.
3. Find the file's JSON key declaration file, and add all new key values. Object and array value keys should be added as `static const juce::Identifier` objects. All other data types should be added as static const [Config::DataKey](../../Source/Files/Config/Config_DataKey.h) objects. 
4. Make sure that all new keys are also added to any relevant key list arrays within the JSON key declaration file.
5. If adding object or array values, update the file's FileResource class to read and write the new values.
    * Update the Resource object's constructor to use `initProperty<juce::var>(arrayKey)` or `initProperty<juce::DynamicObject*>(objectKey)` to load all new array or object values, and have the Resource class cache them within private juce::Array or custom object data members.
    * Create or update the Resource object's `isValidKey(testKey)` function to return true when any valid key is tested.
    * Add functions to the Resource class for getting and (optionally) setting cached object or array data.
    * Create or update the Resource object's `writeDataToJSON()` function to copy cached data back to the JSON file using the Resource's `updateProperty<juce::var>(arrayKey, varArray)` or `updateProperty<juce::DynamicObject*>(objectKey, dynamicObject)` functions.
6. Update the file's FileHandler class with methods to read and (optionally) write each new value type.
