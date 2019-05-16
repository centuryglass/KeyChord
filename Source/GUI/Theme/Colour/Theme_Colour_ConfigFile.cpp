#include "Theme_Colour_ConfigFile.h"
#include "Theme_Colour_JSONResource.h"
#include "Theme_Colour_ColourIds.h"
#include "Theme_Colour_JSONKeys.h"

namespace ColourTheme = Theme::Colour;

ColourTheme::ConfigFile::ConfigFile() { }


// Look up the Colour value saved for a specific JUCE ColourId.
juce::Colour ColourTheme::ConfigFile::getColour(const int colourId)
{
    using namespace JSONKeys;
    juce::String colourStr = getColourString(getColourKey(colourId));
    if (colourStr.isEmpty())
    {
        // No specific value set, look up the UICategory value.
        return getColour(getUICategory(colourId));
    }
    return juce::Colour(colourStr.getHexValue32());
}


// Gets the Colour value assigned as the default for all UI items in a specific
// category.
juce::Colour ColourTheme::ConfigFile::getColour(UICategory category)
{
    if (category == UICategory::none)
    {
        return juce::Colour();
    }
    return getColour(JSONKeys::getCategoryKey(category));
}


// Gets the colour value associated with a particular key string.
juce::Colour ColourTheme::ConfigFile::getColour
(const juce::Identifier& colourKey)
{
    juce::String colourStr = getColourString(colourKey);
    if (colourStr.isEmpty())
    {
        // No specific value set, look up the UICategory value.
        const int colourId = JSONKeys::getColourId(colourKey);
        if (colourId == -1)
        {
            return juce::Colour();
        }
        return getColour(JSONKeys::getUICategory(colourId));
    }
    return juce::Colour(colourStr.getHexValue32());
}


// Sets the saved colour value for a single UI element.
void ColourTheme::ConfigFile::setColour
(const int colourId, const juce::Colour newColour)
{
    const juce::Identifier& colourKey = JSONKeys::getColourKey(colourId);
    if (colourKey != JSONKeys::invalidKey)
    {
        setColour(colourKey, newColour);
    }
}


// Sets the saved colour value for a category of UI elements.
void ColourTheme::ConfigFile::setColour
(const UICategory category, const juce::Colour newColour)
{
    if (category != UICategory::none)
    {
        setColour(JSONKeys::getCategoryKey(category), newColour);
    }
}


// Sets the saved colour value for a specific key string.
void ColourTheme::ConfigFile::setColour
(const juce::Identifier& colourKey, const juce::Colour newColour)
{
    setConfigValue<juce::String>(colourKey, newColour.toString());
}


// Gets a Colour string saved to the colour config file.
juce::String ColourTheme::ConfigFile::getColourString
(const juce::Identifier& colourKey)
{
    return getConfigValue<juce::String>(colourKey);
}
