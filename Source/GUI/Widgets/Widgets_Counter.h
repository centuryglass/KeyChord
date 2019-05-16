#pragma once
/**
 * @file  Widgets_Counter.h
 *
 * @brief  Provides a counter component used to enter integer values.
 */

#include "Widgets_TextEditor.h"
#include "JuceHeader.h"

namespace Widgets { class Counter; }

/**
 * @brief  A UI widget that allows the user to enter or adjust an integer field.
 *
 *  The user can directly enter a number into the number field, or increase or
 * decrease its value by pressing the plus and minus buttons on either side of
 * the number field.
 */
class Widgets::Counter : public juce::Component,
        private juce::Button::Listener,
        private juce::TextEditor::Listener
{
public:
    /**
     * @brief  Sets the stored value and range of the counter on construction.
     *
     * @param initialValue  The initial counter value.
     *
     * @param minimum       The lowest value the counter can have.
     *
     * @param maximum       The highest value the counter can have.
     *
     * If the maximum is less than the minimum, their values will be swapped.
     */
    Counter(const int initialValue = 0, const int minimum = 0,
            const int maximum = 10);

    virtual ~Counter() { }

    /**
     * @brief  Gets the counter's stored value.
     *
     * @return  The integer value held by the counter.
     */
    int getCount() const;

    /**
     * @brief  Sets the counter's stored integer to a new value.
     *
     * @param newValue  The new value to save. If this value is outside of the
     *                  counter's permitted range, it will be rounded to the
     *                  closest valid integer.
     */
    void setCount(const int newValue);

    /**
     * @brief  Sets a new minimum value, updating the counter value if
     *         necessary.
     *
     * @param newMin  The new minimum value. If the maximum is less than the
     *                new minimum, their values will be swapped.
     */
    void setMinimum(const int newMin);

    /**
     * @brief  Sets a new maximum value, updating the counter value if
     *         necessary.
     *
     * @param newMin  The new maximum value. If the maximum is less than the
     *                new minimum, their values will be swapped.
     */
    void setMaximum(const int newMax);

private:
    /**
     * @brief  Increments or decrements the counter value when the plus or
     *         minus buttons are clicked.
     *
     * @param button  The button component that was clicked.
     */
    void buttonClicked(juce::Button* button) override;

    /**
     * @brief  Parses the counter's text field as an integer, and uses it to
     *         change the counter value.
     */
    void setCounterFromTextField();

    /**
     * @brief  Updates the counter's text field to display the counter value.
     */
    void setTextFieldFromCounter();

    /**
     * @brief  Sets the counter's value from the text field when the text field
     *         loses focus.
     *
     * @param editor  The counter's internal text editor.
     */
    virtual void textEditorFocusLost(juce::TextEditor& editor) override;

    /**
     * @brief  Sets the counter's value from the text field when the text field
     *         is focused and the user presses the return key.
     *
     * @param editor  The counter's internal text editor.
     */
    virtual void textEditorReturnKeyPressed
    (juce::TextEditor& editor) override;

    /**
     * @brief  Sets the counter's value from the text field when the text field
     *         is focused and the user presses the escape key.
     *
     * @param editor  The counter's internal text editor.
     */
    virtual void textEditorEscapeKeyPressed
    (juce::TextEditor& editor) override;

    /**
     * @brief  Arranges the buttons and number field to fit in the component
     *         bounds.
     */
    virtual void resized() override;

    // Decrements the counter value:
    juce::TextButton minusButton;

    // Increments the counter value:
    juce::TextButton plusButton;

    // Displays and edits the counter value:
    Widgets::TextEditor textField;

    // Limits the text field to valid integer characters.
    juce::TextEditor::LengthAndCharacterRestriction permittedCharacters;

    // The current counter value:
    int count;

    // The smallest permitted counter value:
    int minimum;

    // The largest permitted counter value:
    int maximum;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Counter)
};
