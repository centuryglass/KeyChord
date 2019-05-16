#pragma once
/**
 * @file  OverlaySpinner.h
 *
 * @brief  Provides an overlay component that displays a loading spinner and
 *         optional loading text.
 */

#include "Widgets_Spinner.h"
#include "JuceHeader.h"
#include "Widgets_BoundedLabel.h"

namespace Widgets { class OverlaySpinner; }

/**
 * @brief  A UI component displaying a Spinner and optional loading text over a
 *         full-screen overlay.
 */
class Widgets::OverlaySpinner : public juce::Component
{
public:
    // JUCE Component ColourId values:
    enum ColourIds
    {
        backgroundColourId = 0x1900400,
        textColourId = 0x1900401
    };

    /**
     * @brief  Sets the length of the spinner's timeout period on construction.
     *
     * @param secondsToTimeout   How long the spinner will show itself before
     *                           timing out and disabling visibility. If this
     *                           value is negative, the spinner will never time
     *                           out.
     */
    OverlaySpinner(const int secondsToTimeout = -1);

    virtual ~OverlaySpinner() { }

    /**
     * @brief  Sets text to display below the spinner.
     *
     * @param newText  New loading text to display.
     */
    void setLoadingText(const juce::String newText);

private:
    /**
     * @brief  Paints the overlay background.
     *
     * @param graphics  The JUCE graphics context used to draw the object.
     */
    void paint(juce::Graphics & g) override;

    /**
     * @brief  Updates the spinner and loading text to fit the overlay bounds.
     */
    virtual void resized() override;

    // Displays optional loading text below the spinner:
    Widgets::BoundedLabel loadingText;

    // The animated spinner component:
    Widgets::Spinner spinner;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverlaySpinner)
};
