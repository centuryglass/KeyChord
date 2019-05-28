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
     *                chord.  Bits are counted from least to most significant.
     */
    Chord(const uint8 bitMap);

    /**
     * @brief  Checks if this object represents a valid input chord.
     *
     * @return  Whether the object has a non-zero value, where only valid chord
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
     * @param rhs  The comparison chord.
     *
     * @return     Whether this Chord and rhs use the same set of chord keys.
     */
    bool operator==(const Chord& rhs) const;

    /**
     * @brief  Checks if two chords are not equal.
     *
     * @param rhs  The comparison chord.
     *
     * @return     Whether this Chord and rhs do not use the same set of chord
     *             keys.
     */
    bool operator!=(const Chord& rhs) const;

    /**
     * @brief  Checks if this Chord is a subchord of another Chord.
     *
     * @param rhs  The comparison chord.
     *
     * @return     Whether this Chord doesn't equal rhs, and only uses keys that
     *             rhs uses.
     */
    bool operator<(const Chord& rhs) const;

    /**
     * @brief  Checks if this Chord is a superchord of another Chord.
     *
     * @param rhs  The comparison chord.
     *
     * @return     Whether this Chord doesn't equal rhs, and rhs only uses keys
     *             that this Chord uses.
     */
    bool operator>(const Chord& rhs) const;

    /**
     * @brief  Checks if this Chord equals or is a subchord of another Chord.
     *
     * @param rhs  The comparison chord.
     *
     * @return     Whether this Chord only uses keys that rhs uses.
     */
    bool operator<=(const Chord& rhs) const;

    /**
     * @brief  Checks if this Chord equals or is a superchord of another Chord.
     *
     * @param rhs  The comparison chord.
     *
     * @return     Whether rhs only uses keys that this Chord uses.
     */
    bool operator>=(const Chord& rhs) const;

private:
    // Marks held key indices with individual bits:
    uint8 bitMap = 0;
};
