#include "Alphabet.h"
#include "AlphabetFactory.h"
#include <vector>
#include <algorithm>

// Chords arranged in order by how easy they are to type. Use this to pick
// reasonable chord maps.
static const Alphabet::uint8 chordConvenienceOrder [] =
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
    // Gapped pairs:
    0b00101,
    0b01010,
    0b01001,
    // Adjacent triples:
    0b00111,
    0b01110,
    // Gapped triples:
    0b01011,
    0b01101,
    // Quads:
    0b01111,
    // All non-singles that use key 5:
    0b11000,
    0b10100,
    0b10010,
    0b10001,
    0b11100,
    0b10110,
    0b10011,
    0b10111,
    0b10101,
    0b11001,
    0b11010,
    0b11011,
    0b11101,
    0b11110,
    0b11111,
    // All zero (invalid)
    0b00000,
};

// Characters in order from most to least frequent. Frequencies are calculated
// by scanning all code and documentation files within my workspace.
static const constexpr char* lowerCase = " iegatbsnoclhrdyumwzvfpxkjq";
static const constexpr char* upperCase = " IEGATBSNOCLHRDYUMWZVFPXKJQ";
static const constexpr char* numeric   = "0.213594,687-=)(*+^%";
static const constexpr char* symbolic  = "\n\b\t></;'\":?}{][#|~\\&!$@";

static Alphabet createAlphabet(const char* charSet)
{
    typedef Alphabet::ChordedChar ChordedChar;
    std::vector<ChordedChar> charMap;
    for (int i = 0; charSet[i] != 0; i++)
    {
        charMap.push_back({ charSet[i], chordConvenienceOrder[i] });
    }
    std::sort(charMap.begin(), charMap.end(),
            [](ChordedChar first, ChordedChar second)
    {
        return first.character < second.character;
    });
    return Alphabet(charMap.data(), charMap.size());
}

Alphabet AlphabetFactory::createLowerCase()
{
    return createAlphabet(lowerCase);
}

Alphabet AlphabetFactory::createUpperCase()
{
    return createAlphabet(upperCase);
}

Alphabet AlphabetFactory::createNumeric()
{
    return createAlphabet(numeric);
}

Alphabet AlphabetFactory::createSymbolic()
{
    return createAlphabet(symbolic);
}

