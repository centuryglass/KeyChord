#include "Input_Buffer.h"
#include "Util_ConditionChecker.h"
#include "Windows_XInterface.h"

// Commmand prefix used to transmit text to the focused window.
static const juce::String typeCommand("xdotool type ");

// Maximum time in milliseconds to wait for the target window to focus:
static const constexpr int focusTimeout = 1000;

// Saves all necessary window IDs on construction.
Input::Buffer::Buffer(const int targetWindow, const int keyChordWindow) :
    targetWindow(targetWindow), keyChordWindow(keyChordWindow) { }


// If the input string isn't empty, send it to the target window before
// destruction.
Input::Buffer::~Buffer()
{
    sendAndClearInput();
}


// Gets the cached input string.
juce::String Input::Buffer::getInputText() const
{
    return inputText;
}


// Sends all cached input to the target window, and clears the saved input
// string.
void Input::Buffer::sendAndClearInput()
{
    if (inputText.isEmpty())
    {
        return;
    }
    using juce::String;
    // Ensure single quotes are properly escaped:
    for (int cIndex = 0; cIndex < inputText.length(); cIndex++)
    {
        unsigned int testChar = inputText[cIndex];
        if (testChar == '\'')
        {
            inputText = inputText.substring(0, cIndex) + "'\\''"
                    + inputText.substring(cIndex + 1);
            cIndex += 3;
        }
    }

    Windows::XInterface xInterface;
    Util::ConditionChecker focusChecker;
    xInterface.activateWindow(targetWindow);
    focusChecker.setCheckInterval(50, 1.2f);
    focusChecker.startCheck([&xInterface, this]()
    {
        return xInterface.isActiveWindow(targetWindow);
    },
    [&xInterface, this]()
    {
        String commandString(typeCommand + "'" + inputText + "'");
        DBG("running input command: " << commandString);
        system(commandString.toRawUTF8());
        clearInput();
        xInterface.activateWindow(keyChordWindow);
    },
    focusTimeout,
    []()
    {
        DBG("Failed to get window focus before timeout!");
    });
    focusChecker.waitForUpdate();
}


// Adds a character to the end of the cached input string.
void Input::Buffer::appendCharacter(const char inputChar)
{
    inputText += juce::String::charToString(inputChar);
}


// Removes the last character from the end of the input string.
void Input::Buffer::deleteLastChar()
{
    inputText = inputText.substring(0, inputText.length() - 1);
}

// Removes all saved input.
void Input::Buffer::clearInput()
{
    inputText.clear();
}
    
// Checks if the input buffer currently contains any input.
bool Input::Buffer::isEmpty() const
{
    return inputText.isEmpty();
}
