#pragma once
/**
 * @file  Component_ChordPreview.h
 *
 * @brief  Shows a preview of available chord options, and of which chord the
 *         user is selecting.
 */

namespace Component { class ChordPreview; }


#include "Component_KeyGrid.h"
#include "Component_ChordKeyDisplay.h"
#include "Chord.h"
#include "Text_CharSet_Cache.h"
#include "Input_Key_ConfigFile.h"
#include "JuceHeader.h"

class Component::ChordPreview : public KeyGrid
{
public:
    ChordPreview();

    virtual ~ChordPreview() { }

    /**
     * @brief  Gets the number of character columns the KeyGrid contains.
     *
     * @return  One for each character in the current active character set, plus
     *          another for each wide character in the current set.
     */
    int getColumnCount() const override;

    /**
     * @brief  Gets the number of character rows the KeyGrid contains.
     *
     * @return  The number of chord input keys, plus one for the character set
     *          list.
     */
     int getRowCount() const override;

private:
    /**
     * @brief  Draws all chord mappings within the current character set.
     *
     * @param g  The JUCE graphics context.
     */
    void paint(juce::Graphics& g) override;
};
