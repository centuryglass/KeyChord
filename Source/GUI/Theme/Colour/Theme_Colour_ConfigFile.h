#pragma once
/**
 * @file  Theme_Colour_ConfigFile.h
 *
 * @brief  Gets and sets all UI colour values defined in the colours.json
 *         configuration file.
 */

#include "Config_FileHandler.h"
#include "Theme_Colour_JSONKeys.h"


namespace Theme
{
    namespace Colour
    {
        class ConfigFile;
        class JSONResource;
    }
}

/**
 * @brief  A FileHandler object class that provides access to the application's
 *         shared colour configuration resource.
 */
class Theme::Colour::ConfigFile : public Config::FileHandler<JSONResource>
{
public:
    ConfigFile();

    virtual ~ConfigFile() { }

    /**
     * @brief  Looks up the Colour value saved for a specific JUCE ColourId.
     *
     * @param colourId  The ID value representing the colour of a specific UI
     *                  element.
     *
     * @return          The Colour value saved to the colours.json file for the
     *                  given colourId. If no Colour is set for that specific
     *                  id, the Colour for the UICategory associated with that
     *                  id will be returned. If colourId is not a valid id,
     *                  Colour() will be returned.
     */
    juce::Colour getColour(const int colourId);

    /**
     * @brief  Gets the Colour value assigned as the default for all UI items
     *         in a specific category.
     *
     * @param category  One of the categories of UI elements.
     *
     * @return          The default colour value used by all items in that
     *                  category that don't have a colour value assigned to
     *                  them specifically.
     */
    juce::Colour getColour(const UICategory category);

    /**
     * @brief  Gets the colour value associated with a particular key string.
     *
     * @param colourKey  The key of a Colour saved in the ColourConfigFile
     *
     * @return           The saved Colour, or Colour() if the colour key isn't
     *                   found.
     */
    juce::Colour getColour(const juce::Identifier& colourKey);

    /**
     * @brief  Sets the saved colour value for a single UI element.
     *
     * @param colourId   A JUCE ColourId value tracked by the colour config
     *                   file.
     *
     * @param newColour  The colour value to assign to the ColourId.
     */
    void setColour(const int colourId, const juce::Colour newColour);

    /**
     * @brief  Sets the saved colour value for a category of UI elements.
     *
     * @param category   The category to assign the new colour value.
     *
     * @param newColour  The colour value to assign to the category.
     */
    void setColour(const UICategory category, const juce::Colour newColour);

    /**
     * @brief  Sets the saved colour value for a specific key string.
     *
     * @param colourKey  A colour key string tracked by the colour
     *                   configuration file..
     *
     * @param newColour  The new colour value to save to the file.
     */
    void setColour(const juce::Identifier& colourKey,
            const juce::Colour newColour);

private:
    /**
     * @brief  Gets a Colour string saved to the colour config file.
     *
     * @param colourKey  The key to a colour value saved in the colours.json
     *                   file.
     *
     * @return           The colour hex string mapped to that key, or the empty
     *                   string if the key isn't found.
     */
    juce::String getColourString(const juce::Identifier& colourKey);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConfigFile)
};
