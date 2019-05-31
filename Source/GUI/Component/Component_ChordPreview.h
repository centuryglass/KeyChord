#pragma once
/**
 * @file  Component_ChordPreview.h
 *
 * @brief  Displays a preview of a potential chorded keyboard.
 */

#include "Text_CharSet_Cache.h"
#include "Input_Key_ConfigFile.h"
#include "Chord.h"
#include "JuceHeader.h"

namespace Component { class ChordPreview; }

class Component::ChordPreview : public juce::Component
{
public:
    enum ColourIds
    {
        // Color used to draw text:
        text = 0x1900100,
        // Color used to draw text for characters that conflict with the current
        // selection:
        inactiveText = 0x1900101,
        // Colors used to draw active chord keys on the selected character:
        chord1Selected = 0x1900102,
        chord2Selected = 0x1900103,
        chord3Selected = 0x1900104,
        chord4Selected = 0x1900105,
        chord5Selected = 0x1900106,
        emptySelected =  0x1900107, // Empty = key should not be held down.
        // Colors used to draw active chord keys in chords that don't 
        // conflict with the current selection:
        chord1Active = 0x1900108,
        chord2Active = 0x1900109,
        chord3Active = 0x190010a,
        chord4Active = 0x190010b,
        chord5Active = 0x190010c,
        emptyActive =  0x190010d,
        // Colors used to draw active chord keys in chords that don't 
        // conflict with the current selection:
        chord1Open = 0x190010e,
        chord2Open = 0x190010f,
        chord3Open = 0x1900110,
        chord4Open = 0x1900111,
        chord5Open = 0x1900112,
        emptyOpen =  0x1900113,
        // Colors used to draw inactive chord keys that conflict with the
        // current selection:
        chord1Blocked = 0x1900114,
        chord2Blocked = 0x1900115,
        chord3Blocked = 0x1900116,
        chord4Blocked = 0x1900117,
        chord5Blocked = 0x1900118,
        emptyBlocked =  0x1900119,
        // Colors used for active modifier keys:
        activeModText = 0x1900120,
        activeModFill = 0x1900121,
        activeModLine = 0x1900122,
        // Colors used for inactive modifier keys:
        inactiveModText = 0x1900123,
        inactiveModFill = 0x1900124,
        inactiveModLine = 0x1900125,
        // Color used to draw keys that should not be held down on chords that
        // are currently
    };

    /**
     * @brief  Requests keyboard focus on construction.
     */
    ChordPreview();

    virtual ~ChordPreview() { }

    /**
     * @brief  Sets the current state of the chorded keyboard, immediately
     *         redrawing the component if the state changes.
     *
     * @param activeSet       The character set mapping between characters and
     *                        chords.
     *                        
     * @param heldChord       The current held Chord value.
     *
     * @param input           The current cached character index list.
     */
    void updateChordState(const Text::CharSet::Cache* activeSet, 
            const Chord heldChord, 
            const juce::Array<unsigned int> input);

private:
    /**
     * @brief  Draws all chord mappings within the current character set, which
     *         chord keys are currently held down, and the buffered input string 
     *         waiting to be sent to the target window.
     *
     * @param g  The JUCE graphics context.
     */
    void paint(juce::Graphics& g) override;

    // The active character alphabet:
    const Text::CharSet::Cache* charSet = nullptr;
    // The current held input chord:
    Chord lastHeldChord;
    // Buffered input waiting to be sent to the target window:
    juce::Array<unsigned int> bufferedInput;
    // Loads chord key display characters:
    Input::Key::ConfigFile keyConfig;
};
