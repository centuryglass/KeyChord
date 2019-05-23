#include "InputBuffer.h"

// Command prefix used to focus the target window:
static const juce::String focusCommand("xdotool windowfocus ");

// Commmand postfix used to transmit text to the focused window.
static const juce::String typeCommand(" && xdotool type ");

// Saves all necessary window IDs on construction.
InputBuffer::InputBuffer(const Window targetWindow, const Window keyChordWindow) :
    targetWindow(targetWindow), keyChordWindow(keyChordWindow) { }


// If the input string isn't empty, send it to the target window before
// destruction.
InputBuffer::~InputBuffer()
{
    sendAndClearInput();
}


// Gets the cached input string.
juce::String InputBuffer::getInputText() const
{
    return inputText;
}


// Sends all cached input to the target window, and clears the saved input
// string.
void InputBuffer::sendAndClearInput()
{
    if (inputText.isEmpty())
    {
        return;
    }
    using juce::String;
    using juce::juce_wchar;
    for (int cIndex = 0; cIndex < inputText.length(); cIndex++)
    {
        juce_wchar testChar = inputText[cIndex];
        if (testChar == '\'')
        {
            inputText = inputText.substring(0, cIndex) + "'\\''"
                    + inputText.substring(cIndex + 1);
            cIndex += 3;
        }
    }
    String commandString(focusCommand + String(targetWindow) + typeCommand
            + "'" + inputText + "' && " 
            + focusCommand + String(keyChordWindow));
    system(commandString.toRawUTF8());
    clearInput();
}


// Adds a character to the end of the cached input string.
void InputBuffer::appendCharacter(const char inputChar)
{
    inputText += juce::String::charToString(inputChar);
}


// Removes the last character from the end of the input string.
void InputBuffer::deleteLastChar()
{
    inputText = inputText.substring(0, inputText.length() - 1);
}

// Removes all saved input.
void InputBuffer::clearInput()
{
    inputText.clear();
}
    
// Checks if the input buffer currently contains any input.
bool InputBuffer::isEmpty() const
{
    return inputText.isEmpty();
}
