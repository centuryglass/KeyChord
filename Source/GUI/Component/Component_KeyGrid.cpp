#include "Component_KeyGrid.h"
#include "Text_CharSet_Cache.h"

Component::KeyGrid::KeyGrid() { }


// Handles changes to the active held chord and character set. This should
// ensure the component is redrawn whenever it has new information to display.
void Component::KeyGrid::updateChordState(const Chord heldChord)
{
    if (lastHeldChord != heldChord)
    {
        lastHeldChord = heldChord;
        repaint();
    }
}


// Handles changes to the active character set. This should ensure the component
// is redrawn whenever it has new information to display.
void Component::KeyGrid::updateCharacterSet
(const Text::CharSet::Cache* charSet)
{
    if (activeSet != charSet && charSet != nullptr)
    {
        activeSet = charSet;
        repaint();
    }
}


// Sets the amount of empty space to leave around drawn keys, measured as a
// fraction of key size.
void Component::KeyGrid::setPaddingFractions
(const float xPadding, const float yPadding)
{
    xPaddingFraction = xPadding;
    yPaddingFraction = yPadding;
    repaint();
}


// Gets the ideal character width, given the current character set, bounds, and
// settings.
int Component::KeyGrid::getPaddedCharWidth() const
{
    return getWidth() / getColumnCount();
}


// Gets the ideal character row height, given the current bounds and settings.
int Component::KeyGrid::getPaddedRowHeight() const
{
    return getHeight() / getRowCount();
}


// Gets the amount of space to leave around characters, given the current
// character set and bounds.
int Component::KeyGrid::getXPadding() const
{
    if (xPaddingFraction == 0)
    {
        return 0;
    }
    int paddedWidth = getPaddedCharWidth();
    return (xPaddingFraction * paddedWidth) / (xPaddingFraction + 1);
}


// Gets the amount of space to leave around character rows given the current
// bounds.
int Component::KeyGrid::getYPadding() const
{
    if (yPaddingFraction == 0)
    {
        return 0;
    }
    int paddedHeight = getPaddedRowHeight();
    return (yPaddingFraction * paddedHeight) / (yPaddingFraction + 1);
}


// Gets the current held key entry chord.
const Chord& Component::KeyGrid::getHeldChord() const
{
    return lastHeldChord;
}


// Gets the current active character set.
const Text::CharSet::Cache* Component::KeyGrid::getActiveSet() const
{
    return activeSet;
}
