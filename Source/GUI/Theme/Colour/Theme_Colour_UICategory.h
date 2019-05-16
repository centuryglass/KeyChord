#pragma once
/**
 * @file  Theme_Colour_UICategory.h
 *
 * @brief  Defines broad categories of UI element types used to assign default
 *         colour values.
 */

namespace Theme { namespace Colour { enum class UICategory; } }

/**
 * @brief  All JUCE ColourId values used by this program are assigned to one of
 *         these categories.
 */
enum class Theme::Colour::UICategory
{
    windowBackground = 0,
    widgetBackground,
    widgetOn,
    widgetOff,
    menuBackground,
    outline,
    focusedOutline,
    text,
    highlightedText,
    textField,
    highlightedTextField,
    none
};
