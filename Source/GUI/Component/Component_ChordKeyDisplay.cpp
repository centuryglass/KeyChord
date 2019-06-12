#include "Component_ChordKeyDisplay.h"
#include "Component_ColourIds.h"
#include "Input_Key_ConfigFile.h"
#include "Input_Key_JSONKeys.h"
#include "Text_Values.h"
#include "Text_Painter.h"


// Loads and saves chord key bindings on construction.
Component::ChordKeyDisplay::ChordKeyDisplay()
{
    Input::Key::ConfigFile keyConfig;
    for (int i = 0; i < Input::Chord::numChordKeys(); i++)
    {
        const juce::Identifier& key = *Input::Key::JSONKeys::chordKeys[i];
        chordKeys.add(keyConfig.getKeyChar(key));
    }
}


// Gets the number of character columns the KeyGrid contains.
int Component::ChordKeyDisplay::getColumnCount() const
{
    return 1;
}


// Gets the number of character rows the KeyGrid contains.
int Component::ChordKeyDisplay::getRowCount() const
{
    return Input::Chord::numChordKeys();
}

// Paints the chord keys.
void Component::ChordKeyDisplay::paint(juce::Graphics& g)
{
    using namespace Text;
    // Calculate layout values:
    const int paddedRowHeight = getPaddedRowHeight();
    const int paddedCharWidth = getPaddedCharWidth();
    const Input::Chord currentChord = getHeldChord();
    const int xPadding = getXPadding();
    const int yPadding = getYPadding();
    const int rowHeight = paddedRowHeight - yPadding;
    const int charWidth = paddedCharWidth - xPadding;
    int yPos = yPadding;
    int xPos = xPadding;

    // A convenience function to more easily request character drawing 
    // operations:
    const auto drawChar = 
        [&g, &xPos, &yPos, charWidth, rowHeight, xPadding, yPadding]
        (const unsigned int toDraw)
    {
        bool wideDrawChar = Values::isWideValue(toDraw);
        Text::Painter::paintChar(g, toDraw, xPos, yPos,
                (wideDrawChar ? charWidth * 2 : charWidth), rowHeight, true);
    };
    for(int keyIdx = 0; keyIdx < chordKeys.size(); keyIdx++)
    {
        const CharValue toDraw = chordKeys[keyIdx];
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
        drawChar(toDraw);
        //Text::Painter::paintChar(g, toDraw, xPos + xPadding, yPos + yPadding,
        //        (wideDrawChar ? charWidth * 2 : charWidth), rowHeight, true);
        yPos += paddedRowHeight;
    }
}
