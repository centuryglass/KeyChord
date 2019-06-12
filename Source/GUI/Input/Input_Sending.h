#pragma once
/**
 * @file  Input_Sending.h
 *
 * @brief  Creates artificial key events to send to another window.
 */

#include "Input_Buffer.h"

namespace Input 
{
    namespace Sending
    {
        /**
         * @brief  Sends a single key press event to a window.
         *
         * @param keyValue       A key value to type.
         *
         * @param modifierFlags  Modifier flags to apply to the key, as defined
         *                       in Input::Modifiers.
         *
         * @param targetWindow   The ID of the window where the key event should
         *                       be sent.
         */
        void sendKey(const Text::CharValue keyValue, const int modifierFlags,
                const int targetWindow);

        /**
         * @brief  Take all input from a buffer object, and send it to a window.
         *
         * @param inputBuffer   The input buffer object.
         *
         * @param targetWindow  The ID of the window where input will be sent.
         */
        void sendBufferedInput(Input::Buffer& inputBuffer,
                const int targetWindow);


    }
}
