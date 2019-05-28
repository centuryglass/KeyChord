#pragma once
/**
 * @file  Input_Buffer.h
 *
 * @brief  Stores chorded keyboard input and sends it to the target
 *         application.
 */

#include "JuceHeader.h"
#include <X11/Xlib.h>

namespace Input { class Buffer; }

class Input::Buffer
{
public:
    /**
     * @brief  Saves all necessary window IDs on construction.
     *
     * @param targetWindow    The ID of the window where input will be sent.
     *
     * @param keyChordWindow  The ID of this application's single window.
     */
    Buffer(const Window targetWindow, const Window keyChordWindow);

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
    juce::String getInputText() const;

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
    void appendCharacter(const char inputChar);

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

private:
    // Cached input text:
    juce::String inputText;
    // ID of the window where text will be sent:
    const Window targetWindow;
    // This application's single window ID:
    const Window keyChordWindow;
};
