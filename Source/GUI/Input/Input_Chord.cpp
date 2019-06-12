#include "Input_Chord.h"

// Sets the number of input keys used.  If you change this value, you'll also
// need to update chord key bindings, extend/reduce the sizes of character sets,
// and probably fix a lot of other less obvious problems. You definitely should
// not set this value to be larger than eight or less than one.
static const constexpr int inputKeyCount = 5;


// Creates a chord from its raw bitmap value.
Input::Chord::Chord(const uint8 bitMap) : bitMap(bitMap)
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
bool Input::Chord::isValid() const
{
    return bitMap != 0;
}


// Creates a string representation of this chord for debugging.
juce::String Input::Chord::toString() const
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
bool Input::Chord::usesChordKey(const uint8 keyIndex) const
{
    const uint8 keyBit = 1 << keyIndex;
    return (keyBit & bitMap) == keyBit;
}


// Checks if this chord only holds down keys that are used in typing another
// chord.
bool Input::Chord::isSubchordOf(const Chord& possibleSuperChord) const
{
    return isValid() && (bitMap != possibleSuperChord.bitMap)
            && ((bitMap & possibleSuperChord.bitMap) == bitMap);
}


// Gets the number of chord input keys are used.
int Input::Chord::numChordKeys()
{
    return inputKeyCount;
}


// Chord that this Chord would become if a new key is held down.
Input::Chord Input::Chord::withKeyHeld(const uint8 keyIndex) const
{
    if (keyIndex >= inputKeyCount)
    {
        return Chord();
    }
    return Chord(bitMap | (1 << keyIndex));
}


// Gets the Chord that this Chord would become if a chord key is released.
Input::Chord Input::Chord::withKeyReleased(const uint8 keyIndex) const
{
    if (keyIndex >= inputKeyCount)
    {
        return Chord();
    }
    return Chord(bitMap & ~(1 << keyIndex));
}


// Gets the bitmap value used to create this chord.
Input::Chord::uint8 Input::Chord::getByteValue() const
{
    return bitMap;
}


// Checks if two chords are equal.
bool Input::Chord::operator==(const Chord& rhs) const
{
    return bitMap == rhs.bitMap;
}


// Checks if two chords are not equal.
bool Input::Chord::operator!=(const Chord& rhs) const
{
    return bitMap != rhs.bitMap;
}


// Checks if this Chord comes after another chord.
bool Input::Chord::operator<(const Chord& rhs) const
{
    return bitMap < rhs.bitMap;
}


// Checks if this Chord comes before another chord
bool Input::Chord::operator>(const Chord& rhs) const
{
    return bitMap > rhs.bitMap;
}
