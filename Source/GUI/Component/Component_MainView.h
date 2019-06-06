#pragma once
/**
 * @file  Component_MainView.h
 *
 * @brief  Displays all application contents.
 */

#include "Text_CharSet_Cache.h"
#include "Input_Key_ConfigFile.h"
#include "Text_CharSet_ConfigFile.h"
#include "Component_CharsetDisplay.h"
#include "Component_ChordKeyDisplay.h"
#include "Component_ChordPreview.h"
#include "Component_InputView.h"
#include "Component_HelpScreen.h"
#include "Config_MainFile.h"
#include "Chord.h"
#include "JuceHeader.h"

namespace Component { class MainView; }

/**
 * @brief  Loads and arranges all content displayed in the single application
 *         window.
 *
 * MainView is responsible for loading and placing the components that work
 * together to show the keyboard state. It also handles the process of
 * rearranging or replacing these components when the application switches to
 * different display modes, such as the minimized view or the help screen.
 */
class Component::MainView : public juce::Component
{
public:
    /**
     * @brief  Requests keyboard focus on construction.
     */
    MainView();

    virtual ~MainView() { }

    /**
     * @brief  Updates the current state of the chorded keyboard, immediately
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

    /**
     * @brief  Makes sure the background is filled in with the appropriate
     *         background color.
     */
    void paint(juce::Graphics& g) override;

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

    // Keep config data loaded:
    Input::Key::ConfigFile inputConfig;
    Text::CharSet::ConfigFile charsetConfig;
    Config::MainFile mainConfig;
};
