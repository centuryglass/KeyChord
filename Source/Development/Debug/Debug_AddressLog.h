#pragma once
/**
 * @file  Debug_AddressLog.h
 *
 * @brief  Keeps and prints logs of events associated with specific memory
 *         address values.
 */

#include "JuceHeader.h"

namespace Debug
{
    namespace AddressLog
    {
        /**
         * @brief  Gets a unique, fixed ID value to represent a particular
         *         memory address. Nullptr will always have ID 0.
         *
         * @param address  Any stored memory address.
         *
         * @return         An ID number unique to the given pointer.
         */
        int getID(const void* address);

        /**
         * @brief  Appends a line of text to the log of events occurring to a
         *         specific address.
         *
         * @param address   Any stored memory address.
         *
         * @param event     A description of some event that occurred involving
         *                  the address.
         *
         * @param address2  An optional second pointer involved in the event.
         *                  If non-null, its ID will be appended onto the end
         *                  of the event log.
         *
         * @return          A reference to the full event log for the address.
         */
        const juce::String& addEvent(const void* address,
                const juce::String event, const void* address2 = nullptr);

        /**
         * @brief  Prints all logged events for a specific memory address
         *
         * @param addressID  The address being debugged.
         */
        void printLog(int addressID);
    }
}
