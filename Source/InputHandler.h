#pragma once
/**
 * @file  InputHandler.h
 *
 * @brief  Listens to key chord events and other keyboard input events,
 *         ensuring that they are handled appropriately.
 */

#include "ChordTracker.h"
#include "ChordComponent.h"
#include "InputBuffer.h"
#include "Alphabet.h"
#include "JuceHeader.h"

class InputHandler : public ChordTracker::Listener
{
public:
    /**
     * @brief  Sets up all keyboard input handling.
     *
     * @param chordComponent  The component used to listen for keyboard events
     *                        and draw the chord state.
     *
     * @param targetWindow    The ID of the window where chord input text should
     *                        be sent.
     *
     * @param keyChordWindow  The ID of this application's single window.
     */
    InputHandler(ChordComponent* chordComponent, Window targetWindow,
            const Window keyChordWindow);

    virtual ~InputHandler() { }

private:
    /**
     * @brief  Updates the ChordComponent when the current held chord changes.
     *
     * @param heldKeys  A bit mask representing the chord keys that are held
     *                  down.
     */
    void heldKeysChanged(const juce::uint8 heldKeys) override;

    /**
     * @brief  Adds the new chord character to the input buffer and updates the
     *         ChordComponent when a chord character is entered.
     *
     * @param selected  The selected character's index in the active alphabet.
     */
    void chordEntered(const juce::uint8 selected) override;

    /**
     * @brief  Updates the ChordComponent when the active alphabet changes.
     *
     * @param alphabet  The new active character set.
     */
    void alphabetChanged(const Alphabet* alphabet) override;

    /**
     * @brief  Handles key commands used to send buffered text, delete buffered
     *         text, or close the application.
     *
     * @param key  The registered key description for the key press event.
     */
    void keyPressed(const juce::String key) override;

    // Captures keyboard input and draws the chord entry state:
    ChordComponent* chordComponent;
    // Converts generic keyboard events to chord input events:
    ChordTracker chordTracker;
    // Buffers text input and sends it out to the target window:
    InputBuffer inputBuffer;

    // When immediate mode is active, input is immediately forwarded to the 
    // target window without buffering.
    bool immediateMode = false;
};
