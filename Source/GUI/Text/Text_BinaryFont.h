/**
 * @file  Text_BinaryFont.h
 *
 * @brief  Loads low-res character image data encoded in binary.
 */

#include "JuceHeader.h"

namespace Text 
{ 
    namespace BinaryFont 
    {
        // The width and height of each character:
        static const constexpr int charSize = 10;

        // Indices of nonstandard characters, stored in place of unprintable
        // characters.

        // Arrows, outlines and filled areas:
        static const constexpr juce::juce_wchar outline     = 0x1;
        static const constexpr juce::juce_wchar fill        = 0x2;
        static const constexpr juce::juce_wchar left        = 0x3;
        static const constexpr juce::juce_wchar up          = 0x4;
        static const constexpr juce::juce_wchar down        = 0x5;
        static const constexpr juce::juce_wchar right       = 0x6;
        static const constexpr juce::juce_wchar newline     = 0x7;
        
        // Function keys:
        static const constexpr juce::juce_wchar f1          = 0x80;
        static const constexpr juce::juce_wchar f2          = 0x81;
        static const constexpr juce::juce_wchar f3          = 0x82;
        static const constexpr juce::juce_wchar f4          = 0x83;
        static const constexpr juce::juce_wchar f5          = 0x84;
        static const constexpr juce::juce_wchar f6          = 0x85;
        static const constexpr juce::juce_wchar f7          = 0x86;
        static const constexpr juce::juce_wchar f8          = 0x87;
        static const constexpr juce::juce_wchar f9          = 0x88;
        static const constexpr juce::juce_wchar f10         = 0x89;
        static const constexpr juce::juce_wchar f11         = 0x8a;
        static const constexpr juce::juce_wchar f12         = 0x8b;

        // Double-wide characters:
        static const constexpr juce::juce_wchar wideOutline = 0x8;
        static const constexpr juce::juce_wchar wideFill    = 0xa;
        static const constexpr juce::juce_wchar del         = 0xc;
        static const constexpr juce::juce_wchar tab         = 0xe;
        static const constexpr juce::juce_wchar ctrl        = 0x10;
        static const constexpr juce::juce_wchar alt         = 0x12;
        static const constexpr juce::juce_wchar shift       = 0x14;
        static const constexpr juce::juce_wchar esc         = 0x16;
        static const constexpr juce::juce_wchar cmd         = 0x18;

        /**
         * @brief  Gets one row of image data for a specific character.
         *
         * @param character  Either a standard printable character within the
         *                   ISO-8859 character set, or one of the nonstandard
         *                   character values defined in this namespace
         *
         * @param row        The index of a row of pixels in the character
         *                   image, ordered from top to bottom.
         *                   
         * @return           A bitmap storing black and white image data for
         *                   that row.
         */
        juce::uint16 getCharacterRow(const juce::juce_wchar character,
                const int row);

        /**
         * @brief  Gets one row of image data for a double-wide character.
         *
         * @param character  One of the double-wide character values defined
         *                   in this namespace.
         *
         * @param row        The index of a row of pixels in the character
         *                   image, ordered from top to bottom.
         *                   
         * @return           A bitmap storing black and white image data for
         *                   that row.
         */
        juce::uint32 getDoubleCharRow(const juce::juce_wchar character,
                const int row);
    }
}
