#pragma once
/**
 * @file  Component_KeyGrid.h
 *
 * @brief  An abstract basis for classes that draw a grid of key values.
 */

#include "Input_Chord.h"
#include "JuceHeader.h"

namespace Component { class KeyGrid; }

namespace Text { namespace CharSet { class Cache; } }

/**
 * @brief  An abstract basis for classes that draw a grid of key values.
 *
 *  KeyGrid is the basis for all classes that draw grids of values using 
 * Text::Painter. It tracks the dimensions of the grid, the current held Chord
 * value, and the active character set, so that inheriting classes can access
 * and draw this information. It also tracks the size and padding space used
 * by grid characters, so different KeyGrid subclasses can be aligned fairly
 * easily.
 */
class Component::KeyGrid : public juce::Component
{
public:
    KeyGrid();
    virtual ~KeyGrid() { }

    /**
     * @brief  Handles changes to the active held chord. This ensures that the
     *         component is redrawn whenever it has new information to display.
     *
     * @param heldChord  The current held Chord value.
     */
    virtual void updateChordState(const Input::Chord heldChord);

    /**
     * @brief  Handles changes to the active character set. This should ensure
     *         the component is redrawn whenever it has new information to
     *         display.
     *
     * @param charSet  The character set mapping between characters and chords.
     */
    virtual void updateCharacterSet(const Text::CharSet::Cache* charSet); 

    /**
     * @brief  Gets the number of character columns the KeyGrid contains.
     *
     * @return  The current width, measured in number of key columns.
     */
    virtual int getColumnCount() const = 0;

    /**
     * @brief  Gets the number of character rows the KeyGrid contains.
     *
     * @return  The current height, measured in number of key rows.
     */
    virtual int getRowCount() const = 0;

    /**
     * @brief  Sets the amount of empty space to leave around drawn keys,
     *         measured as a fraction of key size.
     *
     * @param xPadding  Fraction of key width to leave empty on the left and
     *                  right side of each key.
     *
     * @param yPadding  Fraction of key height to leave empty on the top and 
     *                  bottom side of each key.
     */
    virtual void setPaddingFractions
    (const float xPadding, const float yPadding);

protected:
    /**
     * @brief  Gets the ideal character width, given the current character set,
     *         bounds, and settings.
     *
     * @return  The padded character width in pixels.
     */
    int getPaddedCharWidth() const;

    /**
     * @brief  Gets the ideal character row height, given the current bounds and
     *         settings.
     *
     * @return  The padded character row height in pixels.
     */
    int getPaddedRowHeight() const;

    /**
     * @brief  Gets the amount of space to leave around characters, given the
     *         current character set and bounds.
     *
     * @return  The amount of horizontal padding space, in pixels.
     */
    int getXPadding() const;

    /**
     * @brief  Gets the amount of space to leave around character rows given the
     *         current bounds.
     *
     * @return  The amount of vertical padding space, in pixels.
     */
    int getYPadding() const;

    /**
     * @brief  Gets the key entry chord that's currently held down.
     *
     * @return  A chord value matching the current chord entry state.
     */
    const Input::Chord& getHeldChord() const;

    /**
     * @brief  Gets the current active character set.
     *
     * @return   The active set of typable characters.
     */
    const Text::CharSet::Cache* getActiveSet() const;

private:
    // The current held input chord:
    Input::Chord lastHeldChord;
    // The active character set:
    const Text::CharSet::Cache* activeSet = nullptr;
    // Saved key padding values:
    float xPaddingFraction = 0;
    float yPaddingFraction = 0;
};
