#include "Util_TempTimer.h"

// Starts a new timer.
void Util::TempTimer::initTimer(const int runTimeMS,
        const std::function<void()> callback)
{
    new TempTimer(runTimeMS, callback);
}


// Starts the timer on construction.
Util::TempTimer::TempTimer
(const int runTimeMS, const std::function<void()> callback) :
callback(callback)
{
    selfRef = this;
    startTimer(runTimeMS);
}


// Runs the timer callback function, and asynchronously deletes the timer.
void Util::TempTimer::timerCallback()
{
    stopTimer();
    callback();
    // Can't delete this timer while running its callback, so schedule
    // it to delete asynchronously.
    juce::MessageManager::callAsync([this]()
    {
        selfRef = nullptr;
    });
}
