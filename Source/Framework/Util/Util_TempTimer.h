#pragma once
/**
 * @file  Util_TempTimer.h
 *
 * @brief  Creates single-use timers for scheduling delayed actions.
 */

#include "JuceHeader.h"

namespace Util { class TempTimer; }

/**
 * @brief  A temporary timer object that deletes itself after it runs once.
 *
 *  Once started it cannot be canceled, so avoid using this to handle actions
 * for objects that may be unexpectedly destroyed.
 */
class Util::TempTimer : public juce::ReferenceCountedObject,
        private juce::Timer
{
public:
    /**
     * @brief  Starts a new timer.
     *
     * @param runTimeMS  The time in milliseconds to wait before running the
     *                   timer callback function.
     *
     * @param callback   A function to run when the timer executes.
     */
    static void initTimer(const int runTimeMS,
            const std::function<void()> callback);

    virtual ~TempTimer() { }

private:
    /**
     * @brief  Starts the timer on construction.
     *
     * This should only be called in the initTimer function.
     *
     * @param runTimeMS  The time in milliseconds to wait before running the
     *                   timer callback function.
     *
     * @param callback   A function to run when the timer executes.
     */
    TempTimer(const int runTimeMS, const std::function<void()> callback);


    /**
     * @brief  Runs the timer callback function, and asynchronously deletes the
     *         timer.
     */
    void timerCallback() override;

    // TempTimer uses a reference counted pointer to itself to ensure it is
    // deleted once it finishes.
    juce::ReferenceCountedObjectPtr<TempTimer> selfRef = nullptr;
    // The callback function to run when the timer finishes:
    std::function<void() > callback;
};
