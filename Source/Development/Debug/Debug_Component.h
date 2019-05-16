#pragma once
/**
 * @file  Debug_Component.h
 *
 * @brief  Provides tools for debugging juce::Component objects.
 */

#include "JuceHeader.h"

namespace Debug
{
    namespace Component
    {
        /**
         * @brief  Prints the structure of the component tree to debug output.
         */
        void trace();
    }
}
