#include "ChordTracker.h"
#include "AlphabetFactory.h"
#include <map>

// The number of keys that may be held down to make a chord:
static const constexpr int numChordKeys = 5;

// The list of chord keys, in order:
static const juce::String chordKeys [] = {"A", "S", "D", "F", "G"};


// Standard lowercase alphabet:
static const Alphabet lowerCase = AlphabetFactory::createLowerCase();
static const juce::String lowerMod = "H";

// Uppercase alphabet:
static const Alphabet upperCase = AlphabetFactory::createUpperCase();
static const juce::String upperMod = "J";

// Numeric alphabet:
static const Alphabet numeric = AlphabetFactory::createNumeric();
static const juce::String numberMod = "K";

// Symbolic alphabet:
static const Alphabet symbolic = AlphabetFactory::createSymbolic();
static const juce::String symbolMod = "L";

// Milliseconds to wait between key releases before assuming the user isn't 
// releasing keys to enter a chord value, and is instead changing the selected
// chord:
static const constexpr int keyReleaseChordUpdateDelay = 100;

/**
 * @brief  Gets a text representation of a chord bitmap.
 *
 * @param bitmap  A byte where the rightmost five bits store whether the
 *                 corresponding chord key is held down.
 */
static juce::String chordString(const juce::uint8 bitmap)
{
    juce::String binaryStr = "";
    for (int i = 0; i < numChordKeys; i++)
    {
        const juce::uint8 keymap = 1 << (numChordKeys - i - 1);
        if ((keymap & bitmap) != 0)
        {
            binaryStr = chordKeys[i] + binaryStr;
        }
        else
        {
            binaryStr = "_" + binaryStr;
        }
    }
    return binaryStr;
}

// Create the ChordTracker, assigning it to listen to key events from a
// component.
ChordTracker::ChordTracker(juce::Component* keyComponent) : releaseTimer(*this)
{
    keyComponent->addKeyListener(this);
}


// Gets the set of keys that are currently held down.
juce::uint8 ChordTracker::getHeldKeys() const
{
    return heldKeys;
}


// Gets the current key selection that will be used if all keys are released.
juce::uint8 ChordTracker::getSelectedKey() const
{
    return selectedKey;
}


// Gets the current active alphabet.
const Alphabet* ChordTracker::getAlphabet() const
{
    switch (activeAlphabet)
    {
        case AlphabetType::lowerCase:
            return &lowerCase;
        case AlphabetType::upperCase:
            return &upperCase;
        case AlphabetType::numeric:
            return &numeric;
        case AlphabetType::symbolic:
            return &symbolic;
    }
    DBG("Invalid alphabet type " << (int) activeAlphabet);
    jassertfalse;
    return &lowerCase;
}


// Gets the number of chord keys used:
const int ChordTracker::getNumChordKeys() const
{
    return numChordKeys;
}


// Adds a listener to receive chord event updates.
void ChordTracker::addListener(Listener* listener)
{
    listeners.addIfNotAlreadyThere(listener);
}


// Removes a listener from the list of registered listeners.
void ChordTracker::removeListener(Listener* listener)
{
    listeners.removeAllInstancesOf(listener);
}


// Passes the current selected chord to all registered listeners.
void ChordTracker::sendSelectionUpdate()
{
    for(Listener* listener : listeners)
    {
        listener->heldKeysChanged(selectedKey);
    }
}


// Registers relevant key presses, and prevents KeyPress events from being
// passed to other components.
bool ChordTracker::keyPressed
(const juce::KeyPress& key, juce::Component* source)
{
    using juce::juce_wchar;
    juce::String keyText = key.getTextDescription();

    // Check for alphabet change modifiers:
    AlphabetType newAlphabet = activeAlphabet;
    if (keyText == lowerMod)
    {
        DBG("Switching to lowerCase alphabet");
        newAlphabet = AlphabetType::lowerCase;
    }
    else if (keyText == upperMod)
    {
        DBG("Switching to upperCase alphabet");
        newAlphabet = AlphabetType::upperCase;
    }
    else if (keyText == numberMod)
    {
        DBG("Switching to numeric alphabet");
        newAlphabet = AlphabetType::numeric;
    }
    else if (keyText == symbolMod)
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
    bool changed = false;
    for (int i = 0; i < numChordKeys; i++)
    {
        if (chordKeys[i] == keyText)
        {
            juce::uint8 keyMask = 1 << i;
            heldKeys = heldKeys | keyMask;
            selectedKey = selectedKey | keyMask;
            sendSelectionUpdate();
            return true;
        }
    }

    // Pass any unprocessed key events on to listeners:
    for (Listener* listener : listeners)
    {
        listener->keyPressed(key.getTextDescription());
    }

    return true;
}


// A shorter alias for juce::KeyPress::createFromDescription.
static inline juce::KeyPress getKey(const juce::String& description)
{
    return juce::KeyPress::createFromDescription(description);
}


// Tracks when keys are pressed or released.
bool ChordTracker::keyStateChanged(bool isKeyDown, juce::Component* source)
{
    using juce::String;
    if (isKeyDown)
    {
        return false;
    }

    // Check for released chord keys:
    juce::uint8 updatedMask = heldKeys;
    for (int i = 0; i < numChordKeys; i++)
    {
        juce::uint8 keyMask = 1 << i;
        if ((heldKeys & keyMask) == keyMask)
        {
            const juce::KeyPress heldKey = getKey(String(chordKeys[i]));
            if ((keyMask & updatedMask) == keyMask
                    && ! heldKey.isCurrentlyDown())
            {
                updatedMask = updatedMask & (~keyMask);
            }
        }
    }

    // Save updates to chord keys, only updating the selection if a reasonable
    // amount of time passes between key release events:
    if (updatedMask != heldKeys)
    {
        // Stop the release timer if it was previously running:
        releaseTimer.stopTimer();

        heldKeys = updatedMask;
        // If all keys are released, send the selected chord to registered
        // listeners and reset the selection:
        if (heldKeys == 0)
        {
            DBG("Entered chord " << chordString(selectedKey) << ", with char "
                    << getAlphabet()->getCharacter(selectedKey));
            for (Listener* listener : listeners)
            {
                listener->chordEntered(selectedKey);
            }
            selectedKey = heldKeys;
        }
        // Otherwise, start the timer and let it update the selection.
        else
        {
            releaseTimer.startTimer(keyReleaseChordUpdateDelay);
        }
    }
    return true;
}


// Connects the timer to its chordTracker on construction.
ChordTracker::ReleaseTimer::ReleaseTimer(ChordTracker& chordTracker) :
        chordTracker(chordTracker) { }


// Updates the ChordTracker's selected chord and passes the update on to all
// listeners if the delay period finishes without the user releasing all keys to
// enter the chord.
void ChordTracker::ReleaseTimer::timerCallback()
{
    const juce::MessageManagerLock mmLock;
    if (chordTracker.heldKeys != chordTracker.selectedKey)
    {
        chordTracker.selectedKey = chordTracker.heldKeys;
        chordTracker.sendSelectionUpdate();
    }
    stopTimer();
}
