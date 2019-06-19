#include "Text_Painter.h"
#include "Text_BinaryFont.h"
#include "Text_Values.h"
#include "Util_Math.h"
#include <map>

// Character width in bits/pixels
static const constexpr int charSize = 10;
// Number of(scaled) blank pixels to insert between characters:
static const constexpr int charPixelPadding = 1;
// Number of(scaled) blank pixels to represent whitespace characters:
static const constexpr int whitespaceWidth = 2;


#ifdef JUCE_DEBUG
// Print the full namespace name before all debug output:
static const constexpr char* dbgPrefix = "Text::Painter::";
#endif

// Returns the leftmost and rightmost horizontal indices of the area in a
// character that actually contain pixels, or { -1, -1} if the character is
// empty.
static std::pair<int, int> charBounds(const Text::CharValue toMeasure)
{
    const bool doubleWidth = Text::Values::isWideValue(toMeasure);
    const int charWidth = doubleWidth ? charSize * 2 : charSize;

    std::pair<int, int> bounds = {-1, -1 };

    for (int row = 0; row < charSize; row++)
    {
        juce::uint32 rowPixels;
        if (doubleWidth)
        {
            rowPixels = Text::BinaryFont::getDoubleCharRow(toMeasure, row);
        }
        else
        {
            rowPixels = Text::BinaryFont::getCharacterRow(toMeasure, row);
        }
        for (int xPixel = 0; xPixel < charWidth; xPixel++)
        {
            if ((xPixel >= bounds.first && bounds.first != -1)
                    && xPixel <= bounds.second)
            {
                continue;
            }
            const bool pixelFound = ((1 << (charWidth - xPixel - 1))
                    & rowPixels);
            if (pixelFound)
            {
                if ((bounds.first < 0) || bounds.first > xPixel)
                {
                    bounds.first = xPixel;
                }
                if (bounds.second <= xPixel)
                {
                    bounds.second = xPixel + 1;
                }
            }
        }
    }
    if (doubleWidth)
    {
        DBG(dbgPrefix << __func__ << ": bounds of " << juce::String(toMeasure)
                << " are " << bounds.first << " to " <<bounds.second);
    }
    return bounds;
}

int Text::Painter::paintChar(juce::Graphics& g, const CharValue toPrint,
        int x, int y, int width, int height, const bool preserveAspectRatio)
{
    const bool doubleWidth = Text::Values::isWideValue(toPrint);
    if (preserveAspectRatio)
    {
        // Regular character w:h ratio is 1:1, doubleWidth characters are 2:1
        if (width > (doubleWidth ? height * 2 : height))
        {
            int reduction = width - (doubleWidth ? height * 2 : height);
            width -= reduction;
            x += reduction / 2;
        }
        else if (height > (doubleWidth ? width / 2 : width))
        {
            int reduction = height - (doubleWidth ? width / 2 : width);
            height -= reduction;
            y += reduction / 2;
        }
    }
    const int charWidth = doubleWidth ? charSize * 2 : charSize;
    const int pixelWidth = std::max(1, width / charWidth);
    const int pixelHeight = std::max(1, height / charSize);

    int rightmost = x;

    for (int row = 0; row < charSize; row++)
    {
        juce::uint32 rowPixels;
        if (doubleWidth)
        {
            rowPixels = Text::BinaryFont::getDoubleCharRow(toPrint, row);
        }
        else
        {
            rowPixels = Text::BinaryFont::getCharacterRow(toPrint, row);
        }
        int pixelsToDraw = 0;
        for (int xPixel = 0; xPixel < charWidth; xPixel++)
        {
            const bool pixelFound = ((1 << (charWidth - xPixel - 1))
                    & rowPixels);
            if (pixelFound)
            {
                pixelsToDraw++;
            }
            if ((pixelsToDraw > 0) && (xPixel == (charWidth - 1)
                        || !pixelFound))
            {
                const int xPos = (xPixel - (pixelFound ? pixelsToDraw
                        : pixelsToDraw + 1)) * pixelWidth + x;
                const int yPos = row * pixelHeight + y;
                const int width = pixelsToDraw * pixelWidth;
                g.fillRect(xPos, yPos, width, pixelHeight);
                rightmost = std::max(rightmost, xPos + width + 1);
                pixelsToDraw = 0;
            }
        }
    }
    return rightmost;
}


// Draws an entire string using Text::BinaryFont.
int Text::Painter::paintString(juce::Graphics& g,
        const CharString toPrint,
        const int x,
        const int y,
        const int width,
        const int height,
        const int maxCharSize)
{
    // Map character widths:
    std::map<CharValue, int> drawnWidths;
    std::map<CharValue, std::pair<int, int>> borders;
    int widthSum = 0;
    for (const CharValue& charIndex : toPrint)
    {
        try
        {
            widthSum += drawnWidths.at(charIndex);
        }
        catch(const std::out_of_range& e)
        {
            const std::pair<CharValue, int> border = charBounds(charIndex);
            int width = border.second - border.first;
            if (width == 0)
            {
                width = whitespaceWidth;
            }
            width += charPixelPadding;
            drawnWidths[charIndex] = width;
            borders[charIndex] = border;
            widthSum += width;
        }
    }

    float pixelSize = Util::Math::median<float>(1.0f,
            (float) width / (float) widthSum,
            (float) maxCharSize / (float) charSize);
    pixelSize = std::min(pixelSize, (float) height / float(charSize));

    int xPos = x + pixelSize;
    for (int i = 0; i < toPrint.size(); i++)
    {
        const CharValue& charIndex = toPrint[i];
        const int xStart = borders[charIndex].first;
        if (i > 0 && xStart >= 0)
        {
            xPos -= (xStart - 1) * pixelSize;
        }
        xPos = pixelSize * charPixelPadding
                + paintChar(g, charIndex, xPos, y, pixelSize * charSize,
                pixelSize * charSize);
    }
    return xPos;
}
