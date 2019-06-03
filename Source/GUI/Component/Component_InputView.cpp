#include "Component_InputView.h"
#include "Text_CharSet_Values.h"
#include "Text_Painter.h"
#include "Text_ModTracker.h"

// Layout constants:

// Component outline, as a fraction of height:
static const constexpr float outlineFraction = 0.05;
// Minimum outline size in pixels:
static const constexpr int   minimumOutline = 1;
// Maximum character size, as a fraction of height:
static const constexpr float maxCharSize = 0.8;
// Maximum number of text rows allowed:
static const constexpr int maxRows = 1;


// Updates the array of character indices the InputView will draw.
void Component::InputView::updateInputText
(const juce::Array<unsigned int> updatedInput)
{
    inputText = updatedInput;
    repaint();
}


// Draws the buffered input text.
void Component::InputView::paint(juce::Graphics& g)
{
    juce::Rectangle<int> bounds = getLocalBounds();

    // draw background:
    g.setColour(findColour(background));
    g.fillRect(bounds);

    // draw outline:
    const int outlineSize = std::max<int>(getHeight() * outlineFraction,
            minimumOutline);
    g.setColour(findColour(outline));
    g.drawRect(bounds, outlineSize);
    bounds.reduce(outlineSize * 2, outlineSize * 2);

    // Add active modifiers to drawn text:
    using ModKey = Text::ModTracker::ModKey;
    namespace CharValue = Text::CharSet::Values;
    Text::ModTracker modTracker;
    const std::pair<ModKey, unsigned int> modMappings [] =
    {
        { ModKey::control , CharValue::ctrl },
        { ModKey::alt     , CharValue::alt },
        { ModKey::shift   , CharValue::shift },
        { ModKey::command , CharValue::cmd }
    };
    juce::Array<unsigned int> drawnText;
    for (const auto& mapping : modMappings)
    {
        if (modTracker.isKeyHeld(mapping.first))
        {
            drawnText.add(mapping.second);
            drawnText.add((unsigned int) '+');
        }
    }
    drawnText.addArray(inputText);


    g.setColour(findColour(inputHighlight));
    const int rightEdge = Text::Painter::paintString(g, drawnText,
            bounds.getX(), bounds.getY(),
            bounds.getWidth(), bounds.getHeight(),
            bounds.getHeight() * maxCharSize);
    g.fillRect(bounds.getX(), bounds.getY(),
            rightEdge - bounds.getX(), bounds.getHeight());

    g.setColour(findColour(text));
    Text::Painter::paintString(g, drawnText, bounds.getX(), bounds.getY(),
            bounds.getWidth(), bounds.getHeight(),
            bounds.getHeight() * maxCharSize);

}
