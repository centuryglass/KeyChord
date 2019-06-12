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
#include "Input_Chord.h"
#include "Text_CharSet_Cache.h"
#include "Input_Key_ConfigFile.h"
#include "JuceHeader.h"

/**
 * @brief  Shows a preview of available chord options, and of which chord the
 *         user is selecting.
 *
 *  ChordPreview is meant to be displayed below Component::CharSetDisplay, and
 * beside Component::ChordKeyDisplay. Each column represents an index in the
 * active character set, and each row represents a chord input key.
 *
 *  Each index in ChordPreview's grid is drawn as either a filled square, if its
 * chord key is used with its character index, or an empty square if it is not.
 * The color used to draw the square is a specific configurable color, selected
 * by chord key, whether the square's key is held down, and whether the square's 
 * character is selected or could be selected by holding down more keys.
 */
class Component::ChordPreview : public KeyGrid
{
public:
    ChordPreview() { }

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
     * @return  The number of chord input keys.
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
