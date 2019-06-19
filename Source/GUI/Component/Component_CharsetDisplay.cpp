#include "Component_CharsetDisplay.h"
#include "Component_ColourIds.h"
#include "Text_CharTypes.h"
#include "Text_Painter.h"
#include "Text_CharSet_Cache.h"
#include "Text_CharSet_ConfigFile.h"
#include "Text_Values.h"


// Gets the number of character columns the KeyGrid contains.
int Component::CharsetDisplay::getColumnCount() const
{
    const Text::CharSet::Cache* activeSet = getActiveSet();
    if (activeSet == nullptr)
    {
        return 1;
    }
    return activeSet->getSize() + activeSet->wideDrawCharacterCount();
}


// Gets the number of character rows the KeyGrid contains.
int Component::CharsetDisplay::getRowCount() const
{
    return 1;
}


// Draws all characters in the current active set.
void Component::CharsetDisplay::paint(juce::Graphics& g)
{
    if (getActiveSet() == nullptr)
    {
        return;
    }

    using juce::uint8;
    using juce::Colour;
    using juce::Colours;
    using juce::Rectangle;
    using namespace Text;

    // Calculate layout values:
    const int paddedCharWidth = getPaddedCharWidth();
    const int paddedRowHeight = getPaddedRowHeight();
    const int xPadding = getXPadding();
    const int yPadding = getYPadding();
    const int rowHeight = paddedRowHeight - yPadding;
    const int charWidth = paddedCharWidth - xPadding;
    const Text::CharSet::Cache* activeSet = getActiveSet();

    // Check and save whether the shifted character set is in use:
    Text::CharSet::ConfigFile charsetConfig;
    const bool isShifted = charsetConfig.getShifted();

    // Center all content within the available space:
    const int xStart = (getWidth() % getColumnCount()) / 2;
    int xPos = xStart;
    int yPos = 0;

    // A convenience function to more easily request character drawing
    // operations:
    const auto drawChar =
        [&g, &xPos, &yPos, charWidth, rowHeight, xPadding, yPadding]
        (const Text::CharValue toDraw)
    {
        bool wideDrawChar = Values::isWideValue(toDraw);
        Text::Painter::paintChar(g, toDraw, xPos, yPos,
                (wideDrawChar ? charWidth * 2 : charWidth), rowHeight,
                true);
    };

    // Label and draw chords for each possible character:
    for (int i = 0; i < activeSet->getSize(); i++)
    {
        // Current character set index:
        const Text::CharValue charIndex
                = activeSet->getCharAtIndex(i, isShifted);
        // Whether the character needs double the normal width:
        const bool wideDrawChar = Text::Values::isWideValue(charIndex);
        // Binary mask for the chord used to type the character:
        const Input::Chord characterChord
                = activeSet->getCharacterChord(charIndex);
        // Whether this character is currently selected:
        const bool charSelected = (characterChord == getHeldChord());
        // Whether no chord keys are held that aren't in this character's chord:
        const bool charOpen = charSelected || getHeldChord().isSubchordOf(
                characterChord);

        g.setColour(findColour(charOpen ? text : inactiveText, true));
        if (charSelected)
        {
            drawChar(wideDrawChar ? Values::wideOutline : Values::outline);
        }
        drawChar(charIndex);

        if (wideDrawChar)
        {
            xPos += paddedCharWidth - xPadding;
        }

        xPos += paddedCharWidth;
    }
}
