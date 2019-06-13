#pragma once
/**
 * @file  Output_Modifiers.h
 *
 * @brief  Defines all modifier key types.
 */

#include "JuceHeader.h"

namespace Output
{
    namespace Modifiers
    {
        enum TypeFlag
        {
            control = 0b0001,
            shift   = 0b0010,
            alt     = 0b0100,
            super   = 0b1000
        };

        /**
         * @brief  Gets the string representation of any combination of modifier
         *         types.
         *
         * @param modifierFlags  Any number of modifier values combined with the 
         *                       binary OR operator.
         *
         * @return               The string representation of the given modifier
         *                       set.
         */
        juce::String getModString(const int modifierFlags);

    }
}
