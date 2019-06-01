#pragma once
/**
 * @file  Component_ChordKeyDisplay.h
 *
 * @brief  Draws a vertical indicator of the state and bindings of the chord
 *         input keys.
 */

#include "Chord.h"
#include "JuceHeader.h"

namespace Component { class ChordKeyDisplay; }

class Component::ChordKeyDisplay : public juce::Component
{
public:
    enum ColourIds
    {
        chord1Selected = 0x1900102,
        chord2Selected = 0x1900103,
        chord3Selected = 0x1900104,
        chord4Selected = 0x1900105,
        chord5Selected = 0x1900106,
    };
    ChordKeyDisplay();

    virtual ~ChordKeyDisplay() { }

    /**
     * @brief  Updates the saved chord value.
     *
     * @param newChord  A new chord for the component to display.
     */
    void updateChord(const Chord newChord);

private:
    /**
     * @brief  Paints the chord keys.
     *
     * @param g  JUCE graphics context.
     */
    void paint(juce::Graphics& g) override;

    // The last held chord value:
    Chord currentChord;
    // Stores the chord key characters:
    juce::Array<unsigned int> chordKeys;
};
