#pragma once
/**
 * @file  Text_CharSet_Values.h
 *
 * @brief  Keeps track of all permitted character values.
 */

#include "JuceHeader.h"
#include <map>

namespace Text
{
    namespace CharSet
    {
        namespace Values
        {
            /**
             * @brief  Gets the character value stored in a string loaded from a
             *         configuration file.
             *
             * @param  charString  A string representation of a character.
             *
             * @return             The character value used to represent that
             *                     character
             */
            unsigned int getCharValue(const juce::String charString);

            /**
             * @brief  Gets a character string representing a character index
             *         value.
             *
             * @param charValue  An ISO 8859 character code, or a replacement
             *                   value defined in Text::CharSet::Values.
             *
             * @return           The string representation of the character, or
             *                   the empty string if the character value is
             *                   not valid.
             */
            juce::String getCharString(const unsigned int charValue);

            /**
             * @brief  Checks if a value needs to be drawn twice as wide as
             *         normal.
             *
             * @param value  A ISO 8859 character code, or a replacement value
             *               defined in Text::CharSet::Values.
             *
             * @return       Whether the value requires extra width.
             */
            bool isWideValue(const unsigned int& value);

            // Indices of nonstandard characters, stored in place of unprintable
            // characters:

            // Arrows, outlines, whitespace, and filled areas:
            static const constexpr unsigned int outline     = 0x1;
            static const constexpr unsigned int fill        = 0x2;
            static const constexpr unsigned int backspace   = 0x3;
            static const constexpr unsigned int enter       = 0x4;
            static const constexpr unsigned int tab         = 0x5;
            static const constexpr unsigned int up          = 0x6;
            static const constexpr unsigned int down        = 0x7;
            static const constexpr unsigned int left        = 0x8;
            static const constexpr unsigned int right       = 0x9;
            
            // Function keys:
            static const constexpr unsigned int f1          = 0x80;
            static const constexpr unsigned int f2          = 0x81;
            static const constexpr unsigned int f3          = 0x82;
            static const constexpr unsigned int f4          = 0x83;
            static const constexpr unsigned int f5          = 0x84;
            static const constexpr unsigned int f6          = 0x85;
            static const constexpr unsigned int f7          = 0x86;
            static const constexpr unsigned int f8          = 0x87;
            static const constexpr unsigned int f9          = 0x88;
            static const constexpr unsigned int f10         = 0x89;
            static const constexpr unsigned int f11         = 0x8a;
            static const constexpr unsigned int f12         = 0x8b;

            // Double-wide characters:
            static const constexpr unsigned int ctrl        = 0x10;
            static const constexpr unsigned int alt         = 0x12;
            static const constexpr unsigned int shift       = 0x14;
            static const constexpr unsigned int escape      = 0x16;
            static const constexpr unsigned int cmd         = 0x18;
            static const constexpr unsigned int del         = 0x1a;
            static const constexpr unsigned int wideFill    = 0x1c;
            static const constexpr unsigned int wideOutline = 0x1e;
        }
    }
}
