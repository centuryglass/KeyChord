#include "Theme_Colour_Element.h"

// Constructs a colour element object for a JUCE ColourID value.
Theme::Colour::Element::Element
(const int colourId, const UICategory category) :
    colourId(colourId), category(category) { }


// Creates an empty colour element object.
Theme::Colour::Element::Element() :
    colourId(0), category(UICategory::none) { }


// Gets this object's JUCE ColourId value.
int Theme::Colour::Element::getColourId() const
{
    return colourId;
}


// Gets this object's UI element category.
Theme::Colour::UICategory Theme::Colour::Element::getColourCategory() const
{
    return category;
}
