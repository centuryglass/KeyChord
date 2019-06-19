#include "Input_ChordReader.h"
#include "Input_Key_JSONKeys.h"
#include <map>


// Milliseconds to wait between key releases before assuming the user isn't
// releasing keys to enter a chord value, and is instead changing the selected
// chord:
static const constexpr int keyReleaseChordUpdateDelay = 300;


// Create the ChordReader, assigning it to listen to key events from a
// component.
Input::ChordReader::ChordReader(juce::Component* keyComponent) :
    releaseTimer(*this)
{
    for (const auto* chordKey : Key::JSONKeys::chordKeys)
    {
        chordKeys.add(keyConfig.getBoundKey(*chordKey));
    }
    keyComponent->addKeyListener(this);
}


// Gets the current Chord that will be used if all keys are released.
Input::Chord Input::ChordReader::getSelectedChord() const
{
    return selectedChord;
}


// Adds a listener to receive chord event updates.
void Input::ChordReader::addListener(Listener* listener)
{
    listeners.addIfNotAlreadyThere(listener);
}


// Removes a listener from the list of registered listeners.
void Input::ChordReader::removeListener(Listener* listener)
{
    listeners.removeAllInstancesOf(listener);
}


// Passes the current selected chord to all registered listeners.
void Input::ChordReader::sendSelectionUpdate()
{
    for (Listener* listener : listeners)
    {
        listener->selectedChordChanged(selectedChord);
    }
}


// Registers relevant key presses, and prevents KeyPress events from being
// passed to other components.
bool Input::ChordReader::keyPressed
(const juce::KeyPress& key, juce::Component* source)
{
    juce::String keyText = key.getTextDescription();

    // Check for new chord key presses:
    for (int i = 0; i < Chord::numChordKeys(); i++)
    {
        if (chordKeys[i] == key)
        {
            heldChord = heldChord.withKeyHeld(i);
            if (heldChord != selectedChord)
            {
                selectedChord = heldChord;
                sendSelectionUpdate();
            }
            return true;
        }
    }

    // Pass any unprocessed key events on to listeners:
    for (Listener* listener : listeners)
    {
        listener->keyPressed(key);
    }
    return true;
}


// A shorter alias for juce::KeyPress::createFromDescription.
static inline juce::KeyPress getKey(const juce::String& description)
{
    return juce::KeyPress::createFromDescription(description);
}


// Tracks when keys are pressed or released.
bool Input::ChordReader::keyStateChanged
(bool isKeyDown, juce::Component* source)
{
    using juce::String;
    if (isKeyDown)
    {
        return false;
    }

    // Check for released chord keys:
    Chord updatedChord = heldChord;
    for (int i = 0; i < Chord::numChordKeys(); i++)
    {
        if (heldChord.usesChordKey(i))
        {
            const juce::KeyPress& heldKey = chordKeys[i];
            if (! heldKey.isCurrentlyDown())
            {
                updatedChord = updatedChord.withKeyReleased(i);
            }
        }
    }

    // Save updates to chord keys, only updating the selection if a reasonable
    // amount of time passes between key release events:
    if (updatedChord != heldChord)
    {
        // Stop the release timer if it was previously running:
        releaseTimer.stopTimer();

        heldChord = updatedChord;
        // If all keys are released, send the selected chord to registered
        // listeners and reset the selection:
        if (! heldChord.isValid())
        {
            DBG("Entered chord " << selectedChord.toString());
            for (Listener* listener : listeners)
            {
                listener->chordEntered(selectedChord);
            }
            selectedChord = Chord();
        }
        // Otherwise, start the timer and let it update the selection.
        else
        {
            releaseTimer.startTimer(keyReleaseChordUpdateDelay);
        }
    }
    else
    {
        for (Listener* listener : listeners)
        {
            listener->keyReleased();
        }
    }
    return true;
}


// Connects the timer to its chordReader on construction.
Input::ChordReader::ReleaseTimer::ReleaseTimer(ChordReader& chordReader) :
        chordReader(chordReader) { }


// Updates the ChordReader's selected chord and passes the update on to all
// listeners if the delay period finishes without the user releasing all keys to
// enter the chord.
void Input::ChordReader::ReleaseTimer::timerCallback()
{
    DBG("Chord release timer finished");
    const juce::MessageManagerLock mmLock;
    if (chordReader.heldChord != chordReader.selectedChord)
    {
        chordReader.selectedChord = chordReader.heldChord;
        chordReader.sendSelectionUpdate();
    }
    stopTimer();
}
