#include "Layout_Component_ConfigLayout.h"
#include "Windows_Info.h"


/**
 * @brief  Provides the keys used to load each stored value from JSON.
 */
static const juce::Identifier valueKeys [4] =
{
    "x",
    "y",
    "width",
    "height"
};


Layout::Component::ConfigLayout::ConfigLayout() { }


// Initializes the layout from JSON data.
Layout::Component::ConfigLayout::ConfigLayout(juce::DynamicObject* jsonObj)
{
    for (int i = 0; i < 4; i++)
    {
        if (jsonObj->hasProperty(valueKeys[i]))
        {
            layoutValues[i].value = jsonObj->getProperty(valueKeys[i]);
            layoutValues[i].defined = true;
        }
    }
}


// Gets the layout's x-coordinate value.
int Layout::Component::ConfigLayout::getX(const int defaultValue) const
{
    return getAbsoluteValue(ValueType::xPos, defaultValue);
}


// Gets the layout's y-coordinate value.
int Layout::Component::ConfigLayout::getY(const int defaultValue) const
{
    return getAbsoluteValue(ValueType::yPos, defaultValue);
}


// Gets the layout's width value.
int Layout::Component::ConfigLayout::getWidth(const int defaultValue) const
{
    return getAbsoluteValue(ValueType::width, defaultValue);
}


// Gets the layout's height value.
int Layout::Component::ConfigLayout::getHeight(const int defaultValue) const
{
    return getAbsoluteValue(ValueType::height, defaultValue);
}


// Gets the component's x-coordinate as a fraction of the window's width.
float Layout::Component::ConfigLayout::getXFraction
(const float defaultValue) const
{
    return getRelativeValue(ValueType::xPos, defaultValue);
}


// Gets the component's y-coordinate as a fraction of the window's height.
float Layout::Component::ConfigLayout::getYFraction
(const float defaultValue) const
{
    return getRelativeValue(ValueType::yPos, defaultValue);
}


// Gets the component's width as a fraction of the window's width.
float Layout::Component::ConfigLayout::getWidthFraction
(const float defaultValue) const
{
    return getRelativeValue(ValueType::width, defaultValue);
}


// Gets the component's height as a fraction of the window's height.
float Layout::Component::ConfigLayout::getHeightFraction
(const float defaultValue) const
{
    return getRelativeValue(ValueType::height, defaultValue);
}


// Compares this layout with another layout object.
bool Layout::Component::ConfigLayout::operator==
(const ConfigLayout& rhs) const
{
    for (int i = 0; i < 4; i++)
    {
        if (layoutValues[i].defined != rhs.layoutValues[i].defined)
        {
            return false;
        }
        if (layoutValues[i].defined
                && layoutValues[i].value != rhs.layoutValues[i].value)
        {
            return false;
        }
    }
    return true;
}


// Packages the layout into a juce::DynamicObject.
juce::DynamicObject* Layout::Component::ConfigLayout::getDynamicObject() const
{
    juce::DynamicObject* componentObject = new juce::DynamicObject();
    for (int i = 0; i < 4; i++)
    {
        const LayoutValue& layoutValue = layoutValues[i];
        if (layoutValue.defined)
        {
            componentObject->setProperty(valueKeys[i], layoutValue.value);
        }
    }
    return componentObject;
}


// Gets a value defined by this layout as a fraction of the application window's
// size.
float Layout::Component::ConfigLayout::getRelativeValue
(const ValueType type, const float defaultValue) const
{
    const LayoutValue& v = layoutValues[(int) type];
    return v.defined ? v.value : defaultValue;
}


// Gets a value defined by this layout as an absolute measurement in pixels.
int Layout::Component::ConfigLayout::getAbsoluteValue
(const ValueType type, const int defaultValue) const
{
    const LayoutValue& v = layoutValues[(int) type];
    return v.defined ? (v.value * getWindowDimension(type)) : defaultValue;
}


// Given a layout value type, return the application window dimension used to
// find that value's size in pixels.
int Layout::Component::ConfigLayout::getWindowDimension
(const ValueType layoutValueType)
{
    const juce::Rectangle<int> windowBounds = Windows::Info::getBounds();
    if (layoutValueType == ValueType::xPos
            || layoutValueType == ValueType::width)
    {
        return windowBounds.getWidth();
    }
    else
    {
        return windowBounds.getHeight();
    }
}
