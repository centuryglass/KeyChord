#include "Component_ChordPreview.h"
#include "Component_Char.h"
#include "Input_Key_JSONKeys.h"
#include "Text_BinaryFont.h"
#include <map>
#include <vector>

//  Requests keyboard focus on construction.
Component::ChordPreview::ChordPreview()
{
    setWantsKeyboardFocus(true);
}


// Sets the current state of the chorded keyboard, immediately redrawing the
// component if the state changes.
void Component::ChordPreview::updateChordState(
        const Input::Key::Alphabet* activeAlphabet, 
        const Chord heldChord,
        const juce::String input)
{
    bool shouldRepaint = false;
    if (activeAlphabet != currentAlphabet)
    {
        currentAlphabet = activeAlphabet;
        shouldRepaint = true;
    }
    if (heldChord != lastHeldChord)
    {
        lastHeldChord = heldChord;
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
void Component::ChordPreview::paint(juce::Graphics& g)
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
        Char::paintAt(g, toDraw, xPos + xPadding, yPos + yPadding,
                charWidth, rowHeight);
    };

    // Draw chord key guides:
    for(int keyIdx = 0; keyIdx < Chord::numChordKeys(); keyIdx++)
    {
        yPos += paddedRowHeight;
        juce::Colour chordColour = findColour(chord1Selected + keyIdx, true);
        g.setColour(chordColour);
        if (lastHeldChord.usesChordKey(keyIdx))
        {
            drawChar(Char::fillChar);
            g.setColour(chordColour.contrasting());
        }
        else
        {
            drawChar(Char::outlineChar);
        }

        const juce::Identifier* chordKeyID 
                = Input::Key::JSONKeys::chordKeys[keyIdx];
        drawChar((char) keyConfig.getKeyChar(*chordKeyID));
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
        const Chord characterChord = currentAlphabet->getChord(indexChar);
        // Whether this character is currently selected:
        const bool charSelected = (characterChord == lastHeldChord);
        // Whether no chord keys are held that aren't in this character's chord:
        const bool charOpen = (characterChord <= lastHeldChord);

        g.setColour(findColour(charOpen ? text : inactiveText, true));
        if (charSelected)
        {
            drawChar(Char::outlineChar);
        }
        drawChar(indexChar);

        // Draw each chord key under the character:
        for(int keyIdx = 0; keyIdx < Chord::numChordKeys(); keyIdx++)
        {
            yPos += paddedRowHeight;
            // Check if this chord key is currently held down:
            const bool keyIsHeld = lastHeldChord.usesChordKey(keyIdx);
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
                drawChar(Char::fillChar);
            }
            else
            {
                drawChar(Char::outlineChar);
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
