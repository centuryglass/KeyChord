#pragma once
/**
 * @file  Widgets_Spinner.h
 *
 * @brief  Provides a loading indicator component.
 */

#include "Theme_Image_Component.h"
#include "Windows_FocusedTimer.h"
#include "JuceHeader.h"

namespace Widgets { class Spinner; }
/**
 * @brief  A Component that shows a small spinning circle animation.
 *
 *  The Spinner component is intended to be used as a loading indicator. It
 * only animates while visible, and can be set to automatically lose visibility
 * after a set amount of time.
 */
class Widgets::Spinner : public Theme::Image::Component<>,
        public Windows::FocusedTimer
{
public:
    /**
     * @brief  Optionally constructs the spinner with a specific timeout period.
     *
     * @param secondsToTimeout  Sets how many seconds should pass after
     *                          enabling the spinner before the spinner is
     *                          automatically disabled. If this value is
     *                          negative, the spinner will keep running until
     *                          destroyed or turned off with setEnabled().
     */
    Spinner(const int secondsToTimeout = -1);

    virtual ~Spinner() {}

private:
    /**
     * @brief  Disables the animation when visibility is lost, and enables it
     *         when visibility is gained.
     */
    void visibilityChanged() override;

    /**
     * @brief  Shows the next frame of the spinner animation, and disables the
     *         spinner if runtime exceeds the timeout period.
     */
    virtual void timerCallback() override;

    // The index of the spinner's currently displayed image:
    int imageIndex = 0;

    // The number of image frames in the spinner animation:
    int numImages;

    // The amount of time in milliseconds that the spinner has been visible:
    int runtime = 0;

    // The number of milliseconds to allow the spinner to run, or -1 to let the
    // spinner run indefinitely:
    int timeout;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spinner)
};
