#include "Widgets_Spinner.h"
#include "Theme_Image_Component.h"
#include "Theme_Image_ConfigFile.h"
#include "Theme_Image_JSONKeys.h"

// The amount of time in milliseconds between switching image frames:
static const constexpr int timerInterval = 500;

Widgets::Spinner::Spinner(const int secondsToTimeout) :
Theme::Image::Component<>(Theme::Image::JSONKeys::spinner, 0,
        juce::RectanglePlacement::fillDestination),
timeout(secondsToTimeout)
{
#if JUCE_DEBUG
    setName("Widgets::Spinner");
#endif
    Theme::Image::ConfigFile config;
    numImages = config.getAssetList(Theme::Image::JSONKeys::spinner)
            .getImageFiles().size();
}


// Disables the animation when visibility is lost, and enables it when
// visibility is gained.
void Widgets::Spinner::visibilityChanged()
{
    if (isVisible())
    {
        startTimer(timerInterval);
    }
    else
    {
        stopTimer();
    }
}


// Shows the next frame of the spinner animation, and disables the spinner if
// runtime exceeds the timeout period.
void Widgets::Spinner::timerCallback()
{
    runtime += getTimerInterval();
    if (runtime > timeout && timeout > 0)
    {
        runtime = 0;
        setVisible(false);
    }
    else if (numImages > 0)
    {
        imageIndex = (imageIndex + 1) % numImages;
        setImageAssetIndex(imageIndex);
        startTimer(timerInterval);
    }
}
