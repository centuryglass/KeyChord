#pragma once
/**
 * @file  Component_ChordKeyDisplay.h
 *
 * @brief  Draws a vertical indicator of the state and bindings of the chord
 *         input keys.
 */

#include "Component_KeyGrid.h"
#include "JuceHeader.h"

namespace Component { class ChordKeyDisplay; }

class Component::ChordKeyDisplay : public Component::KeyGrid
{
public:
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
    juce::Array<unsigned int> chordKeys;
};
