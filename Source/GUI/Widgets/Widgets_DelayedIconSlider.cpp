#include "Widgets_DelayedIconSlider.h"

// Sets the displayed images and update frequency on construction.
Widgets::DelayedIconSlider::DelayedIconSlider
(const juce::Identifier& imageKey, const int updateFrequency) :
IconSlider(imageKey),
updateFrequency(updateFrequency)
{
    addListener(this);
}


// Cancels the slider timer when visibility is lost.
void Widgets::DelayedIconSlider::visibilityChanged()
{
    if (!isVisible())
    {
        stopTimer();
    }
}


// Periodically calls valueChanged while the slider is dragged.
void Widgets::DelayedIconSlider::timerCallback()
{
    valueChanged(getValue());
    startTimer(updateFrequency);
}


// Starts a timer to update the slider values as its being dragged.
void Widgets::DelayedIconSlider::sliderDragStarted(juce::Slider* slider)
{
    if (!isTimerRunning())
    {
        startTimer(updateFrequency);
    }
}


// Stops the timer and immediately updates slider values.
void Widgets::DelayedIconSlider::sliderDragEnded(juce::Slider* slider)
{
    stopTimer();
    valueChanged(getValue());
}
