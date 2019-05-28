#include "Input_Key_Alphabet.h"


// Saves all character to chord mappings in the alphabet on construction.
Input::Key::Alphabet::Alphabet
(const ChordedChar* chordSet, const uint8 length) : size(length)
{
    for (int i = 0; i < length; i++)
    {
        const char& character = chordSet[i].character;
        const Chord& chord = chordSet[i].chord;
        charSet[i] = character;
        if (chord.isValid())
        {
            chordToChar[chord.getByteValue() - 1] = character;
        }
        if (character > 0)
        {
            charToChord[character - 1] = chord;
        }
    }
}


// Gets the character in the alphabet with a particular index value.
char Input::Key::Alphabet::getCharAtIndex(const uint8 index) const
{
    if (index >= size)
    {
        return 0;
    }
    return charSet[index];
}


// Finds the character created by a particular chord.
char Input::Key::Alphabet::getChordCharacter(const Chord chord) const
{
    if (chord.getByteValue() > maxChord)
    {
        return 0;
    }
    return chordToChar[chord.getByteValue() - 1];
}


// Finds the chord used to type a particular character.
Chord Input::Key::Alphabet::getChord(const char character) const
{
    return charToChord[character - 1];
}
    

// Gets the number of characters in the alphabet.
Input::Key::Alphabet::uint8
Input::Key::Alphabet::getSize() const
{
    return size;
}
