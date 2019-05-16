#pragma once
/**
 * @file  Windows_FocusedTimer.h
 *
 * @brief  A basis for timer objects that only act when the main application
 *         window is focused.
 */

#include "JuceHeader.h"
#include "Windows_FocusListener.h"

namespace Windows { class FocusedTimer; }

/**
 * @brief  A juce::Timer class that only executes its timer callback function
 *         when the main application window is focused.
 *
 *  In order to reduce the resources used when inactive, running FocusedTimer
 * objects are suspended if the main application window loses focus, and will
 * only resume when the window regains focus.
 *
 *  Suspended timers save their next scheduled runtime, not the amount of time
 * left on the timer, so timers that would have ended while suspended will
 * immediately call their timerCallback when resumed.
 */
class Windows::FocusedTimer : public juce::Timer, public Windows::FocusListener
{
public:
    FocusedTimer() { }

    virtual ~FocusedTimer() { }

    /**
     * @brief  Stops the timer if it is active, or cancels it if it is
     *         suspended.
     */
    void stopTimer();

    /**
     * @brief  Starts the timer, setting the amount of time until the timer's
     *         callback function will run.
     *
     *  If the window is currently unfocused, the timer will immediately be
     * suspended.
     *
     * @param timerMilliseconds  Milliseconds to wait before the timer callback
     *                           function runs.
     */
    void startTimer(const int timerMilliseconds);

private:
    /**
     * @brief  Called whenever the timer is suspended, but takes no action.
     *
     *  Override this function in timer classes that need to act whenever the
     * window loses focus and the timer is suspended.
     */
    virtual void onSuspend() {}

    /**
     * @brief  Suspends the timer when the window loses focus.
     */
    void windowFocusLost() final override;

    /**
     * @brief  Resumes the timer when the window gains focus.
     */
    void windowFocusGained() final override;

    // Next scheduled runtime for a suspended timer, relative to
    // Time::getMillisecondCounter(). If set to zero, this indicates that the
    // timer is not suspended.
    juce::uint32 suspendedEndTime = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FocusedTimer)
};
