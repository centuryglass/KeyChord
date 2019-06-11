#include "Input_Buffer.h"
#include "Util_ConditionChecker.h"
#include "Text_Values.h"
#include "MainWindow.h"
#include <iostream>

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Input::Buffer::";
#endif


// Gets the cached input string, not including modifiers.
Text::CharString Input::Buffer::getInputText() const
{
    return inputText;
}



// Gets the modifier key flags that will be applied to the input.
int Input::Buffer::getModifierFlags() const
{
    return keyModifiers;
}


// Adds a character to the end of the cached input string.
void Input::Buffer::appendCharacter(const Text::CharValue inputChar)
{
    inputText.add(inputChar);
}


// Sets the modifier keys that will be applied to the buffered input.
void Input::Buffer::setModifiers(const int modifierFlags)
{
    keyModifiers = modifierFlags;
}


// Removes the last character from the end of the input string.
void Input::Buffer::deleteLastChar()
{
    inputText.removeLast();
}


// Removes all saved input.
void Input::Buffer::clearInput(const bool clearModifiers)
{
    inputText.clear();
    if (clearModifiers)
    {
        keyModifiers = 0;
    }
}

    
// Checks if the input buffer currently contains any input.
bool Input::Buffer::isEmpty() const
{
    return inputText.isEmpty();
}
