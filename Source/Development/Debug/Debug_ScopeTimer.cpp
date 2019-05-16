#include "Debug_ScopeTimer.h"
#include "Debug_ScopeTimerRecords.h"


// Starts the timer on construction.
Debug::ScopeTimer::ScopeTimer(const juce::String scopeName) :
scopeName(scopeName),
threadID(juce::Thread::getCurrentThreadId())
{
    ScopeTimerRecords::addStartRecord(scopeName, threadID);
}


// Saves the total amount of time this object existed before it is destroyed.
Debug::ScopeTimer::~ScopeTimer()
{
    ScopeTimerRecords::addStopRecord(scopeName, threadID);
}
