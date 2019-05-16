## Adding Configurable ColourId Values
Follow these steps to add customizable colours to UI component classes:

1. Within the class definition in the header file, add an `enum ColourId` declaring names for each different configurable colour type.
2. Find the next unused group of ColourId values.
    * Run the ColourId management script at `[ProjectDir]/project-scripts/ColourIdElementManager.pl`. 
    * Enter '2' to select *Find next open ID value*, and note the printed hex value. 
    * Enter 'q' to exit the script for now.
    * Back in the header file, set the first item in the new enum equal to the given hex value.

## Automatic ColourId Integration
The *ColourIdElementManager.pl* script can be used to automatically connect new ColourIds to default colour categories, and to assign, apply, and document configurable colour keys.

1. Run the ColourId management script again to integrate the new ID value(s) into the project's colour management.
    * Enter '1' to select *"Scan project for new ColourId enums"*.
    * Enter '1' to select *"Scan project Source directory"*.
    * If prompted to resolve a namespace/enum conflict, pick an option to resolve the conflict. If unsure, select '3' to make no changes.
    * If prompted to replace a cached name with an element name, select 'n' to keep using the cached name.
    * At some point you will be asked if you want to add a namespace for your new ColourIds, named after the module and class where the IDs were added. Enter 'y' to confirm.
    * After responding to all conflicts and name differences, and adding your new ColourId namespace, enter 'q' to return to the main menu.
2. Assign general UI Categories to all new colour elements.
    * From the ColourId script main menu, enter '5' to select *"Assign UI categories"*.
    * Enter '1' to select *"Assign categories to uncategorized Elements"*.
    * Enter '1' to assign categories in the default order.
    * You will be asked to assign a default colour category to each uncategorized ColourId element in the project. Select whatever option seems most appropriate for each of your new ColourId values. If asked to categorize element IDs you didn't add, enter '11' to leave the ID uncategorized.
    * Enter 'q' to return to the main menu.
3. Edit the new ColourId namespace.
    * Enter '3' to select *"Edit Element namespaces"*.
    * Find the ColourId namespace you just added in the list, and enter its number to select it.
    * If you want to change the name of the new ColourId namespace, enter '1', then enter the new namespace name.
4. Edit individual colour elements in the namespace.
    * From the namespace menu, enter the menu number of the Element you will edit.
    * If you want to change the element's name, enter '1', then enter the new element name.
    * To allow the user to provide a specific custom colour value for the element, enter '4', then enter an appropriate key to represent the element.
    * To set a default colour value to override the element's category colour, enter '5', then enter an eight digit ARGB hexadecimal colour code. To provide a default colour, the element must also have an assigned key.
    * When finished editing the element, enter 'q' to return to the namespace menu. If all desired changes have been made to all elements in the namespace, enter 'q' twice more to return to the main menu.
5. Save all changes to the project's ColourId namespace file, JSON colour key file, default colour configuration file, and colour key documentation table.
    * Enter '7' to select *"Update project files"*.
    * Enter '1' to select *"Update all colour files"*.
    * For each new colour key, enter a brief sentence describing its purpose when prompted.

### Integrating ColourIds Manually
If you prefer not to update IDs with the perl update script, you can instead manually change all relevant files.

1. Edit `[ProjectDir]/Source/GUI/Theme/Colour/Theme_Colour_ColourIds.h`.
    * When adding ID values to a Component class, find its namespace block within the Theme::Colour::ColourIds namespace, or create one if it doesn't already exist.
    * Within the class ID namespace, ensure that a matching `static const Element idName(idValue, categoryType)` is declared for each ColourID value. 'idName' and 'idValue' should match the name and ID declared in the class ColourIds enum, and 'categoryType' should be an appropriate Theme::Colour::UICategory value.
    * At the bottom of the file, ensure that addresses of all added Element objects have been added to the `allElements` list.  
2. Edit `[ProjectDir]/Source/GUI/Theme/Colour/Theme_Colour_JSONKeys.cpp`.
    * Find the keyMap declaring all configurable ColourId keys. For each new ID that should be configurable, add `{"New key name, &ClassNamespace::elementName}` to the list, substituting in an appropriate key name and element address.
3. Edit `[ProjectDir]/assets/configuration/colours.json`.
    * Add each new ColourId key to the default configuration file as a string key.
    * For each new key, either provide a default colour value to use, or assign the empty string to use the default category colour.
4. Edit `[ProjectDir]/docs/configuration/colours.md`.
    * Add each new ColourId key to the table at the bottom of the file.
    * Fill in the 'Category' and 'Description' table values for each new key. Descriptions should be a brief explanation of how the colour value will be used.

### Using ColourIds within Component classes
Ensure ColourIds are actually used to set the component class colours.

1. In the constructor, override any class IDs that should be replaced with new ID values.
    * Call `setColour(ParentClass::parentId, findColour(replacementId))` for each parent ID value that should be replaced with a new ID.
    * Apply IDs to child components that use their own ID values by calling `childComponent.setColour(ChildClass::childId, findColour(replacementId))` for each relevant ID in each relevant child component.
2. If the class provides its own paint function, load and apply colours from ID values when painting the component.
    * Use `findColour(colourIDValue)` to load specific colours as juce::Colour objects. 
    * Apply these colours to the JUCE graphics context using `juce::Graphics::setColour(Colour newColour)` before using it to draw relevant Component parts.
