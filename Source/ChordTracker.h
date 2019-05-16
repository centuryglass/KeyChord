#pragma once
/**
 * @file  ChordTracker.h
 *
 * @brief  Tracks chorded keyboard input.
 */

#include "JuceHeader.h"
#include "Alphabet.h"

class ChordTracker : public juce::KeyListener
{
public:
    /**
     * @brief  Create the ChordTracker, assigning it to listen to key events
     *         from a component.
     *
     * @param keyComponent  The Component that will provide the ChordTracker
     *                      with keyboard events.
     */
    ChordTracker(juce::Component* keyComponent);

    virtual ~ChordTracker() { }

    /**
     * @brief  Gets the set of keys that are currently held down.
     *
     * @return   A bit mask representing the chord keys that are held down.
     */
    juce::uint8 getHeldKeys() const;
    
    /**
     * @brief  Gets the current key selection that will be used if all keys are
     *         released.
     *
     * @return  The selected key, or 0 if no key is selected.
     */
    juce::uint8 getSelectedKey() const;

    /**
     * @brief  Gets the current active alphabet.
     *
     * @return  The active Alphabet setting the character set used, its order, 
     *          and its chord values.
     */
    const Alphabet& getAlphabet() const;

    /**
     * @brief  Gets the number of chord keys used:
     *
     * @return  The chord key count.
     */
    const int getNumChordKeys() const;

    /**
     * @brief  Receives notifications when the keyboard chord state changes.
     */
    class Listener
    {
    public:
        Listener() { }

        virtual ~Listener() { }

        friend class ChordTracker;

    private:
        /**
         * @brief  Notifies the Listener that the set of keys held down has
         *         changed.
         *
         * @param heldKeys  A bit mask representing the chord keys that are held
         *                  down.
         */
        virtual void heldKeysChanged(const juce::uint8 heldKeys) = 0;

        /**
         * @brief  Notifies the Listener that a character was selected.
         *
         * @param selected  The selected character's index in the active
         *                  alphabet.
         */
        virtual void chordEntered(const juce::uint8 selected) = 0;

        /**
         * @brief  Notifies the listener that the active alphabet has changed.
         *
         * @param alphabet  The new active character set.
         */
        virtual void alphabetChanged(const Alphabet& alphabet) = 0;

        /**
         * @brief  Notifies the listener that the backspace or delete key was
         *         pressed.
         */
        virtual void deleteWasPressed() = 0;
    };

    /**
     * @brief  Adds a listener to receive chord event updates.
     *
     * @param listener  An object that will receive updates.
     */
    void addListener(Listener* listener);

    /**
     * @brief  Removes a listener from the list of registered listeners.
     *
     * @param listener  The object that will no longer recieve updates.
     */
    void removeListener(Listener* listener);

private:
    /**
     * @brief  Registers relevant key presses, and prevents KeyPress events from
     *         being passed to other components.
     *
     * @param key     The registered key press event.
     *
     * @param source  The component that had keyboard focus when the key was
     *                pressed.
     *
     * @return        True, in order to notify the JUCE library that the
     *                ChordTracker is consuming the key event.
     */
    bool keyPressed(const juce::KeyPress& key, juce::Component* source)
            override;

    /**
     * @brief  Tracks when keys are pressed or released.
     *
     * @param isKeyDown  Whether a key was just pressed, or just released.

     * @param source     The component tracked by this ChordTracker.
     *
     * @return           True whenever the key was released. This notifies the
     *                   JUCE library that the ChordTracker is consuming the key
     *                   event.
     */
    bool keyStateChanged(bool isKeyDown, juce::Component* source) override;

    // Bitmap tracking which keys are held down:
    juce::uint8 heldKeys = 0;

    // Bitmap tracking the active key selection:
    juce::uint8 selectedKey = 0;
    
    /**
     * @brief  All alphabets that may be selected.
     */
    enum class AlphabetType
    {
        lowerCase,
        upperCase,
        numeric,
        symbolic
    };
    // The current active alphabet:
    AlphabetType activeAlphabet = AlphabetType::lowerCase;

    // Tracks the time of the last key event:
    juce::uint32 lastUpdate = 0;

    // Tracks whether the last key event was a key press or a key release:
    bool keyPressedLast = true;

    // All registered listeners:
    juce::Array<Listener*> listeners;
};
