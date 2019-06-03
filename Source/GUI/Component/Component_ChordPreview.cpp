#include "Component_ChordPreview.h"
#include "Component_ColourIds.h"
#include "Text_ModTracker.h"
#include "Text_Painter.h"
#include "Text_CharSet_Values.h"
#include "Text_CharSet_ConfigFile.h"
#include "Text_CharTypes.h"

Component::ChordPreview::ChordPreview() { }


// Gets the number of character columns the KeyGrid contains.
int Component::ChordPreview::getColumnCount() const
{
    const Text::CharSet::Cache* activeSet = getActiveSet();
    if (activeSet == nullptr)
    {
        return 0;
    }
    return activeSet->getSize() + activeSet->wideDrawCharacterCount();
}


// Gets the number of character rows the KeyGrid contains.
int Component::ChordPreview::getRowCount() const
{
    return Chord::numChordKeys();
}


// Draws all chord mappings within the current character set.
void Component::ChordPreview::paint(juce::Graphics& g)
{
    if (getActiveSet() == nullptr)
    {
        return;
    }

    using juce::uint8;
    using juce::Colour;
    using juce::Colours;
    using juce::Rectangle;
    using namespace Text::CharSet;

    // Save the character set size:
    const int charCount = getActiveSet()->getSize() 
            + getActiveSet()->wideDrawCharacterCount();

    // Calculate layout values:
    const int paddedCharWidth = getPaddedCharWidth();
    const int paddedRowHeight = getPaddedRowHeight();
    const int xPadding = getXPadding();
    const int yPadding = getYPadding();
    const int rowHeight = paddedRowHeight - yPadding;
    const int charWidth = paddedCharWidth - xPadding;
    const Text::CharSet::Cache* activeSet = getActiveSet();

    Text::CharSet::ConfigFile charsetConfig;
    bool isShifted = charsetConfig.getShifted();

    const int xStart = xPadding;
    const int yStart = yPadding;
    int xPos = xStart;
    int yPos = yStart;
    
    // A convenience function to more easily request character drawing 
    // operations:
    const auto drawChar = 
        [&g, &xPos, &yPos, charWidth, rowHeight, xPadding, yPadding]
        (const Text::CharValue toDraw)
    {
        bool wideDrawChar = Values::isWideValue(toDraw);
        Text::Painter::paintChar(g, toDraw, xPos + xPadding, yPos + yPadding,
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
        const bool wideDrawChar = Text::CharSet::Values::isWideValue(charIndex);
        // Binary mask for the chord used to type the character:
        const Chord characterChord = activeSet->getCharacterChord(charIndex);
        // Whether this character is currently selected:
        const bool charSelected = (characterChord == getHeldChord());
        // Whether no chord keys are held that aren't in this character's chord:
        const bool charOpen = charSelected || getHeldChord().isSubchordOf(
                characterChord);

        // Draw each chord key under the character:
        for(int keyIdx = 0; keyIdx < Chord::numChordKeys(); keyIdx++)
        {
            // Check if this chord key is currently held down:
            const bool keyIsHeld = getHeldChord().usesChordKey(keyIdx);
            // Check if this chord key is used for this character:
            const bool charUsesKey = characterChord.usesChordKey(keyIdx);

            // Select the color used to draw the character:
            int colourID;
            // If key is used for this character, check if selected, active,
            // open, or blocked:
            if (charSelected)
            {
                colourID = chord1Selected;
            }
            else if (! charOpen)
            {
                colourID = chord1Blocked;
            }
            else if (keyIsHeld)
            {
                colourID = chord1Active;
            }
            else
            {
                colourID = chord1Open;
            }
            if (charUsesKey)
            {
                colourID += keyIdx;
            }
            else
            {
                colourID += (emptySelected - chord1Selected);
            }
            g.setColour(findColour(colourID, true));
            if (charUsesKey)
            {
                drawChar(wideDrawChar ? Values::wideFill : Values::fill);
            }
            else
            {
                drawChar(wideDrawChar ? Values::wideOutline : Values::outline);
            }
            yPos += paddedRowHeight;
        }
        if (wideDrawChar)
        {
            xPos += paddedCharWidth - xPadding;
        }

        xPos += paddedCharWidth;
        yPos = yStart;
    }
}
