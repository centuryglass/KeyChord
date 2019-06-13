#pragma once
/**
 * @file  Input_Controller.h
 *
 * @brief  Listens to key chord events and other keyboard input events,
 *         ensuring that they are handled appropriately.
 */

#include "Input_ChordReader.h"
#include "Input_Key_ConfigFile.h"
#include "Output_Buffer.h"
#include "Text_CharSet_ConfigFile.h"
#include "Text_CharTypes.h"
#include "Config_MainFile.h"
#include "Locale_TextUser.h"
#include "Component_MainView.h"
#include "JuceHeader.h"

namespace Input { class Controller; }
namespace juce { class KeyPress; }

/**
 * @brief  Catches and handles all user input.
 *
 *  All chord entry events and key events are passed to the Controller, and the
 * controller is completely responsible for deciding how they are handled.
 * The Controller uses the Input::Key::ConfigFile to determine which key events
 * control specific actions.
 */
class Input::Controller : public ChordReader::Listener, public Locale::TextUser
{
public:
    /**
     * @brief  Sets up all keyboard input handling.
     *
     * @param mainView        The component used to listen for keyboard events
     *                        and draw the chord state.
     *
     * @param targetWindow    The ID of the window where chord input text should
     *                        be sent.
     *
     * @param outputBuffer    Buffer object used to store text before sending
     *                        it to the target application.
     *
     */
    Controller(Component::MainView* mainView, const int targetWindow,
            Output::Buffer& buffer);

    virtual ~Controller() { }

private:
    /**
     * @brief  Gets a CharString displaying appropriate input preview text.
     *
     * @return  A string listing all active modifiers, along with either the
     *          buffered input text or a message that immediate mode is enabled.
     */
    Text::CharString getInputPreview() const;

    /**
     * @brief  Updates the MainView when the current held chord changes.
     *
     * @param selectedChord   An object representing the set of held chord keys.
     */
    void selectedChordChanged(const Chord selectedChord) override;

    /**
     * @brief  Adds the new chord character to the input buffer and updates the
     *         MainView when a chord character is entered.
     *
     * @param selected  The selected chord value.
     */
    void chordEntered(const Chord selected) override;

    /**
     * @brief  Handles all key commands defined in the key input configuration
     *         file.
     *
     * @param key  The registered key description for the key press event.
     */
    void keyPressed(const juce::KeyPress key) override;

    /**
     * @brief  Ensures the help screen is currently closed.
     */
    void closeHelpScreen();

    /**
     * @brief  Currently does nothing, but may later be used to handle key
     *         release events not caused by chord keys.
     */
    void keyReleased() override { }


    // Loads key bindings:
    Input::Key::ConfigFile keyConfig;
    // Loads configurable character set:
    Text::CharSet::ConfigFile charsetConfig;
    // Loads saved application state:
    Config::MainFile mainConfig;

    // Prevents new input events from triggering before old events finish:
    juce::CriticalSection inputGuard;

    // Captures keyboard input and draws the chord entry state:
    Component::MainView* mainView;
    // Converts generic keyboard events to chord input events:
    ChordReader chordReader;
    // Buffers text waiting to be sent to the target window:
    Output::Buffer& outputBuffer;
    // Stores the target window ID:
    const int targetWindow;
};
