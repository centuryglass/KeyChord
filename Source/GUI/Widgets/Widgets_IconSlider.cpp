#include "Widgets_IconSlider.h"
#include "Util_Math.h"

// Icon width will not exceed totalWidth / maxIconWidthFraction
static const constexpr int maxIconWidthFraction = 6;
// Icon width won't go under this minimum size:
static const constexpr int minIconWidth = 4;

// Creates a slider with a default range of 0-100.
Widgets::IconSlider::IconSlider(const juce::Identifier& imageKey) :
lowIcon(imageKey, 0, juce::RectanglePlacement::centred),
highIcon(imageKey, 1, juce::RectanglePlacement::centred)
{
#if JUCE_DEBUG
    setName("Widgets::IconSlider");
#endif
    using juce::Slider;
    slider.setSliderStyle(Slider::LinearHorizontal);
    slider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    slider.setRange(0, 100);
    addAndMakeVisible(slider);
    addAndMakeVisible(lowIcon);
    addAndMakeVisible(highIcon);
}


// Changes the slider's stored value.
void Widgets::IconSlider::setValue
(double newValue, juce::NotificationType notification)
{
    slider.setValue(newValue, notification);
}


// Gets the slider's stored value.
double Widgets::IconSlider::getValue() const
{
    return slider.getValue();
}


// Subscribes a slider listener object to changes in the slider's value.
void Widgets::IconSlider::addListener(juce::Slider::Listener* listener)
{
    slider.addListener(listener);
}


// Compares a raw slider pointer to this object.
bool Widgets::IconSlider::operator== (const juce::Slider* sliderPtr) const
{
    return sliderPtr == &slider;
}


// Sets the slider value's limits.
void Widgets::IconSlider::setRange(double newMinimum, double newMaximum,
        double newInterval)
{
    slider.setRange(newMinimum, newMaximum, newInterval);
}


// Updates the icons and slider to fit the component bounds.
void Widgets::IconSlider::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    const int iconSize = Util::Math::median<int>(
            minIconWidth,
            bounds.getHeight(),
            bounds.getWidth() / maxIconWidthFraction);
    lowIcon.setBounds(bounds.withWidth(iconSize));
    highIcon.setBounds(bounds.withLeft(bounds.getRight() - iconSize));
    slider.setBounds(bounds.reduced(iconSize, 0));
}
