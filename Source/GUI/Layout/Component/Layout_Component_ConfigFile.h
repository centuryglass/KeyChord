#pragma once
/**
 * @file  Layout_Component_ConfigFile.h
 *
 * @brief  Gets and sets the component settings held by the
 *         Layout::Component::JSONResource.
 */

#include "Config_FileHandler.h"
#include "Layout_Component_TextSize.h"

namespace Layout { namespace Component { class ConfigFile; } }
namespace Layout { namespace Component { class JSONResource; } }
namespace Layout { namespace Component { class ConfigLayout; } }

/**
 * @brief  Reads and writes settings defined in
 *         Layout::Component::JSONResource's configurable JSON file.
 *
 *  This includes individual component sizes and positions, and application font
 * sizes.
 */
class Layout::Component::ConfigFile : public Config::FileHandler<JSONResource>
{
public:
    ConfigFile();

    virtual ~ConfigFile() { }

    /**
     * @brief  Gets configured component layouts from shared JSON file data.
     *
     * @param componentKey  A configurable UI component's key string.
     *
     * @return              The layout defined for that component.
     */
    ConfigLayout getLayout(const juce::Identifier& componentKey);

    /**
     * @brief  Returns the most appropriate font size for drawing text.
     *
     * @param textBounds  The area in which the text will be drawn.
     *
     * @param text        The actual text being drawn.
     *
     * @return            Whichever font height(small, medium, or large)
     *                    defined in components.json would best fit this text
     *                    within its bounds, or the largest height that will fit
     *                    if even the small font is too large to fit.
     */
    int getFontHeight(juce::Rectangle <int> textBounds, juce::String text);

    /**
     * @brief  Gets the height in pixels of one of the three configured text
     *         sizes.
     *
     * @param sizeType  A text size type defined in the Component config file.
     *
     * @return          The height in pixels of that text size type.
     */
    int getFontHeight(TextSize sizeType);
};
