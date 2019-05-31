#pragma once
/**
 * @file  Text_CharSet_JSONKeys.h
 *
 * @brief  Defines keys used to load configurable character sets.
 */

#include "Config_DataKey.h"

namespace Text { namespace CharSet { namespace JSONKeys
{
    // ####### String value keys: #######
    // Stores the names used to identify character sets on the help screen:
    const Config::DataKey mainSetName("main character set name",
            Config::DataKey::DataType::stringType);
    const Config::DataKey altSetName("alternate character set name",
            Config::DataKey::DataType::stringType);
    const Config::DataKey specialSetName("special character set name",
            Config::DataKey::DataType::stringType);

    // ####### Character set keys: #######
    //  Each stores a set of character values in display order, paired with the
    // characters that replace them when the shift modifier is active, and the
    // priority they should take when assigning chord values.
    const juce::Identifier mainCharSet("main character set");
    const juce::Identifier altCharSet("alternate character set");
    const juce::Identifier specialCharSet("special character set");
} } }
