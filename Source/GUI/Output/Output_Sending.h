#pragma once
/**
 * @file  Output_Sending.h
 *
 * @brief  Creates artificial key events to send to another window.
 */

#include "Output_Buffer.h"

namespace Output 
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
         * @brief  Take all text from a buffer object, and sends it to a window.
         *
         * @param outputBuffer  The buffer object.
         *
         * @param targetWindow  The ID of the window where the text will be
         *                      sent.
         */
        void sendBufferedOutput(Buffer& outputBuffer,
                const int targetWindow);


    }
}
