#pragma once
#include "JuceHeader.h"

namespace CharPainter
{
    // Represents a rounded rectangle outline in the character map:
    static const constexpr char outlineChar = -1;
    // Represents a rounded filled rectangle in the character map:
    static const constexpr char fillChar = -2;

    void paintAt(juce::Graphics& g,
            const char toPrint,
            const int x,
            const int y,
            const int width,
            const int height);
}
