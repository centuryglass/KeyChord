#pragma once
/**
 * @file  Component_InputView.h
 *
 * @brief  Shows a preview of buffered input text.
 */

#include "JuceHeader.h"

namespace Component { class InputView; }

class Component::InputView : public juce::Component
{
public:
    InputView() { }
    virtual ~InputView() { }

    enum ColourIds
    {
        // Color used to draw text:
        text           = 0x1900200,
        // Component background:
        background     = 0x1900201,
        // Component outline:
        outline        = 0x1900202,
        // Color used to highlight/underline entered text:
        inputHighlight = 0x1900203
    };

    /**
     * @brief  Updates the array of character indices the InputView will draw.
     *
     * @param updatedInput  Buffered input text, as an array of character code
     *                      values as defined in Text::CharSet::Values.
     */
    void updateInputText(const juce::Array<unsigned int> updatedInput);

private:
    /**
     * @brief  Draws the buffered input text.
     *
     * @param g  The JUCE graphics context.
     */
    void paint(juce::Graphics& g) override;

    juce::Array<unsigned int> inputText;
};
