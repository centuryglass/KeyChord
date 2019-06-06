#pragma once
/**
 * @file  Input_ChordReader.h
 *
 * @brief  Converts raw keyboard input events to chord input events.
 */

#include "JuceHeader.h"
#include "Chord.h"
#include "Input_Key_ConfigFile.h"
#include "Text_CharSet_Cache.h"
#include "Text_CharSet_ConfigFile.h"

namespace Input { class ChordReader; }

/**
 * @brief  Listens to keyboard input, reading and interpreting input to the
 *         Chord input keys.
 *
 *  Chord reader converts these key events to Chord objects, makes sure these
 * objects update appropriately when the list of held keys changes, and forwards
 * information to Listener objects whenever the keys change or a chord is
 * entered.
 */
class Input::ChordReader : public juce::KeyListener
{
public:
    /**
     * @brief  Create the ChordReader, assigning it to listen to key events
     *         from a component.
     *
     * @param keyComponent  The Component that will provide the ChordReader
     *                      with keyboard events.
     */
    ChordReader(juce::Component* keyComponent);

    virtual ~ChordReader() { }

    /**
     * @brief  Gets the current Chord that will be used if all keys are
     *         released.
     *
     *  This will return a different value than the current held chord only when
     * the user has released keys very recently, in order to ensure chord input
     * isn't changed if the user releases one key a tiny fraction of a second
     * late.
     *
     * @return  The selected chord, or an invalid chord if no chord keys are
     *          held.
     */
    Chord getSelectedChord() const;

    /**
     * @brief  Receives notifications when the selected chord state changes.
     */
    class Listener
    {
    public:
        Listener() { }

        virtual ~Listener() { }

        friend class ChordReader;

    private:
        /**
         * @brief  Notifies the Listener that the currently selected chord has
         *         changed.
         *
         * @param heldChord  An object representing the chord keys that are held
         *                   down.
         */
        virtual void selectedChordChanged(const Chord selectedChord) = 0;

        /**
         * @brief  Notifies the Listener that a chord was entered
         *
         * @param selected  The entered chord value.
         */
        virtual void chordEntered(const Chord selected) = 0;

        /**
         * @brief  Directly passes all key events unrelated to chord state on
         *         to the listener.
         *
         * @param key  The registered key description for the key press event.
         */
        virtual void keyPressed(const juce::KeyPress key) = 0;

        /**
         * @brief  Directly passes all key release events unrelated to chord
         *         state on to all listeners.
         */
        virtual void keyReleased() = 0;
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
     * @brief  Passes the current selected chord to all registered listeners.
     */
    void sendSelectionUpdate();

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
     *                ChordReader is consuming the key event.
     */
    bool keyPressed(const juce::KeyPress& key, juce::Component* source)
            override;

    /**
     * @brief  Tracks when keys are pressed or released.
     *
     * @param isKeyDown  Whether a key was just pressed, or just released.

     * @param source     The component tracked by this ChordReader.
     *
     * @return           True whenever the key was released. This notifies the
     *                   JUCE library that the ChordReader is consuming the key
     *                   event.
     */
    bool keyStateChanged(bool isKeyDown, juce::Component* source) override;

    // Object tracking which keys are held down:
    Chord heldChord = 0;

    // Object tracking the active key selection:
    Chord selectedChord = 0;

    // The list of chord Keys, in order:
    juce::Array<juce::KeyPress> chordKeys;
    // All registered listeners:
    juce::Array<Listener*> listeners;

    // Loads key binding data:
    Input::Key::ConfigFile keyConfig;

    // Loads configurable character maps:
    Text::CharSet::ConfigFile charsetConfig;

    /**
     * @brief  Passes on key release events as selection changes, after waiting
     *         a brief period to make sure the user isn't releasing keys to
     *         enter a chord.
     */
    class ReleaseTimer : public juce::Timer
    {
    public:
        /**
         * @brief  Connects the timer to its chordReader on construction.
         *
         * @param chordReader  The chordReader using this timer.
         */
        ReleaseTimer(ChordReader& chordReader);

        virtual ~ReleaseTimer() {}

    private:
        /**
         * @brief  Updates the ChordReader's selected chord and passes the
         *         update on to all listeners if the delay period finishes
         *         without the user releasing all keys to enter the chord.
         */
        void timerCallback() override;

        ChordReader& chordReader;
    };
    ReleaseTimer releaseTimer;
};
