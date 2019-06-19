#include "Text_CharSet_Cache.h"
#include "Text_Values.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Text::CharSet::Cache::";
#endif

// Maximum number of valid chords:
static const constexpr int numChords = 31;

// Chords arranged in order by how easy they are to type. Chords with higher
// priority levels are assigned easier chords.
static const juce::uint8 chordConvenienceOrder [] =
{
    // Single key:
    0b00001,
    0b00010,
    0b00100,
    0b01000,
    0b10000,
    // Adjacent pairs:
    0b00011,
    0b00110,
    0b01100,
    0b11000,
    // Gapped pairs:
    0b00101,
    0b01010,
    0b01001,
    0b10100,
    0b10010,
    0b10001,
    // Adjacent triples:
    0b00111,
    0b01110,
    0b11100,
    // Gapped triples:
    0b01011,
    0b01101,
    0b10110,
    0b10011,
    0b11001,
    0b11010,
    0b10101,
    // Quads:
    0b01111,
    0b10111,
    0b11011,
    0b11101,
    0b11110,
    // All Keys:
    0b11111,
    // All zero(invalid)
    0b00000,
};


// Extracts an ordered character set from juce::var configuration data, saves
// the set, and assigns chord mappings.
Text::CharSet::Cache::Cache(const juce::var setData)
{
    using juce::var;
    using juce::String;
    using juce::Identifier;
    using juce::Array;
    using Text::CharValue;
    if (! setData.isArray())
    {
        DBG(dbgPrefix << __func__ << ": Error, character set value not array");
        DBG(dbgPrefix << __func__ << ": Invalid set data: "
                << setData.toString());
        return;
    }
    // character object keys:
    static const Identifier charKey("character");
    static const juce::Identifier shiftCharKey("shifted");
    static const juce::Identifier chordKey("chord");
    static const juce::Identifier priorityKey("chord priority");

    struct PrioritizedCharPair
    {
        CharPair charPair;
        int priority = 0;
    };

    class
    {
    public:
        int compareElements
        (PrioritizedCharPair first, PrioritizedCharPair second)
        {
            return second.priority - first.priority;
        }
    } priorityComparator;

    // Read in each value from the character set array:
    Array<PrioritizedCharPair> chordOrderedSet;
    const Array<var>& setArray = *setData.getArray();
    for (const var& charVar : setArray)
    {
        // Don't allow the character set to get larger than the number of valid
        // chords:
        if (chordOrderedSet.size() == numChords)
        {
            DBG(dbgPrefix << __func__ << ": Warning:  character set has "
                    << setArray.size() << " characters, but there are only "
                    << numChords << " different possible chords.");
            break;
        }
        // Ignore array values that aren't objects:
        if (! charVar.isObject())
        {
            DBG(dbgPrefix << __func__ << ": Warning: non object \""
                    << charVar.toString() << "\" found in character set.");
            continue;
        }
        // Ignore array values without a valid primary character:
        if (! charVar.hasProperty(charKey))
        {
            DBG(dbgPrefix << __func__ << ": Warning: character value \""
                    << charVar.toString() << "\" has no primary character.");
            continue;
        }
        PrioritizedCharPair newPair;

        // Load a character as either a string or a character code:
        const std::function<CharValue(const var&, const Identifier&)> getVarChar
                = [](const var& varCharPair, const Identifier& key)
        {
            CharValue charValue = 0;
            if (! varCharPair.hasProperty(key))
            {
                return charValue;
            }
            var varChar = varCharPair[key];
            if (varChar.isInt())
            {
                // Convert integers to hex strings so they can't be
                // misinterpreted as nonstandard character values:
                int value = varChar.operator int();
                charValue = Values::getCharValue(String("0x")
                        + String::toHexString(value));
            }
            else
            {
                charValue = Values::getCharValue(
                        varChar.operator String());
            }
            return charValue;
        };
        newPair.charPair.charValue = getVarChar(charVar, charKey);
        if (newPair.charPair.charValue == 0)
        {
            DBG(dbgPrefix << __func__ << ": Warning: character value \""
                    << charVar.toString()
                    << "\" has an invalid primary character.");
            continue;
        }

        newPair.charPair.shiftedValue = getVarChar(charVar, shiftCharKey);
        // Reuse the main character value if there's no shifted value provided:
        if (newPair.charPair.shiftedValue == 0)
        {
            newPair.charPair.shiftedValue = newPair.charPair.charValue;
        }

        // Check for an explicit chord assignment:
        if (charVar.hasProperty(chordKey))
        {
            juce::String chordString = charVar[chordKey].toString();
            if (chordString.length() == Input::Chord::numChordKeys()
                    && chordString.containsChar('#')
                    && chordString.containsOnly("#_"))
            {
                juce::uint8 chordByte = 0;
                for (int i = 0; i < chordString.length(); i++)
                {
                    if (chordString[i] == '#')
                    {
                        chordByte |= (1 << i);
                    }
                }
                jassert(chordByte > 0);
                Input::Chord explicitChord(chordByte);
                if (assignChord(newPair.charPair, explicitChord))
                {
                    newPair.priority = -1;
                    DBG(dbgPrefix << __func__
                            << ": Explicitly assigned chord \"" << chordString
                            << "\" to character value \""
                            << charVar.toString());
                }
                else
                {
                    DBG(dbgPrefix << __func__ << ": Couldn't assign chord \""
                            << chordString << "\" to character value \""
                            << charVar.toString()
                            << "\", chord priority will be used instead.");
                }
            }
            else
            {
                DBG(dbgPrefix << __func__ << ": Warning: character value \""
                        << charVar.toString()
                        << "\" uses invalid chord string \""
                        << chordString << "\"");
            }
        }
        // If no chord is assigned, read the chord priority, or use the default
        // minimum if none is provided.
        if (newPair.priority >= 0 && charVar.hasProperty(priorityKey))
        {
            var priority = charVar[priorityKey];
            if (priority.isInt())
            {
                newPair.priority = (int) priority;
            }
            else
            {
                DBG(dbgPrefix << __func__ << ": Warning: character value \""
                        << charVar.toString()
                        << "\" has a non-integer priority value.");
            }
        }
        // Check if the wide character count needs updating:
        if (Values::isWideValue(newPair.charPair.charValue)
                || Values::isWideValue(newPair.charPair.shiftedValue))
        {
            wideDrawCharacters++;
        }
        if (newPair.priority >= 0)
        {
            chordOrderedSet.add(newPair);
        }
        charSet.add(newPair.charPair);
    }

    // Assign chords by priority, keeping keys with shared priority values in
    // order:
    int chordIndex = 0;
    chordOrderedSet.sort(priorityComparator, true);
    for (int i = 0; i < chordOrderedSet.size(); i++)
    {
        Input::Chord nextChord(chordConvenienceOrder[chordIndex]);
        // Skip chords that were explicitly assigned:
        while (charPairMap.count(nextChord) > 0)
        {
            chordIndex++;
            nextChord = Input::Chord(chordConvenienceOrder[chordIndex]);
        }
        jassert(chordIndex < numChords);

        const CharPair& nextPair = chordOrderedSet.getReference(i).charPair;
        assignChord(nextPair, nextChord);
        chordIndex++;
    }
}


