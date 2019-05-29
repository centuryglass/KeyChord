#pragma once
/**
 * @file  Input_Controller.h
 *
 * @brief  Listens to key chord events and other keyboard input events,
 *         ensuring that they are handled appropriately.
 */

#include "Input_ChordReader.h"
#include "Input_Buffer.h"
#include "Input_Key_Alphabet.h"
#include "Input_Key_ConfigFile.h"
#include "Component_ChordPreview.h"
#include "JuceHeader.h"

namespace Input { class Controller; }

class Input::Controller : public ChordReader::Listener
{
public:
    /**
     * @brief  Sets up all keyboard input handling.
     *
     * @param chordPreview  The component used to listen for keyboard events
     *                        and draw the chord state.
     *
     * @param targetWindow    The ID of the window where chord input text should
     *                        be sent.
     *
     * @param keyChordWindow  The ID of this application's single window.
     */
    Controller(Component::ChordPreview* chordPreview, const Window targetWindow,
            const Window keyChordWindow);

    virtual ~Controller() { }

private:
    /**
     * @brief  Updates the ChordPreview when the current held chord changes.
     *
     * @param selectedChord   An object representing the set of held chord keys.
     */
    void selectedChordChanged(const Chord selectedChord) override;

    /**
     * @brief  Adds the new chord character to the input buffer and updates the
     *         ChordPreview when a chord character is entered.
     *
     * @param selected  The selected chord value.
     */
    void chordEntered(const Chord selected) override;

    /**
     * @brief  Updates the ChordPreview when the active alphabet changes.
     *
     * @param alphabet  The new active character set.
     */
    void alphabetChanged(const Input::Key::Alphabet* alphabet) override;

    /**
     * @brief  Handles key commands used to send buffered text, delete buffered
     *         text, or close the application.
     *
     * @param key  The registered key description for the key press event.
     */
    void keyPressed(const juce::String key) override;

    // Loads key bindings:
    Input::Key::ConfigFile keyConfig;

    // Captures keyboard input and draws the chord entry state:
    Component::ChordPreview* chordPreview;
    // Converts generic keyboard events to chord input events:
    ChordReader chordReader;
    // Buffers text input and sends it out to the target window:
    Buffer inputBuffer;

    // When immediate mode is active, input is immediately forwarded to the 
    // target window without buffering.
    bool immediateMode = false;
};
