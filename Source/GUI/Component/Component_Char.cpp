#include "Component_Char.h"
#include "Text_BinaryFont.h"
#include <map>

// Character width in bits/pixels
static const constexpr int charSize = 10;


void Component::Char::paintAt(juce::Graphics& g, const char toPrint,
        const int x, const int y, const int width, const int height)
{
    const int pixelWidth = std::max(1, width / charSize);
    const int pixelHeight = std::max(1, height / charSize);
    if (pixelWidth <= 0 || pixelHeight <= 0)
    {
        jassertfalse;
        return;
    }
    const bool usePixels = true;
    //const bool usePixels = pixelWidth <= maxPixel && pixelHeight <= maxPixel;
    try
    {
        using juce::uint64;
        for (int row = 0; row < charSize; row++)
        {
            juce::uint16 rowPixels
                    = Text::BinaryFont::getCharacterRow(toPrint, row);
            int pixelsToDraw = 0;
            for (int xPixel = 0; xPixel < charSize; xPixel++)
            {
                const bool pixelFound = ((1 << (charSize - xPixel - 1)) 
                        & rowPixels);
                if (pixelFound)
                {
                    pixelsToDraw++;
                }
                if ((pixelsToDraw > 0) && (xPixel == (charSize - 1) 
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
    catch (std::out_of_range& e)
    {
        DBG("Missing char " << toPrint << " (" << (int) toPrint << ")");
        return;
    }
}
