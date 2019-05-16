#pragma once
/**
 * @file  Util_ConditionChecker.h
 *
 * @brief  Handles tasks that require waiting for an event that will occur
 *         after an indeterminate delay or not at all.
 */

#include "JuceHeader.h"

namespace Util { class ConditionChecker; }

/**
 * @brief  Waits for a condition to be met before running a scheduled action.
 */
class Util::ConditionChecker
{
public:
    /**
     * @brief  Initializes the internal timer and loads the default interval
     *         value on construction.
     */
    ConditionChecker();

    /**
     * @brief  Starts checking for a condition, unless already checking for
     *         another condition.
     *
     * @param check          The function used to check the condition. This
     *                       should return true when the condition has been met.
     *
     * @param callback       The action to perform once the condition is met.
     *
     * @param timeoutPeriod  Maximum duration in milliseconds to wait before
     *                       assuming the condition will not be met. If given a
     *                       negative value, condition checks will never time
     *                       out.
     *
     * @param onFailure      An optional action to take if the condition is not
     *                       met before timeout.
     *
     * @return               True if the condition check successfully started,
     *                       false if it was cancelled because the object is
     *                       still checking for another condition.
     */
    bool startCheck(const std::function<bool()> check,
            const std::function<void()> callback,
            const int timeoutPeriod,
            const std::function<void()> onFailure = std::function<void()>());

    /**
     * @brief  Checks if the object is currently waiting for a condition to
     *         occur.
     *
     * @return  Whether an ongoing condition check is running.
     */
    bool isChecking() const;

    /**
     * @brief  Sets how frequently the object should check for its condition to
     *         be met.
     *
     * @param interval    Frequency of checks in milliseconds. This value must
     *                    be greater than zero.
     *
     * @param multiplier  An optional multiplier to apply to the interval after
     *                    each failed test. A multiplier should be provided
     *                    when it's necessary for checks to become either more
     *                    or less frequent over time. The effects of the
     *                    multiplier on the interval will not persist between
     *                    condition checks. This value must be greater than
     *                    zero.
     */
    void setCheckInterval(const int interval, const float multiplier = 1);

    /**
     * @brief  Cancels any ongoing condition checking. This takes no action if
     *         no condition is being checked.
     *
     * @param runFailureCallback  If true, the condition's failure callback
     *                            will run if valid, as long as the optional
     *                            final test doesn't reveal that the condition
     *                            was met.
     *
     * @param runFinalTest        If true, the object will check one last time
     *                            if the condition has been met. If it
     *                            succeeds, the callback will run.
     */
    void cancelCheck(const bool runFailureCallback = false,
            const bool runFinalTest = false);

private:
    /**
     * @brief  Checks the condition, running the condition callback and
     *         clearing condition variables if the condition is met.
     *
     * @return  Whether the condition was met.
     */
    bool checkCondition();

    /**
     * @brief  Removes the conditionCheck test and all callback functions and
     *         stops and resets the timer.
     */
    void clearCheckValues();

    // Condition-specific functions:
    // Returns whether the condition has been met:
    std::function<bool()> conditionCheck;
    // The action to perform when the condition is met:
    std::function<void()> conditionCallback;
    // An optional action to perform if the condition times out:
    std::function<void()> failureCallback;

    // Condition interval and timeout settings:
    // Milliseconds to wait between condition checks:
    int checkInterval;
    // A multiplier applied to the checkInterval after each failed condition
    // check.
    float intervalMultiplier = 1;
    // The time(in milliseconds since the Unix epoch) to stop scheduling
    // condition checks and assume the condition will not be met:
    juce::int64 timeout;

    /**
     * @brief  Runs scheduled condition checks.
     */
    class CheckTimer : public juce::Timer
    {
    public:
        /**
         * @brief  Connects the timer to the ConditionChecker object that owns
         *         it.
         *
         * @param owner  The object that holds this timer.
         */
        CheckTimer(ConditionChecker& owner);

        virtual ~CheckTimer() { }

        /**
         * @brief  Starts periodically checking the test condition.
         */
        void startChecking();

    private:
        /**
         * @brief  Checks the condition, running callbacks or scheduling future
         *         checks as necessary.
         */
        virtual void timerCallback() override;

        /**
         * @brief  Sets the timer for the next condition check, ensuring that
         *         the timer stops at the timeout period if necessary.
         */
        void setCheckTimer();

        ConditionChecker& owner;
        // Holds the owner's interval value, multiplied by the owner's
        // intervalMultiplier once for each previous failed condition.
        int nextInterval;
    };
    CheckTimer timer;

    // Prevents the timer thread and condition functions from acting
    // simultaneously:
    juce::CriticalSection conditionLock;
};
