#pragma once
/**
 * @file  ChordComponent.h
 *
 * @brief  Displays a preview of a potential chorded keyboard.
 */

#include "JuceHeader.h"
#include "ChordTracker.h"

class ChordComponent : public juce::Component, public ChordTracker::Listener
{
public:
    /**
     * @brief  Initializes the component and starts listening for key events.
     */
    ChordComponent();

private:
    /**
     * @brief  Draws all pressed keys and the demo keyboard.
     *
     * @param g  The JUCE graphics context.
     */
    void paint(juce::Graphics& g) override;

    /**
     * @brief  Notifies the Listener that the set of keys held down has
     *         changed.
     *
     * @param heldKeys  A bit mask representing the chord keys that are held
     *                  down.
     */
    virtual void heldKeysChanged(const juce::uint8 heldKeys) override;

    /**
     * @brief  Notifies the Listener that a character was selected.
     *
     * @param selected  The selected character's index in the active
     *                  alphabet.
     */
    virtual void chordEntered(const juce::uint8 selected) override;

    /**
     * @brief  Notifies the listener that the active alphabet has changed.
     *
     * @param alphabet  The new active character set.
     */
    virtual void alphabetChanged(const Alphabet& alphabet) override;

    /**
     * @brief  Notifies the listener that the backspace or delete key was
     *         pressed.
     */
    virtual void deleteWasPressed() override;

    // The text input field:
    juce::String input;

    // Listens for chord key events:
    ChordTracker chordTracker;
};
