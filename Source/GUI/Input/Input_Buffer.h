#pragma once
/**
 * @file  Input_Buffer.h
 *
 * @brief  Stores chorded keyboard input and sends it to the target application.
 */

#include "Text_CharTypes.h"
#include "JuceHeader.h"

namespace Input { class Buffer; }

/**
 * @brief  Unless immediate mode is enabled, all keyboard input is cached within
 *         this object until the user chooses to forward the input to the target
 *         window.
 */
class Input::Buffer
{
public:
    Buffer() { }

    virtual ~Buffer() { }

    /**
     * @brief  Gets the cached input string, not including modifiers.
     *
     * @return  All text waiting to be sent to the target window.
     */
    Text::CharString getInputText() const;

    /**
     * @brief  Gets the modifier key flags that will be applied to the input.
     *
     * @return  All combined modifier flags, as defined in Input::Modifiers.
     */
    int getModifierFlags() const;

    /**
     * @brief  Adds a character to the end of the cached input string.
     *
     * @param inputChar  The character that should be added to the end of the
     *                   input string.
     */
    void appendCharacter(const Text::CharValue inputChar);

    /**
     * @brief  Removes the last character from the end of the input string.
     */
    void deleteLastChar();

    /**
     * @brief  Sets the modifier keys that will be applied to the buffered
     *         input.
     *
     * @param modifierFlags  All combined modifier flags, as defined in
     *                       Input::Modifiers.
     */
    void setModifiers(const int modifierFlags);

    /**
     * @brief  Removes all saved input.
     *
     * @param clearModifiers  Whether the modifier flags should also be cleared.
     */
    void clearInput(const bool clearModifiers = true);

    /**
     * @brief  Checks if the input buffer currently contains any input.
     *
     * @return  Whether the buffer string is empty.
     */
    bool isEmpty() const;

private:
    // Cached input text:
    Text::CharString inputText;
    // Combined key modifier flags, as defined in Input::Modifiers.
    int keyModifiers = 0;
};
