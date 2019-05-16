#pragma once
/**
 * @file  Windows_Alert.h
 *
 * @brief  Provides convenience functions for creating simple popup alert
 *         windows.
 */

namespace juce { class String; }

#include <functional>

namespace Windows
{
    namespace Alert
    {
        /**
         * @brief  Requests user confirmation before performing some action.
         *
         * @param title      The text to print at the top of the confirmation
         *                   box.
         *
         * @param message    Detailed information text about the action being
         *                   confirmed.
         *
         * @param onConfirm  If the user presses the "OK" button, this callback
         *                   function will run.
         */
        void confirmAction(
                const juce::String title,
                const juce::String message,
                const std::function<void()> onConfirm);
    }
}
