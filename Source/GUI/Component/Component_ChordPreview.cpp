#include "Component_ChordPreview.h"
#include "Component_ColourIds.h"
#include "Text_Painter.h"
#include "Text_Values.h"
#include "Text_CharSet_ConfigFile.h"
#include "Text_CharTypes.h"

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
    return Input::Chord::numChordKeys();
}


// Draws all chord mappings within the current character set.
void Component::ChordPreview::paint(juce::Graphics& g)
{
    if (getActiveSet() == nullptr)
    {
        return;
    }

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

    Text::CharSet::ConfigFile charsetConfig;
    bool isShifted = charsetConfig.getShifted();

    // Center all columns within the available space:
    const int xStart = (getWidth() % getColumnCount()) / 2;
    const int yStart = (getHeight() % getRowCount()) / 2;
    int xPos = xStart;
    int yPos = yStart;

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
        // The chord used to type the character:
        const Input::Chord characterChord
                = activeSet->getCharacterChord(charIndex);
        // Whether this character is currently selected:
        const bool charSelected = (characterChord == getHeldChord());
        // Whether all held chord keys are in this character's chord:
        const bool charOpen = charSelected || getHeldChord().isSubchordOf(
                characterChord);

        // Draw each chord key under the character:
        for (int keyIdx = 0; keyIdx < Input::Chord::numChordKeys(); keyIdx++)
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
                // Selected: Releasing all chord keys will select the current
                //           character.
                colourID = chord1Selected;
            }
            else if (! charOpen)
            {
                // Blocked: A chord key is held down that isn't used by the
                //          current character.
                colourID = chord1Blocked;
            }
            else if (keyIsHeld)
            {
                // Active: Not the selected character, but the chord key is held
                //         down and only chord keys that this character uses are
                //         held down.
                colourID = chord1Active;
            }
            else
            {
                // Open: same as active, only this chord key isn't held down.
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
