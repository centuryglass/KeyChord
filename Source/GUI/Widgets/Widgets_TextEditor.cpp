#include "Widgets_TextEditor.h"
#include "Layout_Component_ConfigFile.h"
#include "Layout_Component_TextSize.h"

// Minimum fraction of extra space to leave between edited text and component
// bounds:
static const constexpr float minPadding = 0.1;

// Updates the font height, and immediately applies the change to the edited
// text.
void Widgets::TextEditor::setFontHeight(const int newHeight)
{
    juce::Font newFont(newHeight);
    applyFontToAllText(newFont, true);
}


// Update internal text size to match changes to the component's height.
void Widgets::TextEditor::resized()
{
    juce::TextEditor::resized();
    const int minHeight = getHeight() - getHeight() * minPadding;

    using namespace Layout::Component;
    ConfigFile layoutConfig;
    const juce::Array<TextSize> sizes =
    {
        TextSize::largeText,
        TextSize::mediumText,
        TextSize::smallText
    };

    for (const TextSize& sizeType : sizes)
    {
        const int size = layoutConfig.getFontHeight(sizeType);
        if (size <= minHeight)
        {
            setFontHeight(size);
            return;
        }
    }
    setFontHeight(minHeight);
}
