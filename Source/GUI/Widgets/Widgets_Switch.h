#pragma once
/**
 * @file  Widgets_Switch.h
 *
 * @brief  Provides an animated binary switch component.
 */

#include "JuceHeader.h"

namespace Widgets { class Switch; }

/**
 * @brief  A juce::ToggleButton drawn as a round handle that switches off (left)
 *         or on (right) in front of a rounded rectangle.
 */
class Widgets::Switch : public juce::ToggleButton, private juce::Timer
{
public:

    // JUCE ColourId values:
    enum ColourIds
    {
        backgroundColourId = 0x1900700,
        handleColourId = 0x1900701,
        handleOffColourId = 0x1900702
    };

    /**
     * @brief  Initializes the switch handle on construction.
     */
    Switch();

    virtual ~Switch() { }

    /**
     * @brief  Sets the Switch object's toggle state.
     *
     * @param shouldBeOn    The selected toggle state.
     *
     * @param notification  Whether listeners will be notified if the toggle
     *                      state changes.
     *
     * @param animate       Whether the component should animate the transition
     *                      to the new toggle state. If true, this will briefly
     *                      delay the change in toggle state if the switch is
     *                      currently animating.
     */
    void setToggleState(
            bool shouldBeOn,
            juce::NotificationType notification =
            juce::NotificationType::dontSendNotification,
            bool animate = false);

private:
    /**
     * @brief  Draws the switch background as a rounded rectangle.
     *
     * @param graphics           The JUCE graphics context used to draw the
     *                           switch background.
     *
     * @param isMouseOverButton  Whether the mouse cursor is currently above
     *                           the switch.
     *
     * @param isButtonDown       Whether the switch is currently being clicked.
     */
    void paintButton(juce::Graphics& graphics,
            bool isMouseOverButton, bool isButtonDown) override;

    /**
     * @brief  Updates the switch background and handle shapes to fit the new
     *         component bounds, without changing their aspect ratios.
     */
    void resized() override;

    /**
     * @brief  Animates the transition between on and off states when the
     *         switch is clicked, moving the handle left or right as
     *         appropriate.
     */
    void clicked() override;

    /**
     * @brief  Draws the animated circular switch handle.
     */
    class SwitchHandle : public juce::Component
    {
    public:
        SwitchHandle() { }

        virtual ~SwitchHandle() { }

        /**
         * @brief  Sets the switch handle fill colour.
         *
         * @param newColour  The colour to use when painting the switch handle.
         */
        void setColour(const juce::Colour newColour);

    private:
        /**
         * @brief  Draws the switch handle as a filled circle.
         *
         * @param graphics  The JUCE graphics context used to draw the
         *                  component.
         */
        void paint(juce::Graphics& graphics) override;

        // Handle fill colour:
        juce::Colour colour;
    };

    /**
     * @brief  Handles switch transitions delayed by waiting for animation.
     */
    void timerCallback() override;

    // Draws the switch handle as a filled circle:
    SwitchHandle handle;

    // The local bounds of the switch handle in both button states:
    juce::Rectangle<int> handleBoundsOff, handleBoundsOn;

    // The local bounds of the switch's rounded rectangle background:
    juce::Rectangle<int> backgroundShape;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Switch)
};
