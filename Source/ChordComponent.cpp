#include "ChordComponent.h"
#include <map>


static const juce::Colour colourMap [] =
{
    juce::Colour(0xffff0000),
    juce::Colour(0xff00ff00),
    juce::Colour(0xff0000ff),
    juce::Colour(0xffff00ff),
    juce::Colour(0xffff5500)
};

static const char chordChars [] =
{
    'A',
    'S',
    'D',
    'F',
    'G'
};

// Initializes the component and starts listening for key events.
ChordComponent::ChordComponent() :
    chordTracker(this)
{
    chordTracker.addListener(this);
    setWantsKeyboardFocus(true);
}


// Draws all pressed keys and the demo keyboard.
void ChordComponent::paint(juce::Graphics& g)
{
    using juce::uint8;
    using juce::Colour;
    using juce::Colours;
    using juce::Rectangle;
    // Get chord and character data:
    const Alphabet& alphabet = chordTracker.getAlphabet();
    const uint8 charCount = alphabet.getSize();
    const uint8 heldChord = chordTracker.getHeldKeys();
    const uint8 selection = chordTracker.getSelectedKey();

    // Calculate layout values:
    const int charWidth = getWidth() / (charCount + 1);
    const int rowHeight = getHeight() / 9;
    const int padding = std::max<int>(2, charWidth / 30);
    const int paddedCharWidth = charWidth - padding * 2;
    const int paddedRowHeight = rowHeight - padding * 2;
    const int line = std::max<int>(1, padding / 4);
    const float cornerSize = std::max<int>(line, paddedCharWidth / 30);;
    int xPos = getX();
    int yPos = getY();
    g.setFont(juce::Font(rowHeight * 0.9));

    // Draw chord key guides:
    for(int keyIdx = 0; keyIdx < chordTracker.getNumChordKeys(); keyIdx++)
    {
        yPos += rowHeight;
        const uint8 keyMask = 1 << keyIdx;
        g.setColour(colourMap[keyIdx]);
        Rectangle<float> keyBox(xPos + padding, yPos + padding, paddedCharWidth,
                paddedRowHeight);
        if ((keyMask & heldChord) == keyMask)
        {
            g.fillRoundedRectangle(keyBox, cornerSize);
            g.setColour(colourMap[keyIdx].contrasting());
        }
        else
        {
            g.drawRoundedRectangle(keyBox, cornerSize, line);
        }
        g.drawText(juce::String::charToString(chordChars[keyIdx]), keyBox,
                juce::Justification::centred);
    }
    
    // Draw alternate alphabet guides:
    xPos += charWidth;
    yPos += rowHeight;
    Rectangle<float> keyBounds(xPos + padding, yPos + padding, paddedCharWidth,
            paddedRowHeight);
    Rectangle<float> textBounds(keyBounds.getRight() + padding, yPos + padding, 
            (getWidth() - charWidth * 6) / 4, paddedRowHeight);
    static const std::map<juce::String, juce::String> modMap =
    {
        { "H", ": upper case" },
        { "J", ": numeric" },
        { "K", ": symbols" }
    };
    for (const auto& keyPair : modMap)
    {
        g.setColour(Colours::black);
        using juce::KeyPress;
        KeyPress modKey = KeyPress::createFromDescription(keyPair.first);
        if (modKey.isCurrentlyDown())
        {
            g.fillRoundedRectangle(keyBounds, cornerSize);
            g.setColour(Colours::white);
        }
        else
        {
            g.drawRoundedRectangle(keyBounds, cornerSize, line);
        }
        g.drawText(keyPair.first, keyBounds, juce::Justification::centred);
        g.setColour(Colours::black);
        g.drawFittedText(keyPair.second, textBounds.toNearestInt(),
                juce::Justification::left, 1);
        keyBounds.setX(textBounds.getRight() + padding);
        textBounds.setX(keyBounds.getRight() + padding);
    }
    keyBounds.setWidth(keyBounds.getWidth() * 2);
    g.drawText("<-", keyBounds, juce::Justification::centred);
    g.drawRoundedRectangle(keyBounds, cornerSize, line);
    textBounds.setLeft(keyBounds.getRight() + padding);
    g.drawFittedText(": backspace", textBounds.toNearestInt(),
            juce::Justification::left, 1);

    // Label and draw chords for each possible character:
    yPos = getY();
    for (int i = 0; i < charCount; i++)
    {
        const char indexChar = alphabet.getIndex(i);
        const uint8 indexChord = alphabet.getChord(indexChar);
        const bool selected = (indexChord == selection);
        const bool partialChord = ((indexChord | heldChord) == indexChord);
        const Rectangle<float> charBox(xPos + padding, yPos + padding,
                paddedCharWidth, paddedRowHeight);

        g.setColour(partialChord ? Colours::black : Colours::dimgrey);
        if (selected)
        {
            g.drawRoundedRectangle(charBox, cornerSize, line);
        }
        g.drawText(juce::String::charToString(indexChar), charBox,
                juce::Justification::centred);
        xPos += charWidth;
        for(int keyIdx = 0; keyIdx < chordTracker.getNumChordKeys(); keyIdx++)
        {
            const uint8 keyMask = 1 << keyIdx;
            bool isCharKey = ((keyMask & indexChord) == keyMask);
            if (partialChord)
            {
                g.setColour(colourMap[keyIdx].darker(selected ? 0.0 : 0.8));
            }
            else
            {
                g.setColour(Colours::dimgrey);
            }
            juce::Rectangle<float> keyBox = charBox.withY(yPos 
                    + (1 + keyIdx) * rowHeight);
            if (isCharKey)
            {
                g.fillRoundedRectangle(keyBox, cornerSize);
            }
            else
            {
                g.drawRoundedRectangle(keyBox, cornerSize, line);
            }
        }
    }
    g.setColour(Colours::black);
    yPos = rowHeight * 7;
    g.fillRect(0, yPos, getWidth(), 1);
    const Rectangle<int> inputBounds 
            = getLocalBounds().withY(yPos).reduced(padding, padding);
    g.drawFittedText(input, inputBounds, juce::Justification::topLeft, 5);
}


// Notifies the Listener that the set of keys held down has changed.
void ChordComponent::heldKeysChanged(const juce::uint8 heldKeys)
{
    repaint();
}

// Notifies the Listener that a character was selected.
void ChordComponent::chordEntered(const juce::uint8 selected)
{
    input += juce::String::charToString(chordTracker.getAlphabet()
            .getCharacter(selected));
    DBG("Input=" << input);
    repaint();
}

// Notifies the listener that the active alphabet has changed.
void ChordComponent::alphabetChanged(const Alphabet& alphabet)
{
    repaint();
}

// Notifies the listener that the backspace or delete key was pressed.
void ChordComponent::deleteWasPressed()
{
    input = input.substring(0, input.length() - 1);
    repaint();
}

