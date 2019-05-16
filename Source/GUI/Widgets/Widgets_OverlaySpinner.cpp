#include "Widgets_OverlaySpinner.h"

// The internal spinner height and overlay text component height as a fraction
// of the total component height:
static const constexpr double spinnerHeightFraction = 0.1;

// The vertical position of the overlay text component as a fraction of the
// total component height:
static const constexpr double textTopFraction = 0.66;

// Sets the length of the spinner's timeout period on construction.
Widgets::OverlaySpinner::OverlaySpinner(const int secondsToTimeout) :
spinner(secondsToTimeout)
{
#if JUCE_DEBUG
    setName("Widgets::OverlaySpinner");
#endif
    loadingText.setText("", juce::NotificationType::dontSendNotification);
    loadingText.setJustificationType(juce::Justification::centred);
    loadingText.setColour(juce::Label::textColourId,
            findColour(textColourId));
    addAndMakeVisible(loadingText);
    addAndMakeVisible(spinner);
}


// Sets text to display below the spinner
void Widgets::OverlaySpinner::setLoadingText(const juce::String newText)
{
    loadingText.setText(newText, juce::NotificationType::dontSendNotification);
}


// Paints the overlay background.
void Widgets::OverlaySpinner::paint(juce::Graphics& graphics)
{
    graphics.fillAll(findColour(backgroundColourId));
}


// Updates the spinner and loading text to fit the overlay bounds.
void Widgets::OverlaySpinner::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    const int spinnerSize = bounds.getHeight() * spinnerHeightFraction;
    const juce::Rectangle<int> textBounds(0,
            bounds.getHeight() * textTopFraction, getWidth(), spinnerSize);
    spinner.setBounds(bounds.withSizeKeepingCentre(spinnerSize, spinnerSize));
    loadingText.setBounds(textBounds);
}
