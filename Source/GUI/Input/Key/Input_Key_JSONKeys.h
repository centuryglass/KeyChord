#pragma once
#include "JuceHeader.h"

namespace Input { namespace Key { namespace JSONKeys
{
    // Chord input keys:
    static const juce::Identifier chord1("Chord1");
    static const juce::Identifier chord2("Chord2");
    static const juce::Identifier chord3("Chord3");
    static const juce::Identifier chord4("Chord4");
    static const juce::Identifier chord5("Chord5");
    const juce::Array<const juce::Identifier*> chordKeys =
            { &chord1, &chord2, &chord3, &chord4, &chord5 };

    // Alphabet selection keys:
    static const juce::Identifier letterAlphabet("Select letter keys");
    static const juce::Identifier numberAlphabet("Select number keys");
    static const juce::Identifier symbolAlphabet("Select symbol keys");
    static const juce::Identifier controlAlphabet("Select control keys");

    // Misc. control keys:
    static const juce::Identifier backspace("Backspace");
    static const juce::Identifier clearAll("Clear all");
    static const juce::Identifier sendText("Send text");
    static const juce::Identifier closeAndSend("Close and Send");
    static const juce::Identifier close("Close");
    static const juce::Identifier toggleImmediate("Toggle immediate mode");
    static const juce::Identifier showHelp("Show help");
    static const juce::Identifier toggleWindowEdge("Toggle window edge");
    static const juce::Identifier toggleMinimize("Toggle minimize");
    
    const juce::Array<const juce::Identifier*> allKeys =
    {
        &chord1,
        &chord2,
        &chord3,
        &chord4,
        &chord5,
        &letterAlphabet,
        &numberAlphabet,
        &symbolAlphabet,
        &controlAlphabet,
        &backspace,
        &clearAll,
        &sendText,
        &closeAndSend,
        &close,
        &toggleImmediate,
        &showHelp,
        &toggleWindowEdge,
        &toggleMinimize
    };

} } }
