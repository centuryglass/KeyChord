#include "Widgets_BoundedLabel.h"
#include "Layout_Component_ConfigFile.h"

Widgets::BoundedLabel::BoundedLabel(const juce::String componentName,
        const juce::String labelText, const int fontPadding) :
juce::Label(componentName, labelText),
fontPadding(fontPadding)
{
#if JUCE_DEBUG
    setName(juce::String("BoundedLabel:") + labelText);
#endif
}


// Sets the maximum height of the label text.
void Widgets::BoundedLabel::setMaximumTextSize
(const Layout::Component::TextSize newMaximum)
{
    maxSize = newMaximum;
    resized();
}


// Updates the font size when label bounds change.
void Widgets::BoundedLabel::resized()
{
    Layout::Component::ConfigFile config;
    int fontHeight = std::min(config.getFontHeight(maxSize),
            config.getFontHeight(getLocalBounds(), getText()));
    setFont(getFont().withHeight(fontHeight));
}
