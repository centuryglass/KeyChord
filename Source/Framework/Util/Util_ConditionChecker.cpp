#include "Util_ConditionChecker.h"
#include "Util_Math.h"

// Default condition checking interval in milliseconds:
static const constexpr int defaultInterval = 300;

// Minimum interval between tests:
static const constexpr int minInterval = 10;

// Initializes the internal timer and loads the default interval value on
// construction.
Util::ConditionChecker::ConditionChecker() : timer(*this),
    checkInterval(defaultInterval) { }


// Starts checking for a condition, unless already checking for another
// condition.
bool Util::ConditionChecker::startCheck(
        const std::function<bool()> check,
        const std::function<void()> callback,
        const int timeoutPeriod,
        const std::function<void()> onFailure)
{
    // Check and callback functions must be valid:
    jassert(check);
    jassert(callback);
    const juce::ScopedLock updateLock(conditionLock);
    if (!conditionCheck && check && callback)
    {
        conditionCheck = check;
        conditionCallback = callback;
        failureCallback = onFailure;
        if (timeoutPeriod < 0)
        {
            timeout = -1;
        }
        else
        {
            timeout = juce::Time::currentTimeMillis() + timeoutPeriod;
        }
        if (!checkCondition())
        {
            timer.startChecking();
        }
        return true;
    }
    return false;
}


// Checks if the object is currently waiting for a condition to occur.
bool Util::ConditionChecker::isChecking() const
{
    const juce::ScopedLock updateLock(conditionLock);
    // The connection checking function will be valid if and only if the
    // object is currently checking a condition.
    return (bool) conditionCheck;
}


// Sets how frequently the object should check for its condition to be met.
void Util::ConditionChecker::setCheckInterval
(const int interval, const float multiplier)
{
    const juce::ScopedLock updateLock(conditionLock);
    checkInterval = std::max(interval, minInterval);
    intervalMultiplier = (multiplier > 0) ? multiplier : 1;
}


// Cancels any ongoing condition checking. This takes no action if no condition
// is being checked.
void Util::ConditionChecker::cancelCheck
(const bool runFailureCallback, const bool runFinalTest)
{
    const juce::ScopedLock updateLock(conditionLock);
    if (conditionCheck)
    {
        if (runFinalTest)
        {
            checkCondition();
        }
        if (runFailureCallback && failureCallback)
        {
            failureCallback();
        }
    }
    clearCheckValues();
}


// Checks the condition, running the condition callback if the condition is met.
bool Util::ConditionChecker::checkCondition()
{
    if (conditionCheck())
    {
        conditionCallback();
        clearCheckValues();
        return true;
    }
    return false;
}


// Removes the conditionCheck test and all callback functions and stops and
// resets the timer.
void Util::ConditionChecker::clearCheckValues()
{
    conditionCheck = std::function<bool()>();
    conditionCallback = std::function<void()>();
    failureCallback = std::function<void()>();
    timeout = 0;
    timer.stopTimer();
}


// Connects the timer to the ConditionChecker object that owns it.
Util::ConditionChecker::CheckTimer::CheckTimer(ConditionChecker& owner) :
    owner(owner) { }


// Starts periodically checking the test condition.
void Util::ConditionChecker::CheckTimer::startChecking()
{
    // This should never be called while the timer is already running
    jassert(!isTimerRunning());
    nextInterval = owner.checkInterval;
    setCheckTimer();
}


// Checks the condition, running callbacks or scheduling future checks as
// necessary.
void Util::ConditionChecker::CheckTimer::timerCallback()
{
    const juce::ScopedLock timerLock(owner.conditionLock);
    stopTimer();
    jassert(owner.conditionCheck);
    if (!owner.checkCondition())
    {
        if (owner.timeout >= 0
                && juce::Time::currentTimeMillis() >= owner.timeout)
        {
            if (owner.failureCallback)
            {
                owner.failureCallback();
            }
            owner.clearCheckValues();
        }
        else
        {
            nextInterval *= owner.intervalMultiplier;
            setCheckTimer();
        }
    }
}


// Sets the timer for the next condition check, ensuring that the timer stops at
// the timeout period if necessary.
void Util::ConditionChecker::CheckTimer::setCheckTimer()
{
    juce::int64 interval = std::max(nextInterval, minInterval);
    if (owner.timeout >= 0)
    {
        interval = std::min(interval,
                owner.timeout - juce::Time::currentTimeMillis());
    }
    startTimer(interval);
}
