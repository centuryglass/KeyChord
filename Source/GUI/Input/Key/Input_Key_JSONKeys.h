#pragma once
/**
 * @file  Input_Key_JSONKeys.h
 *
 * @brief  Declares the keys used to store data within the key binding
 *         configuration file.
 */

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

    // Main control keys:
    static const juce::Identifier sendText(
            "Send text");
    static const juce::Identifier backspace(
            "Backspace");
    static const juce::Identifier selectNextSet(
            "Select next character set");
    static const juce::Identifier toggleShift(
            "Toggle shifted characters");
    static const juce::Identifier clearAll(
            "Clear all");
    static const juce::Identifier closeAndSend(
            "Close and send");
    static const juce::Identifier close(
            "Close");

    // Misc. control keys:
    static const juce::Identifier toggleImmediate(
            "Toggle immediate mode");
    static const juce::Identifier showHelp(
            "Show help");
    static const juce::Identifier toggleWindowEdge(
            "Toggle window edge");
    static const juce::Identifier toggleMinimize(
            "Toggle minimize");

    // Specific character set selection keys:
    static const juce::Identifier selectMainSet(
            "Select main character set");
    static const juce::Identifier selectAltSet(
            "Select alternate character set");
    static const juce::Identifier selectSpecialSet(
            "Select special character set");
    static const juce::Identifier selectModSet(
            "Select modifier set");
    
    const juce::Array<const juce::Identifier*> allKeys =
    {
        &chord1,
        &chord2,
        &chord3,
        &chord4,
        &chord5,
        &sendText,
        &backspace,
        &selectNextSet,
        &toggleShift,
        &clearAll,
        &closeAndSend,
        &close,
        &toggleImmediate,
        &showHelp,
        &toggleWindowEdge,
        &toggleMinimize,
        &selectMainSet,
        &selectAltSet,
        &selectSpecialSet,
        &selectModSet
    };
} } }
