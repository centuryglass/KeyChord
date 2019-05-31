#pragma once
/**
 * @file  Text_CharSet_Cache.h
 *
 * @brief  Represents a character set where each character has a corresponding
 *         chord value.
 */

#include "Chord.h"
#include <map>

namespace Text { namespace CharSet { class Cache; } }

class Text::CharSet::Cache
{
public:
    typedef unsigned char uint8;
    /**
     * @brief  Extracts an ordered character set from juce::var configuration
     *         data, saves the set, and assigns chord mappings.
     *
     * @param setData   A character set data array, read from the character
     *                  set configuration file.
     */
    Cache(const juce::var setData);

    /**
     * @brief  Creates an empty cache object.
     */
    Cache() { }

    /**
     * @brief  Gets the character in the alphabet with a particular index value.
     *
     * @param index    The index of the requested character.
     *
     * @param shifted  Whether to get the shifted version of the character.
     *
     * @return         The character value at that index, or 0 if the index was
     *                 invalid.
     */
    unsigned int getCharAtIndex(const unsigned int index,
            const bool shifted) const;

    /**
     * @brief  Finds the character created by a particular chord.
     *
     * @param chord    A chord object representing a particular set of chord
     *                 keys that are held down.
     *
     * @param shifted  Whether to get the shifted version of the character.
     *
     * @return         The character that chord should create, or 0 if the
     *                 alphabet does not use that chord.
     */
    unsigned int getChordCharacter
    (const Chord chord, const bool shifted) const;

    /**
     * @brief  Finds the chord used to type a particular character.
     *
     * @param character  A character within the alphabet.
     *
     * @return           The chord bitmask used to create that character, or 0
     *                   if that character is not in the alphabet.
     */
    Chord getCharacterChord(const unsigned int character) const;

    /**
     * @brief  Gets the number of characters in the alphabet.
     *
     * @return  The total number of character<->chord mappings stored.
     */
    int getSize() const;

private:
    // Stores a character with its shifted value:
    struct CharPair
    {
        unsigned int charValue = 0;
        unsigned int shiftedValue = 0;
    };

    // All characters in the Alphabet, in order:
    juce::Array<CharPair> charSet;
    // Map of each individual character to its chord value:
    std::map<unsigned int, Chord> chordMap;
    // Map of each chord to its character set:
    std::map<Chord, CharPair> charPairMap;
};
