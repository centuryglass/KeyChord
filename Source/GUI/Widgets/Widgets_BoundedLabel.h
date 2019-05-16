#pragma once
/**
 * @file  Widgets_BoundedLabel.h
 *
 * @brief  Provides a Label component with an optional maximum text size and
 *         component padding height.
 */

#include "JuceHeader.h"
#include "Layout_Component_TextSize.h"

namespace Widgets { class BoundedLabel; }

/**
 * @brief  A juce Label with additional controls for setting text size.
 */
class Widgets::BoundedLabel : public juce::Label
{
public:
    /**
     * @brief  Creates the label with optional initial text and padding values.
     *
     * @param componentName  Sets the component's internal name value.
     *
     * @param labelText      Sets the initial label display text.
     *
     * @param fontPadding    Sets the minimum number of pixels to leave between
     *                       the text top and bottom and the label edges.
     */
    BoundedLabel(const juce::String componentName = juce::String(),
            const juce::String labelText = juce::String(),
            const int fontPadding = 0);

    virtual ~BoundedLabel() { }

    /**
     * @brief  Sets the maximum height of the label text.
     *
     * @param newMaximum  One of the text height values set through
     *                    Layout::Component::ConfigFile.
     */
    void setMaximumTextSize(const Layout::Component::TextSize newMaximum);

private:
    /**
     * @brief  Updates the font size when label bounds change.
     */
    void resized() override;

    // Pixels of vertical space to leave between text and component bounds:
    int fontPadding = 0;

    // Maximum text height setting:
    Layout::Component::TextSize maxSize
            = Layout::Component::TextSize::largeText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoundedLabel)
};
