#pragma once
/**
 * @file  Windows_FocusTracker.h
 *
 * @brief  Tracks whether the main application window is focused, and sends
 *         window focus changes to all Windows::FocusListener objects.
 */

namespace Windows { class FocusTracker; }

#include "SharedResource_Resource.h"
#include "JuceHeader.h"

/**
 * @brief  A SharedResource used to track the window focus state and signal to
 *         all FocusListener objects when the window gains or loses focus.
 */
class Windows::FocusTracker : public SharedResource::Resource
{
public:
    // SharedResource object key:
    static const juce::Identifier resourceKey;

    FocusTracker();

    /**
     * @brief  Checks if the main application window is focused.
     *
     * @return  Whether the MainWindow object is currently focused.
     */
    bool getFocusState() const;

    /**
     * @brief  Updates whether the main application window is currently focused.
     *
     * @param windowFocused    Whether the MainWindow object is the focused
     *                         window.
     *
     * @param notifyListeners  Whether listeners should be notified if the
     *                         window focus state is changed.
     */
    void setFocusState(const bool windowFocused,
            const bool notifyListeners = true);

private:
    // Saves the last value sent by setFocusState:
    bool isFocused = true;
};
