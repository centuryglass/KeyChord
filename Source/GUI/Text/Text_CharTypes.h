#pragma once
/**
 * @file  Text_CharTypes.h
 *
 * @brief  Defines types used to handle binary character data.
 */

#include "JuceHeader.h"

namespace Text
{
    
    /**
     * @brief  Either the value of a printable ISO 8859 character value, or one
     *         of the special values assigned in Text::Values;
     */
    typedef unsigned int CharValue;

    /**
     * @brief  A string made up of CharValues.
     */
    typedef juce::Array<CharValue> CharString;

    /**
     * @brief  An array of CharString lines.
     */
    typedef juce::Array<CharString> CharLineArray;
}
