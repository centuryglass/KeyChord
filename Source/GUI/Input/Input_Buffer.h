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
    /**
     * @brief  Saves the target window ID on construction.
     *
     * @param targetWindow    The ID of the window where input will be sent.
     */
    Buffer(const int targetWindow);

    /**
     * @brief  If the input string isn't empty, send it to the target window
     *         before destruction.
     */
    virtual ~Buffer();

    /**
     * @brief  Gets the cached input string.
     *
     * @return  All text waiting to be sent to the target window.
     */
    Text::CharString getInputText() const;

    /**
     * @brief  Sends all cached input to the target window and clears the cached
     *         input string.
     */
    void sendAndClearInput();

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
     * @brief  Removes all saved input.
     */
    void clearInput();

    /**
     * @brief  Checks if the input buffer currently contains any input.
     *
     * @return  Whether the buffer string is empty.
     */
    bool isEmpty() const;

    /**
     * @brief  Gets the cached input string, without including modifiers.
     *
     * @return  All text waiting to be sent to the target window, not including
     *          modifiers.
     */
    Text::CharString getRawInput() const;

private:
    // Cached input text:
    Text::CharString inputText;
    // ID of the window where text will be sent:
    const int targetWindow;
};
