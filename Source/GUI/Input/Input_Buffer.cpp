#include "Input_Buffer.h"
#include "Util_ConditionChecker.h"
#include "Windows_FocusControl.h"
#include "Text_Values.h"
#include "Text_ModTracker.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Input::Buffer::";
#endif

// Commmand prefix used to transmit key presses to the focused window.
static const juce::String keyCommand("xdotool key ");

// Commmand prefix used to transmit text strings to the focused window.
static const juce::String typeCommand("xdotool type ");

// Maximum time in milliseconds to wait for the target window to focus:
static const constexpr int focusTimeout = 5000;

// Saves the target window ID on construction.
Input::Buffer::Buffer(const int targetWindow) : targetWindow(targetWindow) { }


// If the input string isn't empty, send it to the target window before
// destruction.
Input::Buffer::~Buffer()
{
    sendAndClearInput();
}


// Gets the cached input string.
Text::CharString Input::Buffer::getInputText() const
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
    using juce::String;
    using ModKey = Text::ModTracker::ModKey;
    DBG(dbgPrefix << __func__ << ": Forwarding " << inputText.size()
            << " characters to target window.");

    // Add modifiers to input:
    const std::pair<ModKey, String> modInput [] =
    {
        { ModKey::control, "control+" },
        { ModKey::alt,     "alt+" },
        { ModKey::shift,   "shift+" },
        { ModKey::command, "super+" }
    };
    Text::ModTracker modTracker;
    String modPrefix;
    for (const auto& iter : modInput)
    {
        if (modTracker.isKeyHeld(iter.first))
        {
            modPrefix += iter.second;
            jassert(modPrefix.isNotEmpty());
        }
    }

    // Focus target window before sending input:
    Windows::FocusControl focusControl;
    focusControl.focusWindow(targetWindow,
    []()
    {
        DBG(dbgPrefix << "sendAndClearInput: "
                << "Failed to get window focus before timeout!");

        DBG(dbgPrefix << "sendAndClearInput: "
                << "Target window is probably gone, exiting");
        juce::JUCEApplication::getInstance()->quit();
    });

    for (int cIndex = 0; cIndex < inputText.size(); cIndex++)
    {
        juce::String charString = Text::Values::getXString(
                inputText[cIndex]);
        if (charString.isEmpty())
        {
            DBG(dbgPrefix << "sendAndClearInput: No string conversion for "
                    <<  juce::String((int) inputText[cIndex]) 
                    << " at index " << cIndex);
            continue;
        }
        juce::String commandString = keyCommand + "'" + modPrefix;
        commandString += ((charString == "'") ? "\'" : charString) + "'";
        DBG("running input command: " << commandString);
        system(commandString.toRawUTF8());
    }
    clearInput();
    if(juce::Desktop::getInstance().getComponent(0) != nullptr)
    {
        focusControl.takeFocus();
    }
}


// Adds a character to the end of the cached input string.
void Input::Buffer::appendCharacter(const Text::CharValue inputChar)
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
    Text::ModTracker modTracker;
    modTracker.clearAll();
}
    
// Checks if the input buffer currently contains any input.
bool Input::Buffer::isEmpty() const
{
    return inputText.isEmpty();
}


// Gets the cached input string, without including modifiers.
Text::CharString Input::Buffer::getRawInput() const
{
    return inputText;
}
