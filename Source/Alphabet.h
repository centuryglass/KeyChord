#pragma once

class Alphabet
{
public:
    typedef unsigned char uint8;

    struct ChordedChar
    {
        char character;
        uint8 chord;
    };

    Alphabet(const ChordedChar* chordSet, const uint8 length);

    char getIndex(const uint8 index) const;

    char getCharacter(const uint8 chord) const;

    uint8 getChord(const char character) const;

    uint8 getSize() const;

private:
    static const constexpr int maxChord = 0b00011111;
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


