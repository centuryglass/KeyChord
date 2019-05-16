#include "Widgets_Counter.h"
#include "Layout_Component_ConfigFile.h"
#include "Layout_Component_TextSize.h"
#include "Util_Math.h"

// The minimum width:height ratio of all Counter objects:
static const constexpr int widthToHeightRatio = 3;

// Sets the stored value and range of the counter on construction.
Widgets::Counter::Counter
(const int initialValue, const int minimum, const int maximum) :
count(initialValue),
permittedCharacters(-1, "-1234567890"),
minimum(minimum),
maximum(maximum),
minusButton("-"),
plusButton("+")
{
    minusButton.addListener(this);
    plusButton.addListener(this);
    textField.addListener(this);
    textField.setJustification(juce::Justification::centred);
    textField.setInputFilter(&permittedCharacters, false);
    addAndMakeVisible(minusButton);
    addAndMakeVisible(textField);
    addAndMakeVisible(plusButton);
    setTextFieldFromCounter();
}


// Gets the counter's stored value.
int Widgets::Counter::getCount() const
{
    return count;
}


// Sets the counter's stored integer to a new value.
void Widgets::Counter::setCount(const int newValue)
{
    count = Util::Math::median<int>(minimum, newValue, maximum);
    setTextFieldFromCounter();
}


// Sets a new minimum value, updating the counter value if necessary.
void Widgets::Counter::setMinimum(int newMin)
{
    minimum = newMin;
    setCount(count);
}


// Sets a new maximum value, updating the counter value if necessary.
void Widgets::Counter::setMaximum(int newMax)
{
    maximum = newMax;
    setCount(count);
}


// Increments or decrements the counter value when the plus or minus buttons
// are clicked.
void Widgets::Counter::buttonClicked(juce::Button* button)
{
    if (button == &minusButton)
    {
        setCount(count - 1);
    }
    else if (button == &plusButton)
    {
        setCount(count + 1);
    }
}


// Parses the counter's text field as an integer, and uses it to change the
// counter value.
void Widgets::Counter::setCounterFromTextField()
{
    setCount(textField.getText().getIntValue());
}


// Sets the counter's text field to display the counter value.
void Widgets::Counter::setTextFieldFromCounter()
{
    textField.setText(juce::String(count));
}


// Sets the counter's value from the text field when the text field loses focus.
void Widgets::Counter::textEditorFocusLost(juce::TextEditor& editor)
{
    setCounterFromTextField();
}


// Sets the counter's value from the text field when the text field is focused
// and the user presses the return key.
void Widgets::Counter::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    setCounterFromTextField();
}


// Sets the counter's value from the text field when the text field is focused
// and the user presses the escape key.
void Widgets::Counter::textEditorEscapeKeyPressed(juce::TextEditor& editor)
{
    setCounterFromTextField();
}


// Arranges the buttons and number field to fit in the component bounds.
void Widgets::Counter::resized()
{
    using juce::Rectangle;
    Rectangle<int> bounds = getLocalBounds();
    if (bounds.isEmpty())
    {
        return;
    }
    const float currentRatio = (float) bounds.getWidth()
            / (float) bounds.getHeight();
    if (currentRatio < widthToHeightRatio) // Too tall, reduce height:
    {
        bounds = bounds.withSizeKeepingCentre(getWidth(),
                getWidth() / widthToHeightRatio);
    }
    const int buttonSize = bounds.getWidth() / 3;
    Rectangle<int> minusBounds = bounds.withWidth(buttonSize);
    Rectangle<int> plusBounds = minusBounds.withX
            (bounds.getWidth() - buttonSize);
    bounds.setX(bounds.getX() + buttonSize);
    bounds.setWidth(bounds.getWidth() - buttonSize * 2);
    minusButton.setBounds(minusBounds);
    plusButton.setBounds(plusBounds);
    textField.setBounds(bounds);
}
