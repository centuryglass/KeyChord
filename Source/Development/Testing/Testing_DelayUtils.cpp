#include "Testing_DelayUtils.h"

// Runs the juce dispatch loop until an expected condition is true.
bool Testing::DelayUtils::idleUntil
(const std::function<bool()> waitCondition, int testFrequencyMS, int timeoutMS)
{
    juce::int64 startTime = juce::Time::currentTimeMillis();
    while (juce::Time::currentTimeMillis() < (startTime + timeoutMS))
    {
        if (waitCondition())
        {
            return true;
        }
        juce::MessageManager::getInstance()->runDispatchLoopUntil
            (testFrequencyMS);
    }
    return false;
}
