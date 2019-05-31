#include "Text_Painter.h"
#include "Text_BinaryFont.h"
#include "Text_CharSet_Values.h"
#include <map>

// Character width in bits/pixels
static const constexpr int charSize = 10;

// Minimum buffer between text rows, as a fraction of height:
static const constexpr float minRowPadding = 0.1;

// Maximum buffer between text rows, as a fraction of height:
static const constexpr float maxRowPadding = 0.2;


void Text::Painter::paintChar(juce::Graphics& g, const unsigned int toPrint,
        const int x, const int y, const int width, const int height)
{
    const bool doubleWidth = Text::CharSet::Values::isWideValue(toPrint);
    const int charWidth = doubleWidth ? charSize * 2 : charSize;
    const int pixelWidth = std::max(1, width / charWidth);
    const int pixelHeight = std::max(1, height / charSize);
    using juce::uint64;
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
                pixelsToDraw = 0;
            }
        }
    }
}


// Draws an entire string using Text::BinaryFont.
void Text::Painter::paintString(juce::Graphics& g,
        const juce::Array<unsigned int> charIndices,
        const int x,
        const int y,
        const int width,
        const int height,
        const int maxCharSize,
        const int maxRowCount)
{
    jassert(maxRowCount > 0); // You can't have less than one row...

    // In strings, overlap characters by the scaled equivalent of two pixels:
    const float widthOverlapAdjustment = 0.8;
    // Check if text will fit without any obnoxious layout adjustment:
    const int strLength = charIndices.size();
    int charSize = std::min(height, maxCharSize);

    int rowCount = 1;
    int skippedChars = 0;

    while (charSize * ((strLength / rowCount) + (strLength % rowCount))
            * widthOverlapAdjustment > width)
    {
        // Try increasing padding:
        float paddingNeeded = width * strLength * widthOverlapAdjustment
                / height / rowCount;
        if (paddingNeeded <= (maxRowPadding))
        {
            if (paddingNeeded < minRowPadding)
            {
                paddingNeeded = minRowPadding;
            }
            charSize = std::min<int>(height * (1.0 - paddingNeeded) / rowCount,
                    maxCharSize);
        }
        // Try increasing row count:
        else if (rowCount < maxRowCount)
        {
            ++rowCount;
            charSize = std::min<int>((height * (1.0 - minRowPadding) 
                    / rowCount), maxCharSize);
        }
        // No way to make it fit, cut characters:
        else
        {
            const int widthNeeded = charSize * strLength 
                    * widthOverlapAdjustment;
            const int overflow = widthNeeded - width * rowCount;
            skippedChars = overflow / charSize / widthOverlapAdjustment + 1;
            break;
        }
    }

    const int paddingSize = (height / rowCount - charSize) / 2;
    const int charXOffset = charSize * widthOverlapAdjustment;
    int rowIndex = 0;
    int xPos = x;
    int yPos = y;
    DBG("X=" << xPos << ", Y=" << yPos << ", size=" << charSize << ", padding="
            << paddingSize << ", rows=" << rowCount << ", skippedChars="
            << skippedChars);
    for (int i = skippedChars; i < strLength; i++)
    {
        if ((xPos + charSize) > (x + width))
        {
            rowIndex++;
            jassert(rowIndex < maxRowCount);
            xPos = x;
            yPos += (height / rowCount);
        }
        //g.drawRect(xPos, yPos, charSize, charSize, paddingSize);
        paintChar(g, charIndices[i], xPos, yPos, charSize, charSize);
        xPos += charXOffset;
    }


}
