#pragma once
/**
 * @file  Component_ChordKeyDisplay.h
 *
 * @brief  Draws a vertical indicator of the state and bindings of the chord
 *         input keys.
 */

#include "Component_KeyGrid.h"
#include "Text_CharTypes.h"
#include "JuceHeader.h"

namespace Component { class ChordKeyDisplay; }

/**
 * @brief  Draws key images representing each chord key, highlighting each key
 *         if it is currently held down.
 *
 * Chord keys are drawn in a single vertical column.
 */
class Component::ChordKeyDisplay : public Component::KeyGrid
{
public:
    /**
     * @brief  Loads and saves chord key bindings on construction.
     */
    ChordKeyDisplay();

    virtual ~ChordKeyDisplay() { }

    /**
     * @brief  Gets the number of character columns the KeyGrid contains.
     *
     * @return  One, as the ChordKeyDisplay arranges all chord keys in one
     *          column.
     */
    int getColumnCount() const override;

    /**
     * @brief  Gets the number of character rows the KeyGrid contains.
     *
     * @return  The number of chord input keys.
     */
    int getRowCount() const override;

private:
    /**
     * @brief  Paints the chord keys.
     *
     * @param g  JUCE graphics context.
     */
    void paint(juce::Graphics& g) override;

    // Stores the chord key characters:
    Text::CharString chordKeys;
};
