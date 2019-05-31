#include "Input_Buffer.h"
#include "Util_ConditionChecker.h"
#include "Windows_XInterface.h"
#include "Text_CharSet_Values.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Input::Buffer::";
#endif

// Commmand prefix used to transmit text to the focused window.
static const juce::String typeCommand("xdotool key ");

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
juce::Array<unsigned int> Input::Buffer::getInputText() const
{
    return inputText;
}


// Sends all cached input to the target window, and clears the saved input
// string.
void Input::Buffer::sendAndClearInput()
{
    if (inputText.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": No text to send, aborting");
        return;
    }
    DBG(dbgPrefix << __func__ << ": Forwarding " << inputText.size()
            << " characters to target window.");

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
        for (int cIndex = 0; cIndex < inputText.size(); cIndex++)
        {
            juce::String charString = Text::CharSet::Values::getCharString(
                    inputText[cIndex]);
            if (charString.isEmpty())
            {
                DBG(dbgPrefix << "sendAndClearInput: No string conversion for "
                        << (int) inputText[cIndex] << " at index " << cIndex);
                continue;
            }
            juce::String commandString(typeCommand + "'" + 
                    ((charString == "'") ? "\'" : charString) + "'");
            DBG("running input command: " << commandString);
            system(commandString.toRawUTF8());
        }
        clearInput();
        if(juce::Desktop::getInstance().getComponent(0) != nullptr)
        {
            xInterface.activateWindow(keyChordWindow);
        }
    },
    focusTimeout,
    []()
    {
        DBG(dbgPrefix << "sendAndClearInput: "
                << "Failed to get window focus before timeout!");
    });
    focusChecker.waitForUpdate();
}


// Adds a character to the end of the cached input string.
void Input::Buffer::appendCharacter(const unsigned int inputChar)
{
    inputText.add(inputChar);
}


// Removes the last character from the end of the input string.
void Input::Buffer::deleteLastChar()
{
    inputText.removeLast();
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
