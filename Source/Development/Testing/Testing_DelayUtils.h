#pragma once
#include "JuceHeader.h"

/**
 * @file  Testing_DelayUtils.h
 *
 * @brief  Handles tests that require waiting until some particular event is
 *         true.
 */
namespace Testing { class DelayUtils; }

/**
 * @brief  Provides the static idleUntil function used to run the JUCE event
 *         loop until some condition is met or times out.
 */
class Testing::DelayUtils
{
public:
    /**
     * @brief  Runs the juce dispatch loop until an expected condition is true.
     *
     * @param waitCondition     Tests if the expected condition is true yet.
     *
     * @param testFrequencyMS   Milliseconds to wait between tests of the
     *                          expected condition.
     *
     * @param timeoutMS         Milliseconds to wait before assuming the
     *                          condition will never be met.
     *
     * @return                  Whether waitCondition returned true before the
     *                          timeout period ended.
     */
    static bool idleUntil(const std::function<bool()> waitCondition,
            int testFrequencyMS, int timeoutMS);
};

