#pragma once
/**
 * @file  Debug_ScopeTimerRecords.h
 *
 * @brief  Records and organizes times recorded by all ScopeTimer objects.
 */

#include "JuceHeader.h"

namespace Debug
{
    namespace ScopeTimerRecords
    {
        /**
         * @brief  Records the start of a ScopeTimer's measurement period.
         *
         * @param scopeName  A name identifying the measured scope.
         *
         * @param threadID   The thread where the timer was created and
         *                   destroyed.
         */
        void addStartRecord(const juce::String scopeName,
                const juce::Thread::ThreadID threadID);

        /**
         * @brief  Records the end of a ScopeTimer's measurement period.
         *
         * @param scopeName  A name identifying the measured scope.
         *
         * @param threadID   The thread where the timer was created and
         *                   destroyed.
         */
        void addStopRecord(const juce::String scopeName,
                const juce::Thread::ThreadID threadID);

        /**
         * @brief  Organizes and prints all records chronologically.
         */
        void printRecords();
    }
}
