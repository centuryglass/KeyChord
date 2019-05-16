#pragma once
/**
 * @file  Theme_Colour_Element.h
 *
 * @brief  Stores information about a configurable UI colour element.
 */

#include "Theme_Colour_UICategory.h"
#include "JuceHeader.h"

namespace Theme { namespace Colour { class Element; } }

/**
 * @brief  Stores the JUCE ColourId value, and general colour category of a UI
 *         colour element.
 */
class Theme::Colour::Element
{
public:
    /**
     * @brief  Constructs a colour element object for a JUCE ColourID value.
     *
     * @param colourId   The element's JUCE ColourID value.
     *
     * @param category   The UI element category that best matches this element.
     */
    Element(const int colourId,
            const UICategory category = UICategory::none);

    /**
     * @brief  Creates an empty colour element object.
     */
    Element();

    virtual ~Element() { }

    /**
     * @brief  Gets this object's JUCE ColourId value.
     *
     * @return  The object's ID, or zero if the object is empty.
     */
    int getColourId() const;

    /**
     * @brief  Gets this object's UI element category.
     *
     * @return  The UI element category that best matches this object, or
     *          UICategory::none if the object is empty.
     */
    UICategory getColourCategory() const;

private:
    // The element's JUCE ColourId value:
    int colourId;
    // The element's general UI element category
    UICategory category;
};
