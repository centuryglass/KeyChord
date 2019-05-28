#include "Input_Key_Alphabet.h"
#include "Input_Key_AlphabetFactory.h"
#include <vector>
#include <algorithm>

namespace InKey = Input::Key;
namespace KeyFactory = Input::Key::AlphabetFactory;

// Chords arranged in order by how easy they are to type. Use this to pick
// reasonable chord maps.
static const InKey::Alphabet::uint8 chordConvenienceOrder [] =
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

namespace Input { namespace Key
{
    static Alphabet createAlphabet(const char* charSet);
} }

InKey::Alphabet InKey::createAlphabet(const char* charSet)
{
    typedef Alphabet::ChordedChar ChordedChar;
    std::vector<ChordedChar> charMap;
    for (int i = 0; charSet[i] != 0; i++)
    {
        charMap.push_back({ charSet[i], Chord(chordConvenienceOrder[i]) });
    }
    std::sort(charMap.begin(), charMap.end(),
            [](ChordedChar first, ChordedChar second)
    {
        return first.character < second.character;
    });
    return Alphabet(charMap.data(), charMap.size());
}

InKey::Alphabet KeyFactory::createLowerCase()
{
    return createAlphabet(lowerCase);
}

InKey::Alphabet KeyFactory::createUpperCase()
{
    return createAlphabet(upperCase);
}

InKey::Alphabet KeyFactory::createNumeric()
{
    return createAlphabet(numeric);
}

InKey::Alphabet KeyFactory::createSymbolic()
{
    return createAlphabet(symbolic);
}

