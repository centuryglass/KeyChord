#pragma once
/**
 * @file  Component_CharsetDisplay.h
 *
 * @brief  Draws all characters in the active character set, outlining the
 *         selected character, and highlighting all characters that could be
 *         selected by holding down additional chord keys.
 */

namespace Component { class CharsetDisplay; }

#include "Component_KeyGrid.h"

class Component::CharsetDisplay : public Component::KeyGrid
{
public:
    CharsetDisplay() { }
    virtual ~CharsetDisplay() { }

    /**
     * @brief  Gets the number of character columns the KeyGrid contains.
     *
     * @return  One for each character in the current active character set, plus
     *          another for each wide character in the current set.
     */
    int getColumnCount() const override;

    /**
     * @brief  Gets the number of character rows the KeyGrid contains.
     *
     * @return  One, the component only contains a single row.
     */
     int getRowCount() const override;

private:
    /**
     * @brief  Draws all characters in the current active set.
     *
     * @param g  The JUCE graphics context.
     */
    void paint(juce::Graphics& g) override;
};
