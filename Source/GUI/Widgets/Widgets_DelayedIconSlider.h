#pragma once
/**
 * @file  Widgets_DelayedIconSlider.h
 *
 * @brief  An IconSlider that runs a function at controlled intervals as it is
 *         changed.
 */

#include "Widgets_IconSlider.h"
#include "Windows_FocusedTimer.h"

namespace Widgets { class DelayedIconSlider; }

/**
 * @brief  A simplified IconSlider class that runs an update function at a
 *         controlled frequency while it is being dragged.
 *
 *  juce::Slider objects send update signals to any number of listeners
 * whenever their slider is clicked, dragged, or released. DelayedIconSlider
 * combines the slider and the listener to provide a basis for simple, single
 * purpose slider classes.
 *
 *  DelayedIconSlider has a single virtual valueChanged function to handle all
 * updates. Whenever it is being dragged, or whenever it is released, the
 * valueChanged function will be called regularly. The frequency that the update
 * function is called is set on construction.
 */
class Widgets::DelayedIconSlider : public IconSlider,
        public Windows::FocusedTimer, private juce::Slider::Listener
{
public:
    /**
     * @brief  Sets the displayed images and update frequency on construction.
     *
     * @param imageKey         The key to the slider's image assets within the
     *                         image asset configuration file.
     *
     * @param updateFrequency  Frequency in milliseconds to call the
     *                         valueChanged function while the slider is being
     *                         dragged.
     */
    DelayedIconSlider(const juce::Identifier& imageKey,
            const int updateFrequency);

    virtual ~DelayedIconSlider() { }

protected:
    /**
     * @brief  Cancels the slider timer when visibility is lost.
     */
    virtual void visibilityChanged() override;

private:
    /**
     * @brief  The update function that will periodically run while the slider
     *         value is changing.
     *
     * @param newValue  The current slider value.
     */
    virtual void valueChanged(const double newValue) = 0;

    /**
     * @brief  Periodically calls valueChanged while the slider is dragged.
     */
    virtual void timerCallback() final override;

    /**
     * @brief  Slider::Listener requires this function to be implemented, but
     *         it's not actually used.
     *
     *  The timerCallback function replaces this function, so the frequency of
     * update actions can more easily be controlled.
     */
    virtual void sliderValueChanged(juce::Slider* slider) final override { };

    /**
     * @brief  Starts a timer to update the slider values as its being dragged.
     *
     * @param slider  This slider object.
     */
    virtual void sliderDragStarted(juce::Slider* slider) final override;

    /**
     * @brief  Stops the timer and immediately updates slider values.
     *
     * @param slider  This slider object.
     */
    virtual void sliderDragEnded(juce::Slider* slider) final override;

    // Slider callback update frequency in milliseconds:
    const int updateFrequency;
};
