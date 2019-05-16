#pragma once
/**
 * @file   Layout_Config_ComponentLayout.h
 *
 * @brief  Defines the position and size of a single UI component, relative to
 *         the application window bounds.
 */

#include "JuceHeader.h"

namespace Layout { namespace Component { class ConfigLayout; } }

/**
 * @brief  Defines the position and size of a single component within the
 *         application window.
 *
 *  All layout values are relative to the size of the main application window.
 * Width and x-coordinate are expressed as fractions of the window's width.
 * Height and y-coordinate are expressed as fractions of the window's height.
 *
 *  ConfigLayout objects are not required to provide all values for each
 * component, as some components may automatically determine some of their own
 * layout values. For example, a component that should always be centered in the
 * window does not need the layout to provide its position, only its width and
 * height.
 */
class Layout::Component::ConfigLayout
{
public:
    /**
     * @brief  Creates an empty layout object.
     */
    ConfigLayout();

    /**
     * @brief  Initializes the layout from JSON data.
     *
     * @param jsonObj  A dynamic object containing JSON data.
     */
    ConfigLayout(juce::DynamicObject* jsonObj);

    /**
     * @brief  Gets the layout's x-coordinate value.
     *
     * @param defaultValue  An optional default value to return if the
     *                      x-coordinate is undefined.
     *
     * @return  The layout's x-coordinate, expressed as the distance in pixels
     *          from the left edge of the application window.
     */
    int getX(const int defaultValue = -1) const;

    /**
     * @brief  Gets the layout's y-coordinate value.
     *
     * @param defaultValue  An optional default value to return if the
     *                      y-coordinate is undefined.
     *
     * @return  The layout's y-coordinate, expressed as the distance in pixels
     *          from the top edge of the application window.
     */
    int getY(const int defaultValue = -1) const;

    /**
     * @brief  Gets the layout's width value.
     *
     * @param defaultValue  An optional default value to return if the width is
     *                      undefined.
     *
     * @return  The layout's width, measured in pixels.
     */
    int getWidth(const int defaultValue = -1) const;

    /**
     * @brief  Gets the layout's height value.
     *
     * @param defaultValue  An optional default value to return if the height is
     *                      undefined.
     *
     * @return  The layout's height, measured in pixels.
     */
    int getHeight(const int defaultValue = -1) const;

    /**
     * @brief  Gets the component's x-coordinate as a fraction of the window's
     *         width.
     *
     * @param defaultValue  An optional default value to return if the
     *                      x-coordinate is not defined.
     *
     * @return              The relative x-coordinate value, or defaultValue if
     *                      the x-coordinate is not defined.
     */
    float getXFraction(const float defaultValue = 0) const;

    /**
     * @brief  Gets the component's y-coordinate as a fraction of the window's
     *         height.
     *
     * @param defaultValue  An optional default value to return if the
     *                      y-coordinate is not defined.
     *
     * @return              The relative y-coordinate value, or defaultValue if
     *                      the y-coordinate is not defined.
     */
    float getYFraction(const float defaultValue = 0) const;

    /**
     * @brief  Gets the component's width as a fraction of the window's width.
     *
     * @param defaultValue  An optional default value to return if the width is
     *                      not defined.
     *
     * @return              The relative width value, or defaultValue if the
     *                      width is not defined.
     */
    float getWidthFraction(const float defaultValue = 0) const;

    /**
     * @brief  Gets the component's height as a fraction of the window's
     *         height.
     *
     * @param defaultValue  An optional default value to return if the height is
     *                      not defined.
     *
     * @return              The relative height value, or defaultValue if the
     *                      height is not relative.
     */
    float getHeightFraction(const float defaultValue = 0) const;

    /**
     * @brief  Compares this layout with another layout object.
     *
     * @param rhs  Another component layout object to compare with this one.
     *
     * @return     Whether both settings objects define identical position and
     *             size values.
     */
    bool operator== (const ConfigLayout& rhs) const;

    /**
     * @brief  Packages the component layout into a juce::DynamicObject.
     *
     * @return  The converted component layout.
     */
    juce::DynamicObject * getDynamicObject() const;

private:
    /**
     * @brief  Identifies the four layout values held by the ConfigLayout and
     *         sets their indices.
     */
    enum class ValueType
    {
        xPos = 0,
        yPos = 1,
        width = 2,
        height = 3
    };

    /**
     * @brief  Gets a value defined by this layout as a fraction of the
     *         application window's size.
     *
     * @param type          The layout value type to access.
     *
     * @param defaultValue  A default value to return if the requested type is
     *                      not defined.
     *
     * @return              The relative value definition, or the default value
     *                      if undefined.
     */
    float getRelativeValue(const ValueType type,
            const float defaultValue = 0) const;

    /**
     * @brief  Gets a value defined by this layout as an absolute measurement
     *         in pixels.
     *
     * @param type          The layout value type to access.
     *
     * @param defaultValue  A default value to return if the requested type is
     *                      not defined.
     *
     * @return              The value's absolute measurement in pixels, or the
     *                      default value if undefined.
     */
    int getAbsoluteValue(const ValueType type,
            const int defaultValue = -1) const;

    /**
     * @brief  Given a layout value type, return the application window
     *         dimension used to find that value's size in pixels.
     *
     * @param layoutValueType  One of the four value types that make up a
     *                         component layout object.
     *
     * @return                 Either the width or the height in pixels of the
     *                         main application window.
     */
    static int getWindowDimension(const ValueType layoutValueType);

    /**
     * @brief  Stores a single value in the component layout.
     */
    struct LayoutValue
    {
        // The relative layout value:
        float value;
        // Whether the layout explicitly defines this value:
        bool defined = false;
    };

    // All layout values that may be defined:
    LayoutValue layoutValues [4];
};
