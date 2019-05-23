#pragma once
/**
 * @file  Alphabet.h
 *
 * @brief  Represents a character set where each character has a corresponding
 *         chord value.
 */

class Alphabet
{
public:
    typedef unsigned char uint8;

    /**
     * @brief  Stores a character along with the chord used to create it.
     */
    struct ChordedChar
    {
        // A typeable character.
        char character;
        // A bitmask representing the set of chord keys used to type the
        // character.
        uint8 chord;
    };


    /**
     * @brief  Saves all character to chord mappings in the alphabet on
     *         construction.
     *
     * @param chordSet  An array of character to chord mappings.
     *
     * @param length    The number of characters in the alphabet.
     */
    Alphabet(const ChordedChar* chordSet, const uint8 length);

    /**
     * @brief  Gets the character in the alphabet with a particular index value.
     *
     * @param index  The index of the requested character.
     *
     * @return       The character at that index, or 0 if the index was invalid.
     */
    char getCharAtIndex(const uint8 index) const;

    /**
     * @brief  Finds the character created by a particular chord.
     *
     * @param chord  A bitmask storing which keys are held down to create the
     *               chord.
     *
     * @return       The character that chord should create, or 0 if the
     *               alphabet does not use that chord.
     */
    char getCharacter(const uint8 chord) const;

    /**
     * @brief  Finds the chord used to type a particular character.
     *
     * @param character  A character within the alphabet.
     *
     * @return           The chord bitmask used to create that character, or 0
     *                   if that character is not in the alphabet.
     */
    uint8 getChord(const char character) const;

    /**
     * @brief  Gets the number of characters in the alphabet.
     *
     * @return  The total number of character<->chord mappings stored.
     */
    uint8 getSize() const;

private:
    // Largest value a chord may have:
    static const constexpr int maxChord = 0b00011111;
    // Largest size a character in the alphabet may have: 
    static const constexpr int maxAscii = 0b11111111;

    // Number of characters in the alphabet:
    uint8 size = 0;
    // All characters in the Alphabet, in order:
    char charSet [maxChord] = { 0 };
    // chordToChar[chord - 1] = character
    char chordToChar [maxChord] = { 0 };
    // charToChord[character - 1] = chord
    uint8 charToChord[maxAscii] = { 0 };
};


