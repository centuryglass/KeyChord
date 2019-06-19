/**
 * @file  Text_BinaryFont.h
 *
 * @brief  Loads low-res character image data encoded in binary.
 */

#include "Text_CharTypes.h"
#include "JuceHeader.h"

namespace Text
{
    namespace BinaryFont
    {
        // The width and height of each character:
        static const constexpr int charSize = 10;

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
        juce::uint16 getCharacterRow(const CharValue character,
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
        juce::uint32 getDoubleCharRow(const CharValue character,
                const int row);
    }
}
