#include "Component_ChordKeyDisplay.h"
#include "Input_Key_ConfigFile.h"
#include "Input_Key_JSONKeys.h"
#include "Text_CharSet_Values.h"
#include "Text_Painter.h"

Component::ChordKeyDisplay::ChordKeyDisplay()
{
    Input::Key::ConfigFile keyConfig;
    for (int i = 0; i < Chord::numChordKeys(); i++)
    {
        const juce::Identifier& key = *Input::Key::JSONKeys::chordKeys[i];
        chordKeys.add(keyConfig.getKeyChar(key));
    }
}


// Updates the saved chord value.
void Component::ChordKeyDisplay::updateChord(const Chord newChord)
{
    currentChord = newChord;
    repaint();
}


// Paints the chord keys.
void Component::ChordKeyDisplay::paint(juce::Graphics& g)
{
    using namespace Text::CharSet;
    // Calculate layout values:
    const int paddedRowHeight = getHeight() / Chord::numChordKeys();
    const int paddedCharWidth = getWidth();
    const int yPadding = paddedRowHeight * 0.1;
    const int xPadding = paddedCharWidth * 0.1;
    const int rowHeight = paddedRowHeight - yPadding;
    const int charWidth = paddedCharWidth - xPadding;
    int yPos = 0;
    int xPos = 0;

    // A convenience function to more easily request character drawing 
    // operations:
    const auto drawChar = 
        [&g, &xPos, &yPos, charWidth, rowHeight, xPadding, yPadding]
        (const unsigned int toDraw)
    {
        bool wideDrawChar = Values::isWideValue(toDraw);
        Text::Painter::paintChar(g, toDraw, xPos + xPadding, yPos + yPadding,
                (wideDrawChar ? charWidth * 2 : charWidth), rowHeight);
    };
    for(int keyIdx = 0; keyIdx < chordKeys.size(); keyIdx++)
    {
        const unsigned int toDraw = chordKeys[keyIdx];
        bool wideDrawChar = Values::isWideValue(toDraw);
        juce::Colour chordColour = findColour(chord1Selected + keyIdx, true);
        g.setColour(chordColour);
        if (currentChord.usesChordKey(keyIdx))
        {
            drawChar(Values::fill);
            g.setColour(chordColour.contrasting());
        }
        else
        {
            drawChar(Values::outline);
        }
        Text::Painter::paintChar(g, toDraw, xPos + xPadding, yPos + yPadding,
                (wideDrawChar ? charWidth * 2 : charWidth), rowHeight);
        yPos += paddedRowHeight;
    }
}
