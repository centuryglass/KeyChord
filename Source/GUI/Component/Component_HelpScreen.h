#pragma once
/**
 * @file  Component_HelpScreen.h
 *
 * @brief  Displays localized key binding descriptions to the user.
 */

#include "Input_Key_ConfigFile.h"
#include "Locale_TextUser.h"
#include "Text_CharTypes.h"
#include "JuceHeader.h"

namespace Component { class HelpScreen; }

class Component::HelpScreen : public juce::Component,
        public Locale::TextUser
{
public:
    /**
     * @brief  Loads help text on construction.
     */
    HelpScreen();

    virtual ~HelpScreen() { }

private:
    /**
     * @brief  Prints all help text.
     *
     * @param g  JUCE graphics context object.
     */
    void paint(juce::Graphics& g) override;

    // Ensures key bindings are available.
    Input::Key::ConfigFile keyConfig;

    // Store Chord key info on its own, as it is handled differently:
    // Chord key display symbols:
    Text::CharString chordChars;
    // Chord key names:
    Text::CharString chordNames;
    // Chord key description:
    Text::CharString chordDescription;

    // Single symbols used to represent key commands:
    Text::CharString symbolChars;
    // All key names, divided into lines.
    Text::CharLineArray keyNames;
    // All key action descriptions, divided into lines.
    Text::CharLineArray descriptions;

};
