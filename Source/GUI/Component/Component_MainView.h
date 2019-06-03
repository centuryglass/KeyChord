#pragma once
/**
 * @file  Component_MainView.h
 *
 * @brief  Displays all application contents.
 */

#include "Text_CharSet_Cache.h"
#include "Input_Key_ConfigFile.h"
#include "Component_CharsetDisplay.h"
#include "Component_ChordKeyDisplay.h"
#include "Component_ChordPreview.h"
#include "Component_InputView.h"
#include "Component_HelpScreen.h"
#include "Chord.h"
#include "JuceHeader.h"

namespace Component { class MainView; }

class Component::MainView : public juce::Component
{
public:
    /**
     * @brief  Requests keyboard focus on construction.
     */
    MainView();

    virtual ~MainView() { }

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

    /**
     * @brief  Shows the help screen if it's not currently visible, or hides it
     *         if it is visible.
     */
    void toggleHelpScreen();

    /**
     * @brief  Checks if the help screen is currently being shown.
     *
     * @return  Whether the help screen is currently visible.
     */
    bool isHelpScreenShowing() const;

private:
    /**
     * @brief  Update child component bounds if the component changes size.
     */
    void resized() override;

    // Displays the state of the chord input keys:
    ChordKeyDisplay chordKeyDisplay;

    // Displays all characters in the active character set:
    CharsetDisplay charsetDisplay;

    // Displays the chord preview:
    ChordPreview chordPreview;

    // Displays buffered input text:
    InputView inputView;

    // Displays help info when enabled:
    HelpScreen helpScreen;
};
