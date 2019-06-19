#pragma once
/**
 * @file  Windows_FocusControl.h
 *
 * @brief  Simplifies the process of changing window focus and waiting until
 *         that focus has been gained.
 */

#include "Util_ConditionChecker.h"

namespace Windows { class FocusControl; }

class Windows::FocusControl
{
public:
    FocusControl();

    /**
     * @brief  Focuses the window with the given window ID, waiting until
     *         focus is gained successfully before doing anything else.
     *
     * @param windowID   The X11 window ID of the window that should be focused.
     *
     * @param onFailure  An optional failure action to perform if the window
     *                   no longer exists or fails to focus before the timeout
     *                   period finishes.
     */
    void focusWindow(const int windowID,
            std::function<void()> onFailure = [](){});

    /**
     * @brief  Refocuses this application's window, and ensures the main
     *         component has keyboard focus.
     *
     * @param mainComponent  A pointer to the component object that should be
     *                       given keyboard focus.
     */
    void takeFocus(juce::Component* mainComponent);

private:
    // Handles the process of regularly checking for window focus conditions to
    // be met:
    Util::ConditionChecker focusChecker;
};
