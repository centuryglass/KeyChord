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

    // Character set selection keys:
    static const juce::Identifier selectMainSet(
            "Select main character set");
    static const juce::Identifier selectAltSet(
            "Select alternate character set");
    static const juce::Identifier selectSpecialSet(
            "Select special character set");
    static const juce::Identifier selectNextSet(
            "Select next character set");

    // Modifiers control:
    static const juce::Identifier openModSelect(
            "Show modifier selector");
    static const juce::Identifier toggleShift(
            "Toggle shift modifier");
    static const juce::Identifier toggleCtrl(
            "Toggle ctrl modifier");
    static const juce::Identifier toggleAlt(
            "Toggle alt modifier");
    static const juce::Identifier toggleCmd(
            "Toggle cmd modifier");

    // Misc. control keys:
    static const juce::Identifier backspace(
            "Backspace");
    static const juce::Identifier clearAll(
            "Clear all");
    static const juce::Identifier sendText(
            "Send text");
    static const juce::Identifier closeAndSend(
            "Close and send");
    static const juce::Identifier close(
            "Close");
    static const juce::Identifier toggleImmediate(
            "Toggle immediate mode");
    static const juce::Identifier showHelp(
            "Show help");
    static const juce::Identifier toggleWindowEdge(
            "Toggle window edge");
    static const juce::Identifier toggleMinimize(
            "Toggle minimize");
    
    const juce::Array<const juce::Identifier*> allKeys =
    {
        &chord1,
        &chord2,
        &chord3,
        &chord4,
        &chord5,
        &selectMainSet,
        &selectAltSet,
        &selectSpecialSet,
        &selectNextSet,
        &openModSelect,
        &toggleShift,
        &toggleCtrl,
        &toggleAlt,
        &toggleCmd,
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
