#pragma once
/**
 * @file  Chord.h
 *
 * @brief  Represents an input event created by releasing several keys at once.
 */

#include "JuceHeader.h"

/**
 * @brief  Represents a key combination that, when released, may be used to
 *         select a specific keyboard input event.
 *
 *  Chorded input allows 31 different characters to be typed using only five 
 * keys. Any combination of chord keys is held down, and then are released at
 * once. When all keys are released, the combination of keys that was held down
 * last selects the input character.
 */
class Chord
{
public:
    typedef unsigned char uint8;

    /**
     * @brief  Creates an invalid chord, with no held keys.
     */
    Chord() { }

    /**
     * @brief  Creates a chord from its raw bitmap value.
     *
     * @param bitMap  A byte value where each bit is set to one if and only if a
     *                chord key at the same index is held down to create this
     *                chord. Bits are counted from least to most significant.
     */
    Chord(const uint8 bitMap);

    /**
     * @brief  Checks if this object represents a valid input chord.
     *
     * @return  Whether the object has a nonzero value, where only valid chord
     *          key bits are set to one.
     */
    bool isValid() const;

    /**
     * @brief  Creates a string representation of this chord for debugging.
     *
     * @return  A string used to show which chord keys are held down to create
     *          this chord.
     */
    juce::String toString() const;

    /**
     * @brief  Checks if a particular chord key index is held down when typing
     *         this chord.
     *
     * @param keyIndex  A valid index of a chord input key.
     *
     * @return          True if the key is used, false if the key is not used or
     *                  is invalid.
     */
    bool usesChordKey(const uint8 keyIndex) const;

    /**
     * @brief  Checks if this chord only holds down keys that are used in typing
     *         another chord.
     *
     * @param possibleSuperChord  A possible larger chord containing this chord.
     *
     * @return                    Whether all keys in this chord are also held
     *                            down when typing possibleSuperChord.
     */
    bool isSubchordOf(const Chord& possibleSuperChord) const;

    /**
     * @brief  Gets the Chord that this Chord would become if a new key is held
     *         down.
     *
     * @param keyIndex  The index of a valid chord input key.
     *
     * @return          The new chord that would be created if the key is held,
     *                  or the same chord if the given key is already held down
     *                  in this chord, or an invalid chord if keyIndex is
     *                  invalid.
     */
    Chord withKeyHeld(const uint8 keyIndex) const;

    /**
     * @brief  Gets the Chord that this Chord would become if a chord key is
     *         released.
     *
     * @param keyIndex  The index of a valid chord input key.
     *
     * @return          The new chord that would be created if the key is
     *                  released, or the same chord if the given key is already
     *                  not held down in this chord, or an invalid chord if
     *                  keyIndex is invalid.
     */
    Chord withKeyReleased(const uint8 keyIndex) const;

    /**
     * @brief  Gets the number of chord input keys are used.
     *
     * @return  The maximum number of keys allowed when typing any chord.
     */
    static int numChordKeys();

    /**
     * @brief  Gets the bitmap value used to create this chord.
     *
     * @return  A bitmap marking which keys are held down in this chord.
     */
    uint8 getByteValue() const;

    /**
     * @brief  Checks if two chords are equal.
     *
     * @param rhs  The chord being compared with this chord.
     *
     * @return     Whether this Chord and rhs use the same set of chord keys.
     */
    bool operator==(const Chord& rhs) const;

    /**
     * @brief  Checks if two chords are not equal.
     *
     * @param rhs  The chord being compared with this chord.
     *
     * @return     Whether this Chord and rhs do not use the same set of chord
     *             keys.
     */
    bool operator!=(const Chord& rhs) const;

    /**
     * @brief  Checks if this Chord comes after another Chord when sorting chord
     *         values.
     *
     * @param rhs  The chord being compared with this chord.
     *
     * @return     Whether this Chord should be placed after rhs. 
     */
    bool operator<(const Chord& rhs) const;

    /**
     * @brief  Checks if this Chord comes before another Chord when sorting
     *         chord values.
     *
     * @param rhs  The chord being compared with this chord.
     *
     * @return     Whether this Chord should be placed before rhs.
     */
    bool operator>(const Chord& rhs) const;

private:
    // Each of the first [numChordKeys()] bits in this value marks if a key is
    // held down.
    uint8 bitMap = 0;
};
