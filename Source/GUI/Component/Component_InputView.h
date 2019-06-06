#pragma once
/**
 * @file  Component_InputView.h
 *
 * @brief  Shows a preview of buffered input text.
 */

#include "Text_CharTypes.h"
#include "JuceHeader.h"

namespace Component { class InputView; }

/**
 * @brief  Shows a preview of buffered input text.
 *
 *  User entered text that has not yet been sent to the target window is
 * displayed in the InputView. This includes active modifiers, always printed
 * before all other text. If immediate mode is enabled and all normal keys
 * are sent immediately to the target window, InputView will instead show any
 * active modifiers, followed by "(Immediate input mode)", or the localized
 * equivalent.
 */
class Component::InputView : public juce::Component
{
public:
    InputView() { }
    virtual ~InputView() { }

    /**
     * @brief  Custom input preview colors:
     */
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
     * @param updatedInput  The buffered input text string.
     */
    void updateInputText(const Text::CharString updatedInput);

private:
    /**
     * @brief  Draws the buffered input text.
     *
     * @param g  The JUCE graphics context.
     */
    void paint(juce::Graphics& g) override;

    // Cached input text:
    Text::CharString inputText;
};