// Creates a character set cache for the modifier keys.
Text::CharSet::Cache Text::CharSet::Cache::getModCharset()
{
    Cache modCache;
    juce::Array<Text::CharValue> modKeys =
    {
        Values::shift,
        Values::ctrl,
        Values::alt,
        Values::super
    };
    for (int i = 0; i < modKeys.size(); i++)
    {
        CharPair modPair = { modKeys[i], modKeys[i] };
        modCache.charSet.add(modPair);
        Input::Chord modChord(chordConvenienceOrder[i]);
        modCache.chordMap[modKeys[i]] = modChord;
        modCache.charPairMap[modChord] = modPair;
        if (Values::isWideValue(modKeys[i]))
        {
            modCache.wideDrawCharacters++;
        }
    }
    return modCache;
}


// Gets the character in the alphabet with a particular index value.
Text::CharValue Text::CharSet::Cache::getCharAtIndex
(const unsigned int index, const bool shifted) const
{
    if (index > charSet.size())
    {
        return 0;
    }
    return shifted ? charSet[index].shiftedValue : charSet[index].charValue;
}


// Finds the character created by a particular chord.
Text::CharValue Text::CharSet::Cache::getChordCharacter
(const Input::Chord chord, const bool shifted) const
{
    try
    {
        return shifted ? charPairMap.at(chord).shiftedValue :
            charPairMap.at(chord).charValue;
    }
    catch(const std::out_of_range& e)
    {
        return 0;
    }
}


// Finds the chord used to type a particular character.
Input::Chord Text::CharSet::Cache::getCharacterChord
(const Text::CharValue character) const
{
    try
    {
        return chordMap.at(character);
    }
    catch(const std::out_of_range& e)
    {
        DBG(dbgPrefix << __func__ << ": Couldn't find chord for char value 0x"
                << juce::String::toHexString((Text::CharValue) character)
                << " (" << juce::String((Text::CharValue) character) << ")");
        return Input::Chord();
    }
}


// Gets the number of characters in the alphabet.
int Text::CharSet::Cache::getSize() const
{
    return charSet.size();
}


// Gets how many characters in this set need twice as much width to draw.
int Text::CharSet::Cache::wideDrawCharacterCount() const
{
    return wideDrawCharacters;
}


// Attempts to assign a character value to a specific Chord.
bool Text::CharSet::Cache::assignChord
(const CharPair& character, const Input::Chord& chord)
{
    if (! chord.isValid() || character.charValue == 0
            || charPairMap.count(chord) > 0)
    {
        return false;
    }
    chordMap[character.charValue] = chord;
    chordMap[character.shiftedValue] = chord;
    charPairMap[chord] = character;
    return true;
}
