#include "Input_ChordReader.h"
#include "Input_Key_AlphabetFactory.h"
#include "Input_Key_JSONKeys.h"
#include <map>

// The number of keys that may be held down to make a chord:
static const constexpr int numChordKeys = 5;

// Standard lowercase alphabet:
static const Input::Key::Alphabet lowerCase 
        = Input::Key::AlphabetFactory::createLowerCase();

// Numeric alphabet:
static const Input::Key::Alphabet numeric
        = Input::Key::AlphabetFactory::createNumeric();

// Symbolic alphabet:
static const Input::Key::Alphabet symbolic
        = Input::Key::AlphabetFactory::createSymbolic();

// Milliseconds to wait between key releases before assuming the user isn't 
// releasing keys to enter a chord value, and is instead changing the selected
// chord:
static const constexpr int keyReleaseChordUpdateDelay = 100;


// Create the ChordReader, assigning it to listen to key events from a
// component.
Input::ChordReader::ChordReader(juce::Component* keyComponent) :
    releaseTimer(*this)
{
    for (const auto* chordKey : Key::JSONKeys::chordKeys)
    {
        chordKeys.add(keyConfig.getBoundKey(*chordKey));
    }
    alphabetKeys[(int) AlphabetType::lowerCase] 
            = keyConfig.getBoundKey(Key::JSONKeys::letterAlphabet);
    alphabetKeys[(int) AlphabetType::numeric] 
            = keyConfig.getBoundKey(Key::JSONKeys::letterAlphabet);
    alphabetKeys[(int) AlphabetType::symbolic] 
            = keyConfig.getBoundKey(Key::JSONKeys::symbolAlphabet);
    keyComponent->addKeyListener(this);
}


// Gets the chord value that's currently held.
Chord Input::ChordReader::getHeldChord() const
{
    return heldChord;
}


// Gets the current Chord that will be used if all keys are released.
Chord Input::ChordReader::getSelectedChord() const
{
    return selectedChord;
}


// Gets the current active alphabet.
const Input::Key::Alphabet* Input::ChordReader::getAlphabet() const
{
    switch (activeAlphabet)
    {
        case AlphabetType::lowerCase:
            return &lowerCase;
        case AlphabetType::numeric:
            return &numeric;
        case AlphabetType::symbolic:
            return &symbolic;
    }
    DBG("Invalid alphabet type " << (int) activeAlphabet);
    jassertfalse;
    return &lowerCase;
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
    for(Listener* listener : listeners)
    {
        listener->selectedChordChanged(selectedChord);
    }
}


// Registers relevant key presses, and prevents KeyPress events from being
// passed to other components.
bool Input::ChordReader::keyPressed
(const juce::KeyPress& key, juce::Component* source)
{
    using juce::juce_wchar;
    juce::String keyText = key.getTextDescription();

    // Check for alphabet change modifiers:
    AlphabetType newAlphabet = activeAlphabet;
    if (key == alphabetKeys[(int) AlphabetType::lowerCase])
    {
        DBG("Switching to lowerCase alphabet");
        newAlphabet = AlphabetType::lowerCase;
    }
    else if (key == alphabetKeys[(int) AlphabetType::numeric])
    {
        DBG("Switching to numeric alphabet");
        newAlphabet = AlphabetType::numeric;
    }
    else if (key == alphabetKeys[(int) AlphabetType::symbolic])
    {
        DBG("Switching to symbolic alphabet");
        newAlphabet = AlphabetType::symbolic;
    }

    if (newAlphabet != activeAlphabet)
    {
        activeAlphabet = newAlphabet;
        for (Listener* listener : listeners)
        {
            listener->alphabetChanged(getAlphabet());
        }
        return true;
    }

    // Check for new chord key presses:
    for (int i = 0; i < numChordKeys; i++)
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
        listener->keyPressed(keyText);
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
    for (int i = 0; i < numChordKeys; i++)
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
            DBG("Entered chord " << selectedChord.toString() << ", with char '"
                    << getAlphabet()->getChordCharacter(selectedChord) << "'");
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
    const juce::MessageManagerLock mmLock;
    if (chordReader.heldChord != chordReader.selectedChord)
    {
        chordReader.selectedChord = chordReader.heldChord;
        chordReader.sendSelectionUpdate();
    }
    stopTimer();
}
