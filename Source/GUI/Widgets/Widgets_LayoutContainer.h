#pragma once
/**
 * @file  Widgets_LayoutContainer.h
 *
 * @brief  A component that arranges a set of child components within itself.
 */

#include "JuceHeader.h"
#include "Layout_Group_Manager.h"

namespace Widgets { class LayoutContainer; }

/**
 * @brief  A container Component that displays and arranges a set of child
 *         components using a Layout::Group::RelativeLayout object.
 */
class Widgets::LayoutContainer : public juce::Component
{
public:
    LayoutContainer() { }

    virtual ~LayoutContainer() { }

    // JUCE ColourId values:
    enum ColourIds
    {
        // Colour used to fill in the background if no image is set:
        backgroundColourId = 0x1900499
    };

    /**
     * @brief  Sets the layout of the container component.
     *
     *  If the layout was set previously, the old layout will be cleared, and
     * its components will be removed.
     *
     * @param layout  Provides the new layout of child components.
     */
    virtual void setLayout(const Layout::Group::RelativeLayout layout);

    /**
     * @brief  Sets a background image to draw behind all child components.
     *
     * @param bgImage   The image to draw, or a null Image(). If a null image
     *                  is used, the background will be filled with the
     *                  background colour instead of an image.
     */
    void setBackgroundImage(const juce::Image bgImage);

    /**
     * @brief  Adds all components in the layout to the container and makes
     *         them visible.
     */
    void addAndShowLayoutComponents();

protected:
    /**
     * @brief  Runs whenever the container is resized, after it reapplies its
     *         layout.
     */
    virtual void containerResized() { }

private:
    /**
     * @brief  Updates component layout and back button bounds when the
     *         container is resized.
     */
    virtual void resized() override;

    /**
     * @brief  Fills the container background with an image or colour.
     *
     * @param graphics  The graphics context used to draw the container.
     */
    virtual void paint(juce::Graphics& graphics) override;

    // Manages the relative layout of container child components:
    Layout::Group::Manager layoutManager;

    // Optional container background image:
    juce::Image backgroundImage;
};
