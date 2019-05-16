#include "Widgets_Switch.h"
#include "Util_SafeCall.h"

// Switch layout constants:

// Minimum switch component width:
static const constexpr int minWidth = 8;
// Minimum switch component height:
static const constexpr int minHeight = 4;
// Width : height ratio of all switch components:
static const constexpr double widthToHeightRatio = 1.5;
// Switch handle margin size, as a fraction of the switch height:
static const constexpr double handleMarginFraction = 0.06;

// Switch handle layout constants:

// Minimum switch handle margin size in pixels:
static const constexpr int minimumHandleMargin = 2;
// Minimum switch handle size in pixels:
static const constexpr int minimumHandleSize = 2;

// Switch animation duration in milliseconds:
static const constexpr int animationDuration = 150;

// Initializes the switch handle on construction.
Widgets::Switch::Switch()
{
#if JUCE_DEBUG
    setName("Widgets::Switch");
#endif
    addAndMakeVisible(handle);
    handle.setInterceptsMouseClicks(false, false);
}


// Sets the Switch object's toggle state.
void Widgets::Switch::setToggleState(bool shouldBeOn,
        juce::NotificationType notification, bool animate)
{
    if (shouldBeOn != getToggleState())
    {
        juce::ToggleButton::setToggleState(shouldBeOn, notification);
        clicked();
    }
}


// Draws the switch background as a rounded rectangle.
void Widgets::Switch::paintButton
(juce::Graphics& graphics, bool isMouseOverButton, bool isButtonDown)
{
    const float radius = float(backgroundShape.getHeight()) / 2.0f;
    graphics.setColour(findColour(backgroundColourId));
    graphics.fillRoundedRectangle(backgroundShape.getX(),
            backgroundShape.getY(),
            backgroundShape.getWidth(),
            backgroundShape.getHeight(),
            radius);
}


// Updates the switch background and handle shapes to fit the new component
// bounds, without changing their aspect ratios.
void Widgets::Switch::resized()
{
    backgroundShape = getLocalBounds();
    int width = std::max(getWidth(), minWidth);
    int height = std::max(getHeight(), minHeight);

    // Ensure minimum bounds are met:
    if (width != getWidth() || height != getHeight())
    {
        setBounds(getX(), getY(), width, height);
        return;
    }

    // Ensure width : height ratio is correct:
    const float ratio = width / height;
    if (ratio > widthToHeightRatio) // too wide, reduce width
    {
        width = height * widthToHeightRatio;
        backgroundShape.reduce((backgroundShape.getWidth() - width) / 2, 0);
    }
    else if (ratio < widthToHeightRatio) // too tall, reduce height
    {
        height = width / widthToHeightRatio;
        backgroundShape.reduce(0, (backgroundShape.getHeight() - height) / 2);
    }
    int handleMargin = std::max(int(height * handleMarginFraction),
            minimumHandleMargin);
    const int handleSize = std::max(int(height - handleMargin * 2),
            minimumHandleSize);
    handleBoundsOff.setBounds(
            backgroundShape.getX() + handleMargin,
            backgroundShape.getY() + handleMargin,
            handleSize,
            handleSize);
    handleBoundsOn = handleBoundsOff.withRightX
            (backgroundShape.getRight() - handleMargin);
    juce::Colour handleColour;
    juce::Rectangle<int>& handleBounds
            = getToggleState() ?  handleBoundsOn : handleBoundsOff;
    if (handle.getBounds() != handleBounds)
    {
        clicked();
    }
}


// Animates the transition between on and off states when the switch is
// clicked, moving the handle left or right as appropriate.
void Widgets::Switch::clicked()
{
    if (juce::Desktop::getInstance().getAnimator().isAnimating(&handle))
    {
        startTimer(animationDuration);
    }
    else
    {
        const juce::Rectangle<int>& bounds
                = getToggleState() ?  handleBoundsOn : handleBoundsOff;
        if (bounds != handle.getBounds())
        {
            juce::Desktop::getInstance().getAnimator().animateComponent(
                    &handle, bounds, 1.0f, animationDuration, false, 0.0, 0.0);
        }
        handle.setColour(findColour( getToggleState() ?  handleColourId
                : handleOffColourId));
    }
}


// Sets the switch handle fill colour.
void Widgets::Switch::SwitchHandle::setColour(const juce::Colour newColour)
{
    colour = newColour;
}


// Draws the switch handle as a filled circle.
void Widgets::Switch::SwitchHandle::paint(juce::Graphics& graphics)
{
    graphics.setColour(colour);
    graphics.fillEllipse(getLocalBounds().toFloat());
}


// Handles switch transitions delayed by waiting for animation.
void Widgets::Switch::timerCallback()
{
    Util::SafeCall::callAsync<Switch>(this, [](Switch* switchPtr)
    {
        switchPtr->stopTimer();
        switchPtr->clicked();
    });
}
