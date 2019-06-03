#pragma once
/**
 * @file  Text_Charset_Type.h
 *
 * @brief  Defines the types of available keyboard character sets. 
 */

namespace Text 
{
    namespace CharSet 
    { 
        enum class Type
        {
            main,         // main set, lowercase letters by default.
            alt,          // alternate set, number values by default.
            special,      // special character set
            modifier,     // modifier keys
        };
        static const constexpr int numCharacterSets = 4;
    } 
}
