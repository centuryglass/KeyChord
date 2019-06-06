#pragma once
/**
 * @file  Text_Values.h
 *
 * @brief  Keeps track of all permitted character values.
 */

#include "Text_CharTypes.h"
#include "JuceHeader.h"
#include <map>

namespace Text
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
        CharValue getCharValue(const juce::String charString);

        /**
         * @brief  Gets a character string representing a character index
         *         value, usable with xdotool.
         *
         * @param charValue  An ISO 8859 character code, or a replacement
         *                   value defined in Text::Values.
         *
         * @return           An Xqq string representation of the character, or
         *                   the empty string if the character value is
         *                   not valid.
         */
        juce::String getXString(const CharValue charValue);

        /**
         * @brief  Checks if a value needs to be drawn twice as wide as
         *         normal.
         *
         * @param value  An ISO 8859 character code, or a replacement value
         *               defined in Text::Values.
         *
         * @return       Whether the value requires extra width.
         */
        bool isWideValue(const CharValue& value);

        /**
         * @brief  Checks if a value represents a modifier, rather than a
         *         typical printed character.
         *
         * @param value  An ISO 8859 character code, or a replacement value
         *               defined in Text::Values.
         *
         * @return       Whether the value represents the control, alt,
         *               shift, or command keys.
         */
        bool isModifier(const CharValue& value);

        // Indices of nonstandard characters, stored in place of unprintable
        // characters:

        // Arrows, outlines, whitespace, and filled areas:
        static const constexpr CharValue outline     = 0x1;
        static const constexpr CharValue fill        = 0x2;
        static const constexpr CharValue backspace   = 0x3;
        static const constexpr CharValue enter       = 0x4;
        static const constexpr CharValue tab         = 0x5;
        static const constexpr CharValue up          = 0x6;
        static const constexpr CharValue down        = 0x7;
        static const constexpr CharValue left        = 0x8;
        static const constexpr CharValue right       = 0x9;
        
        // Function keys:
        static const constexpr CharValue f1          = 0x80;
        static const constexpr CharValue f2          = 0x81;
        static const constexpr CharValue f3          = 0x82;
        static const constexpr CharValue f4          = 0x83;
        static const constexpr CharValue f5          = 0x84;
        static const constexpr CharValue f6          = 0x85;
        static const constexpr CharValue f7          = 0x86;
        static const constexpr CharValue f8          = 0x87;
        static const constexpr CharValue f9          = 0x88;
        static const constexpr CharValue f10         = 0x89;
        static const constexpr CharValue f11         = 0x8a;
        static const constexpr CharValue f12         = 0x8b;

        // Double-wide characters:
        static const constexpr CharValue ctrl        = 0x10;
        static const constexpr CharValue alt         = 0x12;
        static const constexpr CharValue shift       = 0x14;
        static const constexpr CharValue cmd         = 0x16;
        static const constexpr CharValue escape      = 0x18;
        static const constexpr CharValue del         = 0x1a;
        static const constexpr CharValue wideFill    = 0x1c;
        static const constexpr CharValue wideOutline = 0x1e;

        // Bounds to use when recognizing normal ASCII printable characters:
        static const constexpr CharValue normalPrintMin = 0x20; 
        static const constexpr CharValue normalPrintMax = 0x7f; 

        // Bounds to use when recognizing extended printable characters:
        static const constexpr CharValue extraPrintMin = 0xa1;
        static const constexpr CharValue extraPrintMax = 0xff;
    }
}
