#pragma once
#include "JuceHeader.h"

namespace Text
{
    namespace Painter
    {
        /**
         * @brief  Paints a single character value using Text::BinaryFont.
         *
         * @param g        JUCE graphics context used for drawing.
         *
         * @param toPrint  ISO 8859 index of the character to draw.
         *
         * @param x        X coordinate where the character will be drawn.
         *
         * @param y        Y coordinate where the character will be drawn.
         *
         * @param width    Width of the drawn character.
         *
         * @param height   Height of the drawn character.
         */
        void paintChar(juce::Graphics& g,
                const unsigned int toPrint,
                const int x,
                const int y,
                const int width,
                const int height);

        /**
         * @brief  Draws an entire string using Text::BinaryFont.
         *
         * @param g              JUCE graphics context used for drawing.
         *
         * @param toPrint        ISO 8859 character value array containing the
         *                       string to draw.
         *
         * @param x              X coordinate where the string will be drawn.
         *
         * @param y              Y coordinate where the string will be drawn.
         *
         * @param width          Width of the area where the string
         *
         * @param height         Height of the drawn character.
         *
         * @param maxCharSize    Maximum size to draw characters.
         *
         * @param maxRowCount    Maximum number of rows to split the string 
         *                       across if necessary.
         */
        void paintString(juce::Graphics& g,
                const juce::Array<unsigned int> charIndices,
                const int x,
                const int y,
                const int width,
                const int height,
                const int maxCharSize,
                const int maxRowCount = 1);
    }
}
