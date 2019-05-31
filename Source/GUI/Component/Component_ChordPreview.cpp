#include "Component_ChordPreview.h"
#include "Input_Key_JSONKeys.h"
#include "Text_BinaryFont.h"
#include "Text_Painter.h"
#include "Text_CharSet_Values.h"
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
        const Text::CharSet::Cache* activeSet, 
        const Chord heldChord,
        const juce::Array<unsigned int> input)
{
    bool shouldRepaint = false;
    if (activeSet != charSet)
    {
        charSet = activeSet;
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
    if (charSet == nullptr)
    {
        return;
    }

    using juce::uint8;
    using juce::Colour;
    using juce::Colours;
    using juce::Rectangle;
    using namespace Text::CharSet;

    // Save the character set size:
    const uint8 charCount = charSet->getSize() 
            + charSet->wideDrawCharacterCount();

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
        (const unsigned int toDraw)
    {
        bool wideDrawChar = Values::isWideValue(toDraw);
        Text::Painter::paintChar(g, toDraw, xPos + xPadding, yPos + yPadding,
                (wideDrawChar ? charWidth * 2 : charWidth), rowHeight);
    };

    // Draw chord key guides:
    for(int keyIdx = 0; keyIdx < Chord::numChordKeys(); keyIdx++)
    {
        yPos += paddedRowHeight;
        juce::Colour chordColour = findColour(chord1Selected + keyIdx, true);
        g.setColour(chordColour);
        if (lastHeldChord.usesChordKey(keyIdx))
        {
            drawChar(Values::fill);
            g.setColour(chordColour.contrasting());
        }
        else
        {
            drawChar(Values::outline);
        }

        const juce::Identifier* chordKeyID 
                = Input::Key::JSONKeys::chordKeys[keyIdx];
        drawChar((char) keyConfig.getKeyChar(*chordKeyID));
    }

    // Label and draw chords for each possible character:
    xPos = getX();
    for (int i = 0; i < charSet->getSize(); i++)
    {
        xPos += paddedCharWidth;
        yPos = getY();
        // Current character set index:
        const unsigned int charIndex = charSet->getCharAtIndex(i, false);
        // Whether the character needs double the normal width:
        const bool wideDrawChar = Text::CharSet::Values::isWideValue(charIndex);
        // Binary mask for the chord used to type the character:
        const Chord characterChord = charSet->getCharacterChord(charIndex);
        // Whether this character is currently selected:
        const bool charSelected = (characterChord == lastHeldChord);
        // Whether no chord keys are held that aren't in this character's chord:
        const bool charOpen = charSelected || lastHeldChord.isSubchordOf(
                characterChord);

        g.setColour(findColour(charOpen ? text : inactiveText, true));
        if (charSelected)
        {
            drawChar(wideDrawChar ? Values::wideOutline : Values::outline);
        }
        drawChar(charIndex);

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
                drawChar(wideDrawChar ? Values::wideFill : Values::fill);
            }
            else
            {
                drawChar(wideDrawChar ? Values::shift : Values::outline);
            }
        }
        if (wideDrawChar)
        {
            xPos += paddedCharWidth;
        }
    }

    // Draw all buffered input text:
    g.setColour(findColour(text, true));
    yPos = paddedRowHeight * 6 + yPadding;
    g.fillRect(0, yPos, getWidth(), 1);
    const Rectangle<int> inputBounds 
            = getLocalBounds().withTop(yPos).reduced(xPadding, yPadding);
    g.setColour(findColour(text));
    Text::Painter::paintString(g, bufferedInput, inputBounds.getX(),
            inputBounds.getY(), inputBounds.getWidth(), inputBounds.getHeight(),
            rowHeight, 2);
}
