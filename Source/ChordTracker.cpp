#include "ChordTracker.h"
#include "AlphabetFactory.h"
#include <map>

// The number of keys that may be held down to make a chord:
static const constexpr int numChordKeys = 5;


// The list of chord keys, in order:
static const juce::String chordKeys [] = {"A", "S", "D", "F", "G"};


// Standard lowercase alphabet:
static const Alphabet lowerCase = AlphabetFactory::createLowerCase();

// Uppercase alphabet:
static const Alphabet upperCase = AlphabetFactory::createUpperCase();
static const juce::String upperMod = "H";

// Numeric alphabet:
static const Alphabet numeric = AlphabetFactory::createNumeric();
static const juce::String numberMod = "J";

// Symbolic alphabet:
static const Alphabet symbolic = AlphabetFactory::createSymbolic();
static const juce::String symbolMod = "K";

static void printMask(juce::uint8 bitmask)
{
    juce::String binaryStr = "";
    for (int i = 4; i >= 0; i--)
    {
        if (((1 << i) & bitmask) != 0)
        {
            binaryStr += '1';
        }
        else
        {
            binaryStr += '0';
        }
    }
    DBG("Mask=" << binaryStr);
}

// Create the ChordTracker, assigning it to listen to key events from a
// component.
ChordTracker::ChordTracker(juce::Component* keyComponent)
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
const Alphabet& ChordTracker::getAlphabet() const
{
    switch (activeAlphabet)
    {
        case AlphabetType::lowerCase:
            return lowerCase;
        case AlphabetType::upperCase:
            return upperCase;
        case AlphabetType::numeric:
            return numeric;
        case AlphabetType::symbolic:
            return symbolic;
    }
    DBG("Invalid alphabet type " << (int) activeAlphabet);
    jassertfalse;
    return lowerCase;
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


// Registers relevant key presses, and prevents KeyPress events from being
// passed to other components.
bool ChordTracker::keyPressed
(const juce::KeyPress& key, juce::Component* source)
{
    using juce::juce_wchar;
    keyPressedLast = true;
    juce::String keyText = key.getTextDescription();
    if (keyText == "backspace" || keyText == "delete")
    {
        for (Listener* listener : listeners)
        {
            listener->deleteWasPressed();
        }
        return true;
    }

    // Check for alphabet change modifiers:
    if (activeAlphabet == AlphabetType::lowerCase)
    {
        AlphabetType newAlphabet = activeAlphabet;
        if (keyText == upperMod)
        {
            newAlphabet = AlphabetType::upperCase;
        }
        else if (keyText == numberMod)
        {
            newAlphabet = AlphabetType::numeric;
        }
        else if (keyText == symbolMod)
        {
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
            for (Listener* listener : listeners)
            {
                listener->heldKeysChanged(heldKeys);
            }
            // printMask(heldKeys);
            return true;
        }
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

    // Check for alphabet modifier key releases:
    static const std::map<AlphabetType, juce::KeyPress> modMap =
    {
        { AlphabetType::upperCase, getKey(upperMod) },
        { AlphabetType::numeric,   getKey(numberMod) },
        { AlphabetType::symbolic,  getKey(symbolMod) }
    };
    if (activeAlphabet != AlphabetType::lowerCase)
    {
        if (! modMap.at(activeAlphabet).isCurrentlyDown())
        {
            DBG("Released mod key " 
                    << modMap.at(activeAlphabet).getTextDescription());
            activeAlphabet = AlphabetType::lowerCase;
            for (Listener* listener : listeners)
            {
                listener->alphabetChanged(getAlphabet());
            }
        }
    }

    // Check for released modifiers:
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
                // DBG("updatedMask & ~keyMask for " 
                //          << heldKey.getTextDescription());
                // printMask(updatedMask);
                // printMask(~keyMask);
                updatedMask = updatedMask & (~keyMask);
            }
        }
    }
    if (updatedMask != heldKeys)
    {
        heldKeys = updatedMask;
        if (keyPressedLast)
        {
            lastUpdate = juce::Time::getMillisecondCounter();
        }
        else if (juce::Time::getMillisecondCounter() - lastUpdate > 100)
        {
            selectedKey = updatedMask;
        }
        keyPressedLast = false;

        for (Listener* listener : listeners)
        {
            listener->heldKeysChanged(heldKeys);
            if (heldKeys == 0)
            {
                listener->chordEntered(selectedKey);
            }
        }
        printMask(heldKeys);
        if (heldKeys == 0)
        {
            selectedKey = 0;
        }
    }
    return true;
}
