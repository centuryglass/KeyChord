#pragma once
/**
 * @file  Layout_Component_TextSize.h
 *
 * @brief  Represents the three main text size options defined by the
 *         Layout::Component::JSONResource.
 */

namespace Layout { namespace Component { enum class TextSize; } }

/**
 * @brief  Represents the three main text size options.
 *
 *  The actual size of each of these options is set in layout.json, either as a
 * fraction of the smallest window dimension(if textSize <= 1) or as a fixed
 * height in pixels(if textSize > 1).
 */
enum class Layout::Component::TextSize
{
    smallText,
    mediumText,
    largeText
};
