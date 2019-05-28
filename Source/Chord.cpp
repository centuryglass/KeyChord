#include "Chord.h"

// Sets the number of input keys used. Don't mess with this value lightly, and
// definitely don't set it to be larger than eight or less than one.
static const constexpr int inputKeyCount = 5;


// Creates a chord from its raw bitmap value.
Chord::Chord(const uint8 bitMap) : bitMap(bitMap)
{
    static uint8 maxChord = 0;
    if (maxChord == 0)
    {
        for (int i = 0; i < inputKeyCount; i++)
        {
            maxChord |= (1 << i);
        }
    }
    if (bitMap > maxChord)
    {
        this->bitMap = 0;
    }
}


// Checks if this object represents a valid input chord.
bool Chord::isValid() const
{
    return bitMap != 0;
}


// Creates a string representation of this chord for debugging.
juce::String Chord::toString() const
{
    using juce::String;
    String chordString;
    for (int i = 0; i < inputKeyCount; i++)
    {
        chordString += (usesChordKey(i) ? "#" : "_");
    }
    return chordString;
}


// Checks if a particular chord key index is held down when typing this chord.
bool Chord::usesChordKey(const uint8 keyIndex) const
{
    const uint8 keyBit = 1 << keyIndex;
    return (keyBit & bitMap) == keyBit;
}


// Checks if this chord only holds down keys that are used in typing another
// chord.
bool Chord::isSubchordOf(const Chord& possibleSuperChord) const
{
    return isValid() && (bitMap != possibleSuperChord.bitMap)
            && ((bitMap & possibleSuperChord.bitMap) == bitMap);
}


// Gets the number of chord input keys are used.
int Chord::numChordKeys()
{
    return inputKeyCount;
}


// Chord that this Chord would become if a new key is held down.
Chord Chord::withKeyHeld(const uint8 keyIndex) const
{
    if (keyIndex >= inputKeyCount)
    {
        return Chord();
    }
    return Chord(bitMap | (1 << keyIndex));
}


// Gets the Chord that this Chord would become if a chord key is released.
Chord Chord::withKeyReleased(const uint8 keyIndex) const
{
    if (keyIndex >= inputKeyCount)
    {
        return Chord();
    }
    return Chord(bitMap & ~(1 << keyIndex));
}


// Gets the bitmap value used to create this chord.
Chord::uint8 Chord::getByteValue() const
{
    return bitMap;
}


// Checks if two chords are equal.
bool Chord::operator==(const Chord& rhs) const
{
    return bitMap == rhs.bitMap;
}


// Checks if two chords are not equal.
bool Chord::operator!=(const Chord& rhs) const
{
    return bitMap != rhs.bitMap;
}


// Checks if this Chord is a subchord of another Chord.
bool Chord::operator<(const Chord& rhs) const
{
    return bitMap != rhs.bitMap && (bitMap & rhs.bitMap) == rhs.bitMap;
}


// Checks if this Chord is a superchord of another Chord.
bool Chord::operator>(const Chord& rhs) const
{
    return bitMap != rhs.bitMap && (bitMap & rhs.bitMap) == bitMap;
}


// Checks if this Chord equals or is a subchord of another Chord.
bool Chord::operator<=(const Chord& rhs) const
{
    return (bitMap & rhs.bitMap) == rhs.bitMap;
}


// Checks if this Chord equals or is a superchord of another Chord.
bool Chord::operator>=(const Chord& rhs) const
{
    return (bitMap & rhs.bitMap) == bitMap;
}
