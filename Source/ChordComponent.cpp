#include "ChordComponent.h"
#include "CharPainter.h"
#include <map>
#include <vector>

// Characters drawn to represent the five chord buttons:
static const char chordChars [] = { 'A', 'S', 'D', 'F', 'G' };
static const constexpr int numChordKeys = 5;


//  Requests keyboard focus on construction.
ChordComponent::ChordComponent()
{
    setWantsKeyboardFocus(true);
}


// Sets the current state of the chorded keyboard, immediately redrawing the
// component if the state changes.
void ChordComponent::updateChordState(const Alphabet* activeAlphabet, 
        const juce::uint8 heldKeys, const juce::String input)
{
    bool shouldRepaint = false;
    if (activeAlphabet != currentAlphabet)
    {
        currentAlphabet = activeAlphabet;
        shouldRepaint = true;
    }
    if (heldKeys != lastHeldKeys)
    {
        lastHeldKeys = heldKeys;
        shouldRepaint = true;
    }
    if (input != bufferedInput)
    {
        bufferedInput = input;
        shouldRepaint = true;
    }

    if (shouldRepaint)
    {
        repaint();
    }
}

// Draws all chord mappings within the current alphabet, which chord keys are
// currently held down, and the buffered input string waiting to be sent to the
// target window.
void ChordComponent::paint(juce::Graphics& g)
{
    if (currentAlphabet == nullptr)
    {
        return;
    }

    using juce::uint8;
    using juce::Colour;
    using juce::Colours;
    using juce::Rectangle;

    // Save the character set size:
    const uint8 charCount = currentAlphabet->getSize();

    // Calculate layout values:
    const int paddedRowHeight = getHeight() / 8;
    const int paddedCharWidth = getWidth() / (charCount + 1);
    const int yPadding = paddedRowHeight * 0.1;
    const int xPadding = paddedCharWidth * 0.1;
    const int rowHeight = paddedRowHeight - yPadding;
    const int charWidth = paddedCharWidth - xPadding;
    int yPos = getY();
    int xPos = getX();
    g.setFont(juce::Font(rowHeight * 0.9));


    // A convenience function to more easily request charadcter drawing 
    // operations:
    const auto drawChar = 
        [&g, &xPos, &yPos, charWidth, rowHeight, xPadding, yPadding]
        (const char toDraw)
    {
        CharPainter::paintAt(g, toDraw, xPos + xPadding, yPos + yPadding,
                charWidth, rowHeight);
    };

    // Draw chord key guides:
    for(int keyIdx = 0; keyIdx < numChordKeys; keyIdx++)
    {
        yPos += paddedRowHeight;
        const uint8 keyMask = 1 << keyIdx;
        juce::Colour chordColour = findColour(chord1Selected + keyIdx, true);
        g.setColour(chordColour);
        if ((keyMask & lastHeldKeys) == keyMask)
        {
            drawChar(CharPainter::fillChar);
            g.setColour(chordColour.contrasting());
        }
        else
        {
            drawChar(CharPainter::outlineChar);
        }

        drawChar(chordChars[keyIdx]);
    }

    // Label and draw chords for each possible character:
    xPos = getX();
    for (int i = 0; i < charCount; i++)
    {
        xPos += paddedCharWidth;
        yPos = getY();
        // Current alphabet character:
        const char indexChar = currentAlphabet->getCharAtIndex(i);
        // Binary mask for the chord used to type the character:
        const uint8 characterChord = currentAlphabet->getChord(indexChar);
        // Whether this character is currently selected:
        const bool charSelected = (characterChord == lastHeldKeys);
        // Whether no chord keys are held that aren't in this character's chord:
        const bool charOpen
                = ((characterChord | lastHeldKeys) == characterChord);

        g.setColour(findColour(charOpen ? text : inactiveText, true));
        if (charSelected)
        {
            drawChar(CharPainter::outlineChar);
        }
        drawChar(indexChar);

        // Draw each chord key under the character:
        for(int keyIdx = 0; keyIdx < numChordKeys; keyIdx++)
        {
            yPos += paddedRowHeight;
            const uint8 keyMask = 1 << keyIdx;
            // Check if this chord key is currently held down:
            const bool keyIsHeld = ((keyMask & lastHeldKeys) == keyMask);
            // Check if this chord key is used for this character:
            const bool charUsesKey = ((keyMask & characterChord) == keyMask);

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
                drawChar(CharPainter::fillChar);
            }
            else
            {
                drawChar(CharPainter::outlineChar);
            }
        }
    }

    // Draw all buffered input text:
    g.setColour(findColour(text, true));
    yPos = paddedRowHeight * 6 + yPadding;
    g.fillRect(0, yPos, getWidth(), 1);
    const Rectangle<int> inputBounds 
            = getLocalBounds().withY(yPos).reduced(xPadding, yPadding);
    g.drawFittedText(bufferedInput, inputBounds, juce::Justification::topLeft,
            5);
}
