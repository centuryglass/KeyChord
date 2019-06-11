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
    // All zero (invalid)
    0b00000,
};
// Extracts an ordered character set from juce::var configuration data, saves
// the set, and assigns chord mappings.
Text::CharSet::Cache::Cache(const juce::var setData)
{
    using juce::var;
    using juce::Identifier;
    using juce::Array;
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
    static const juce::Identifier priorityKey("chord priority");

    struct PrioritizedCharPair
    {
        CharPair charPair;
        int priority = std::numeric_limits<int>::min();
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
        const std::function<unsigned int(var&, const Identifier&)> getVarChar = 
        [](var& varCharPair, const Identifier& key)
        {
            var varChar = varCharPair[key];
            unsigned int charIndex = 0;
            if (varChar.isInt())
            {
                int index = varChar.operator int();
                charIndex = index;
            }
            else
            {
                charIndex = Values::getCharValue(
                        varChar.operator juce::String());
            }
            return charIndex;
        };
        var mainCharVar = charVar[charKey];
        if (mainCharVar.isInt())
        {
            newPair.charPair.charValue = mainCharVar.operator int();
        }
        else
        {
            newPair.charPair.charValue = Values::getCharValue(charVar[charKey]);
        }
        if (newPair.charPair.charValue == 0)
        {
            DBG(dbgPrefix << __func__ << ": Warning: character value \""
                    << charVar.toString() 
                    << "\" has an invalid primary character.");
            continue;
        }

        if (charVar.hasProperty(shiftCharKey))
        {
            newPair.charPair.shiftedValue 
                = Values::getCharValue(charVar[shiftCharKey]);
        }
        // Reuse the main character value if there's no shifted value provided:
        if (newPair.charPair.shiftedValue == 0)
        {
            newPair.charPair.shiftedValue = newPair.charPair.charValue;
        }

        // Read the chord priority, or use the default minimum if none is
        // provided:
        if (charVar.hasProperty(priorityKey))
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
        chordOrderedSet.add(newPair);
        charSet.add(newPair.charPair);
    }

    // Assign chords by priority, keeping keys with shared priority values in
    // order:
    chordOrderedSet.sort(priorityComparator, true);
    for (int i = 0; i < chordOrderedSet.size(); i++)
    {
        Chord nextChord(chordConvenienceOrder[i]);
        const CharPair& nextPair = chordOrderedSet.getReference(i).charPair;
        chordMap[(unsigned int) nextPair.charValue] = nextChord;
        chordMap[(unsigned int) nextPair.shiftedValue] = nextChord;
        charPairMap[nextChord] = nextPair;
    }
}


 // Creates a character set cache for the modifier keys.
Text::CharSet::Cache Text::CharSet::Cache::getModCharset()
{
    Cache modCache;
    juce::Array<unsigned int> modKeys =
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
        Chord modChord(chordConvenienceOrder[i]);
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
unsigned int Text::CharSet::Cache::getCharAtIndex
(const unsigned int index, const bool shifted) const
{
    if (index > charSet.size())
    {
        return 0;
    }
    return shifted ? charSet[index].shiftedValue : charSet[index].charValue;
}


// Finds the character created by a particular chord.
unsigned int Text::CharSet::Cache::getChordCharacter
(const Chord chord, const bool shifted) const
{
    try
    {
        return shifted ? charPairMap.at(chord).shiftedValue :
            charPairMap.at(chord).charValue;
    }
    catch (const std::out_of_range& e)
    {
        return 0;
    }
}


// Finds the chord used to type a particular character.
Chord Text::CharSet::Cache::getCharacterChord
(const unsigned int character) const
{
    try
    {
        return chordMap.at(character);
    }
    catch (const std::out_of_range& e)
    {
        DBG(dbgPrefix << __func__ << ": Couldn't find chord for char value 0x"
                << juce::String::toHexString((unsigned int) character)
                << " (" << juce::String((unsigned int) character) << ")");
        return Chord();
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
