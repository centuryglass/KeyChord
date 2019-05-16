#include "Alphabet.h"

Alphabet::Alphabet(const ChordedChar* chordSet, const uint8 length) : 
    size(length)
{
    for (int i = 0; i < length; i++)
    {
        const char& character = chordSet[i].character;
        const uint8& chord = chordSet[i].chord;
        charSet[i] = character;
        if (chord > 0)
        {
            chordToChar[chord - 1] = character;
        }
        if (character > 0)
        {
            charToChord[character - 1] = chord;
        }
    }
}

char Alphabet::getIndex(const uint8 index) const
{
    if (index >= size)
    {
        return 0;
    }
    return charSet[index];
}

char Alphabet::getCharacter(const uint8 chord) const
{
    if (chord > maxChord)
    {
        return 0;
    }
    return chordToChar[chord - 1];
}

Alphabet::uint8 Alphabet::getChord(const char character) const
{
    return charToChord[character - 1];
}
    
Alphabet::uint8 Alphabet::getSize() const
{
    return size;
}
