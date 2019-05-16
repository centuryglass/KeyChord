#include "Windows_FocusedTimer.h"

// Stops the timer if it is active, or cancels it if it is suspended.
void Windows::FocusedTimer::stopTimer()
{
    juce::Timer::stopTimer();
    suspendedEndTime = 0;
}


// Starts the timer, setting the amount of time until the timer's callback
// function will run.
void Windows::FocusedTimer::startTimer(const int timerMilliseconds)
{
    if (getFocusState())
    {
        Timer::startTimer(timerMilliseconds);
    }
    else
    {
        suspendedEndTime = juce::Time::getMillisecondCounter()
                + timerMilliseconds;
        onSuspend();
    }
}


// Suspends the timer when the window loses focus.
void Windows::FocusedTimer::windowFocusLost()
{
    if (isTimerRunning())
    {
        juce::uint32 endTime = juce::Time::getMillisecondCounter()
                + getTimerInterval();
        stopTimer();
        suspendedEndTime = endTime;
        onSuspend();
    }
}


// Resumes the timer when the window gains focus.
void Windows::FocusedTimer::windowFocusGained()
{
    if (suspendedEndTime > 0)
    {
        const juce::uint32 now = juce::Time::getMillisecondCounter();
        if (now > suspendedEndTime)
        {
            timerCallback();
        }
        else
        {
            juce::uint32 timeLeft = suspendedEndTime - now;
            startTimer(timeLeft);
        }
        suspendedEndTime = 0;
    }
}
