#pragma once
/**
 * @file  Output_Buffer.h
 *
 * @brief  Stores chorded keyboard input and sends it to the target application.
 */

#include "Text_CharTypes.h"
#include "JuceHeader.h"

namespace Output { class Buffer; }

/**
 * @brief  Unless immediate mode is enabled, all keyboard input is cached within
 *         this object until the user chooses to forward the input to the target
 *         window.
 */
class Output::Buffer
{
public:
    Buffer() { }

    virtual ~Buffer() { }

    /**
     * @brief  Gets the cached output string, not including modifiers.
     *
     * @return  All text waiting to be sent to the target window.
     */
    Text::CharString getBufferedText() const;

    /**
     * @brief  Gets the modifier key flags that will be applied to the output.
     *
     * @return  All combined modifier flags, as defined in Output::Modifiers.
     */
    int getModifierFlags() const;

    /**
     * @brief  Adds a character to the end of the cached output string.
     *
     * @param outputChar  The character that should be added to the end of the
     *                    output string.
     */
    void appendCharacter(const Text::CharValue outputChar);

    /**
     * @brief  Removes the last character from the end of the output string.
     */
    void deleteLastChar();

    /**
     * @brief  Sets the modifier keys that will be applied to the buffered
     *         text.
     *
     * @param modifierFlags  All combined modifier flags, as defined in
     *                       Output::Modifiers.
     */
    void setModifiers(const int modifierFlags);

    /**
     * @brief  Removes all buffered text.
     *
     * @param clearModifiers  Whether the modifier flags should also be cleared.
     */
    void clear(const bool clearModifiers = true);

    /**
     * @brief  Checks if the buffer currently contains any text or key values.
     *
     * @return  Whether the buffer string is empty.
     */
    bool isEmpty() const;

private:
    // Buffered text/keys:
    Text::CharString bufferedText;
    // Combined key modifier flags, as defined in Output::Modifiers.
    int keyModifiers = 0;
};
