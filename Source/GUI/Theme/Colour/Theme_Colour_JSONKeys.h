#pragma once
/**
 * @file  Theme_Colour_JSONKeys.h
 *
 * @brief  Defines all UI colour keys used in the colours.json configuration
 *         file.
 */

#include "Theme_Colour_UICategory.h"
#include "JuceHeader.h"

namespace Theme { namespace Colour { namespace JSONKeys
{

    // Represents an invalid colour key
    static const juce::Identifier invalidKey("invalid!");

    /**
     * @brief  Gets all JUCE ColourId values defined by the colour config file.
     *
     * @return  All ColourIds defined in colours.json, either directly or
     *          through a UICategory.
     */
    juce::Array<int> getColourIds();

    /**
     * @brief  Gets all UI category colour keys.
     *
     * @return  Keys for all generic UI element category colours.
     */
    const juce::Array<juce::Identifier>& getCategoryKeys();

    /**
     * @brief  Gets all keys used by the ColourConfigFile.
     *
     * @return  All UICategory keys and ColourID keys.
     */
    juce::Array<const juce::Identifier*> getColourKeys();

    /**
     * @brief  Gets the UICategory assigned to a JUCE ColourId value.
     *
     * @param colourId  The ColourId value of some UI element.
     *
     * @return          The corresponding UICategory type, or UICategory::none
     *                  if no assigned UICategory was found.
     */
    Theme::Colour::UICategory getUICategory(const int colourId);

    //=================== Lookup values using keys: ===========================
    /**
     * @brief  Finds the JUCE ColourId value of a UI element from its key.
     *
     * @param colourKey  The key for a UI element colour.
     *
     * @return           The corresponding ColourId value, or -1 if colourKey
     *                   doesn't map to a UI element.
     */
    int getColourId(const juce::Identifier& colourKey);

    /**
     * @brief  Finds the UICategory type represented by a specific key.
     *
     * @param categoryKey  The key of a UICategory value.
     *
     * @return             The UICategory value associated with the key, or
     *                     UICategory::none if categoryKey is not a valid key.
     */
    Theme::Colour::UICategory getCategoryType
    (const juce::Identifier& categoryKey);

    //====================== Lookup keys by value: ===========================
    /**
     * @brief  Gets the key used to store a specific JUCE ColourId in the
     *         colour config file.
     *
     * @param colourId  The JUCE colourId value of a UI element.
     *
     * @return          The key for that colourId, or invalidKey if colourId is
     *                  not tracked in colours.json.
     */
    const juce::Identifier& getColourKey(const int colourId);

    /**
     * @brief  Finds the key string representing a UICategory value.
     *
     * @param category  One of the default UI element types.
     *
     * @return          The key used to store this category's value in the JSON
     *                  config file.
     */
    const juce::Identifier& getCategoryKey
    (const Theme::Colour::UICategory category);
} } }
