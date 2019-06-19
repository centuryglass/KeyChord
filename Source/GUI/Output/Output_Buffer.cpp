#include "Output_Buffer.h"
#include "Util_ConditionChecker.h"
#include "Text_Values.h"
#include "MainWindow.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Output::Buffer::";
#endif


// Gets the cached output string, not including modifiers.
Text::CharString Output::Buffer::getBufferedText() const
{
    return bufferedText;
}


// Gets the modifier key flags that will be applied to the output.
int Output::Buffer::getModifierFlags() const
{
    return keyModifiers;
}


// Adds a character to the end of the cached output string.
void Output::Buffer::appendCharacter(const Text::CharValue outputChar)
{
    bufferedText.add(outputChar);
}


// Sets the modifier keys that will be applied to the buffered text.
void Output::Buffer::setModifiers(const int modifierFlags)
{
    keyModifiers = modifierFlags;
}


// Removes the last character from the end of the output string.
void Output::Buffer::deleteLastChar()
{
    bufferedText.removeLast();
}


// Removes all bufferedText.
void Output::Buffer::clear(const bool clearModifiers)
{
    bufferedText.clear();
    if (clearModifiers)
    {
        keyModifiers = 0;
    }
}


// Checks if the buffer currently contains any text or key values.
bool Output::Buffer::isEmpty() const
{
    return bufferedText.isEmpty();
}
