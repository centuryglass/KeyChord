#include "Component_HelpScreen.h"
#include "Component_ChordPreview.h"
#include "Component_ColourIds.h"
#include "Input_Key_JSONKeys.h"
#include "Text_Values.h"
#include "Text_Painter.h"

// Localized text class key:
static const juce::Identifier localeKey("Component_HelpScreen");
// Localized text value keys:
static const juce::Identifier helpTitle("helpTitle");
static const juce::Identifier chordKeys("chordKeys");

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Component::HelpScreen::";
#endif

/**
 * @brief  Converts a JUCE String object into an array of character values.
 *
 * @param inputStr  The string to convert.
 *
 * @return          Character values that Text::Painter can use to draw the
 *                  string.
 */
static Text::CharString toCharString(const juce::String inputStr)
{
    Text::CharString charString;
    for (int i = 0; i < inputStr.length(); i++)
    {
        const juce::String juceString(inputStr.substring(i, i + 1));
        const Text::CharValue charValue
                = Text::Values::getCharValue(juceString);
        if (charValue != 0)
        {
            charString.add(charValue);
        }
    }
    return charString;
}


/**
 * @brief  Loads help text on construction.
 */
Component::HelpScreen::HelpScreen() : Locale::TextUser(localeKey)
{
    using juce::Array;
    using Text::CharString;
    using Text::CharLineArray;
    namespace CharValues = Text::Values;
    namespace InputKeys = Input::Key::JSONKeys;

    // Load all chord key info together:
    const CharString chordDivider = toCharString(", ");
    for (const juce::Identifier* chordID : InputKeys::chordKeys)
    {
        if (chordNames.isEmpty())
        {
            chordNames.addArray(toCharString(" ("));
        }
        chordChars.add(keyConfig.getKeyChar(*chordID));

        const CharString nameValue
                = toCharString(keyConfig.getKeyName(*chordID));
        if (chordNames.size() > 2 && ! nameValue.isEmpty())
        {
            chordNames.addArray(chordDivider);
        }
        chordNames.addArray(nameValue);
    }
    if (! chordNames.isEmpty())
    {
        chordNames.addArray(toCharString("): "));
    }
    chordDescription = toCharString(localeText(chordKeys));

    // Load all bound remaining keys as new lines:
    int indexOffset = InputKeys::chordKeys.size();
    for (int i = 0; i < InputKeys::allKeys.size() - indexOffset; i++)
    {
        const juce::Identifier& keyId
                = *InputKeys::allKeys.getReference(i + indexOffset);
        // skip unbound keys:
        if (! keyConfig.getBoundKey(keyId).isValid())
        {
            i--;
            indexOffset++;
            continue;
        }
        symbolChars.add(keyConfig.getKeyChar(keyId));

        CharString name = toCharString(juce::String(" (")
                + keyConfig.getKeyName(keyId));
        if (name.size() == 2 || (name.size() ==  3
                    && name.getLast() == symbolChars[i]))
        {
            name.clear();
        }
        else
        {
            name.addArray(toCharString(")"));
        }
        keyNames.add(name);

        CharString description = toCharString(": ");
        const CharString actionDescription
                = toCharString(keyConfig.getActionDescription(keyId));
        description.addArray(actionDescription);
        descriptions.add(description);
    }
}


// Prints all help text.
void Component::HelpScreen::paint(juce::Graphics& g)
{
    using juce::Array;
    namespace TextValues = Text::Values;

    const int xMargin = getWidth() / 30;
    const int yMargin = getHeight() / 30;
    const int rowHeight = (getHeight() - yMargin) / (symbolChars.size() + 2);
    const int charPadding = rowHeight / 10;
    const int drawnCharSize = rowHeight - charPadding;

    const int xStart = xMargin / 2;
    const int yStart = yMargin / 2;

    int xPos = xStart;
    int yPos = yStart;

    // Draw a key, returning the drawn key width.
    const auto drawKey = [&g, &xPos, &yPos, &drawnCharSize, &charPadding]
            (const Text::CharValue key)
    {
        const bool isWideKey = TextValues::isWideValue(key);
        const int width = (isWideKey ? drawnCharSize * 2 : drawnCharSize);
        Text::Painter::paintChar(g,
                (isWideKey ? TextValues::wideOutline : TextValues::outline),
                xPos, yPos, width, drawnCharSize);
        Text::Painter::paintChar(g, key, xPos, yPos, width, drawnCharSize);
        return width;
    };

    // Draw a string, returning the x-coordinate of the string's end.
    const auto drawString = [&g, &xPos, &yPos, &rowHeight, &drawnCharSize, this]
            (const Text::CharString charString)
    {
        return Text::Painter::paintString(g, charString, xPos, yPos,
                getWidth() - xPos, rowHeight, rowHeight);
    };

    // Draw title:
    g.setColour(findColour(text));
    Text::CharString title = toCharString(localeText(helpTitle));
    drawString(title);
    xPos = xStart;
    yPos += rowHeight;

    // Draw chord key guide:
    for (int i = 0; i < Input::Chord::numChordKeys(); i++)
    {
        g.setColour(findColour((int) chord1Active + i));
        Text::CharValue chordKey = chordChars[i];
        xPos += drawKey(chordKey) + charPadding;
    }
    g.setColour(findColour(text));
    xPos = drawString(chordNames);
    xPos = drawString(chordDescription);

    // Draw all other action keys:
    xPos = xStart;
    yPos = yStart + rowHeight * 2;
    int maxXPos = xStart;
    for (const Text::CharValue& keyChar : symbolChars)
    {
        maxXPos = std::max(maxXPos, xPos + drawKey(keyChar) + charPadding);
        yPos += rowHeight;
    }

    // Draw action key names:
    xPos = maxXPos;
    yPos = yStart + rowHeight * 2;
    for (const Text::CharString& keyName : keyNames)
    {
        maxXPos = std::max(maxXPos, drawString(keyName) + charPadding);
        yPos += rowHeight;
    }

    // Draw descriptions:
    xPos = maxXPos;
    yPos = yStart + rowHeight * 2;
    for (const Text::CharString& description : descriptions)
    {
        drawString(description);
        yPos += rowHeight;
    }
}
