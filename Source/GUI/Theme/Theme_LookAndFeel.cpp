#include "Theme_LookAndFeel.h"
#include "Theme_Colour_JSONKeys.h"
#include "Layout_Component_ConfigFile.h"
#include "Widgets_Switch.h"
#include "Widgets_BoundedLabel.h"
#include "Widgets_DrawableImage.h"
#include "Widgets_ListEditor.h"
#include "Widgets_FilePathEditor.h"
#include "Widgets_OverlaySpinner.h"

// TODO: Get rid of all the inline magic numbers used for calculating sizes,
//       make them into proper constexpr values defined up here.

Theme::LookAndFeel::LookAndFeel() :
seguibl(juce::Typeface::createSystemTypefaceFor(BinaryData::LatoRegular_ttf,
        BinaryData::LatoRegular_ttfSize))
{
    using juce::Array;
    using juce::Identifier;
    const Array<int>& colourIds = Colour::JSONKeys::getColourIds();
    for (const int& id : colourIds)
    {
        addTrackedColourId(id);
    }
    const Array<Identifier>& categoryKeys
            = Colour::JSONKeys::getCategoryKeys();
    for (const Identifier& key : categoryKeys)
    {
        addTrackedKey(key);
    }
    DBG("Theme::LookAndFeel::LookAndFeel: Tracking " << colourIds.size()
            << " colour id values, " << categoryKeys.size()
            << " category keys.");
    loadAllConfigProperties();
}


// Get the appropriate typeface for the given font.
juce::Typeface::Ptr Theme::LookAndFeel::getTypefaceForFont
(const juce::Font &font)
{
    return seguibl;
}


// Draws the thumb portion of a linear Slider.
void Theme::LookAndFeel::drawLinearSliderThumb(
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider &slider)
{
    using juce::Slider;
    const float radius = getSliderThumbRadius(slider);
    g.setColour(slider.findColour(Slider::thumbColourId));
    float kx, ky;
    if (style == Slider::LinearVertical)
    {
        kx = x + width * 0.5f;
        ky = sliderPos;
    }
    else
    {
        kx = sliderPos;
        ky = y + height * 0.5f;
    }

    juce::Path circle;
    circle.addCentredArc(kx, ky, radius, radius, 0.0f, 0.0f, M_PI * 2.0f);
    circle.closeSubPath();
    g.fillPath(circle);
}


// Draws the background of a linear slider.
void Theme::LookAndFeel::drawLinearSliderBackground(
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider &slider)
{
    const float radius = std::floor(getSliderThumbRadius(slider) * 0.333f);
    g.setColour(slider.findColour(juce::Slider::backgroundColourId));
    juce::Path indent;
    if (slider.isHorizontal())
    {
        const float iy = y + height * 0.5f - radius;
        indent.addRoundedRectangle(
                x - radius,
                iy,
                width + radius * 2.0f,
                radius * 2.0f, 1);
    }
    else
    {
        const float ix = x + width * 0.5f - radius;
        indent.addRoundedRectangle(
                ix,
                y - radius,
                radius * 2.0f,
                height + radius * 2.0f,
                1);
    }
    g.fillPath(indent);
}


// Draws the entire linear slider component.
void Theme::LookAndFeel::drawLinearSlider(
        juce::Graphics &g,
        int x,
        int y,
        int width,
        int height,
        float sliderPos,
        float minSliderPos,
        float maxSliderPos,
        const juce::Slider::SliderStyle style,
        juce::Slider &slider)
{
    drawLinearSliderBackground(g, x, y, width, height,
            sliderPos, minSliderPos, maxSliderPos, style,
            slider);
    drawLinearSliderThumb(g, x, y, width, height, sliderPos,
            minSliderPos, maxSliderPos, style,
            slider);
}


// Defines the radius in pixels of the slider thumb.
int Theme::LookAndFeel::getSliderThumbRadius(juce::Slider &slider)
{
    return juce::jmin(14, slider.getHeight() / 2, slider.getWidth() / 2);
}


// Draws the text onto a TextButton.
void Theme::LookAndFeel::drawButtonText(
        juce::Graphics &g,
        juce::TextButton &button,
        bool isMouseOverButton,
        bool isButtonDown)
{
    if (button.getButtonText().isEmpty() || button.getBounds().isEmpty())
    {
        return;
    }
    using juce::jmin;
    using juce::Font;
    using juce::TextButton;
    int yIndent = jmin(4, button.proportionOfHeight(0.3f));
    int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

    int leftIndent = 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2);
    int rightIndent = 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2);
    juce::Rectangle<int> textBounds = button.getLocalBounds();
    int width = std::max(0, textBounds.getWidth() - leftIndent - rightIndent);
    int height = std::max(0, textBounds.getHeight() - 2 * yIndent);
    Font font(getTextButtonFont(button, button.getHeight()));
    font.setExtraKerningFactor(0.06f);
    textBounds.setWidth(width);
    textBounds.setHeight(height);

    using TextSize = Layout::Component::TextSize;
    Layout::Component::ConfigFile layoutConfig;
    int fontHeight = layoutConfig.getFontHeight
            (textBounds, button.getButtonText());
    font.setHeight(fontHeight);
    g.setFont(font);

    juce::Colour buttonColour = button.findColour(button.getToggleState() ?
            TextButton::textColourOnId : TextButton::textColourOffId);
    if (!button.isEnabled())
    {
        buttonColour = buttonColour.withMultipliedAlpha(0.5f);
    }
    if (isButtonDown && isMouseOverButton)
    {
        buttonColour = buttonColour.darker();
    }
    g.setColour(buttonColour);

    g.drawFittedText(button.getButtonText(), leftIndent, yIndent,
            width, height, juce::Justification::centred, 2);
}


// Draws the background of a Button component.
void Theme::LookAndFeel::drawButtonBackground(
        juce::Graphics &g,
        juce::Button &button,
        const juce::Colour &backgroundColour,
        bool isMouseOverButton,
        bool isButtonDown)
{
    using juce::TextButton;
    const int width = button.getWidth();
    const int height = button.getHeight();

    juce::Path path;
    path.addRoundedRectangle(0, 0, width, height, 1);
    juce::Colour buttonColour = (button.getToggleState() ?
            findColour(TextButton::ColourIds::buttonOnColourId)
            : findColour(TextButton::ColourIds::buttonColourId));
    if (isButtonDown && isMouseOverButton)
    {
        buttonColour = buttonColour.darker();
    }
    g.setColour(buttonColour);
    g.fillPath(path);
}


// Gets the default font to use for popup menu text.
juce::Font Theme::LookAndFeel::getPopupMenuFont()
{
    using TextSize = Layout::Component::TextSize;
    Layout::Component::ConfigFile config;
    return juce::Font(config.getFontHeight(TextSize::mediumText));
}


// Gets the default font to use for combo box text.
juce::Font Theme::LookAndFeel::getComboBoxFont(juce::ComboBox& comboBox)
{
    using TextSize = Layout::Component::TextSize;
    Layout::Component::ConfigFile config;
    int height = config.getFontHeight(comboBox.getLocalBounds(),
            comboBox.getText());
    height = std::min( config.getFontHeight(TextSize::mediumText), height);
    return juce::Font(height);
}


// Gets the default font to use for Label components.
juce::Font Theme::LookAndFeel::getLabelFont(juce::Label& label)
{
    // Let bounded labels set their own font
    Widgets::BoundedLabel* testBounded = dynamic_cast<Widgets::BoundedLabel*>
            (&label);
    if (testBounded != nullptr)
    {
        return label.getFont();
    }
    Layout::Component::ConfigFile config;
    using TextSize = Layout::Component::TextSize;
    int height = std::min(config.getFontHeight(TextSize::smallText),
            config.getFontHeight(label.getLocalBounds(), label.getText()));
    return juce::Font(height);
}


// Gets the default font to use for alert window title text.
juce::Font Theme::LookAndFeel::getAlertWindowTitleFont()
{
    Layout::Component::ConfigFile config;
    using TextSize = Layout::Component::TextSize;
    return juce::Font(config.getFontHeight(TextSize::largeText));
}


// Gets the default font to use for alert window message text.
juce::Font Theme::LookAndFeel::getAlertWindowMessageFont()
{
    Layout::Component::ConfigFile config;
    using TextSize = Layout::Component::TextSize;
    return juce::Font(config.getFontHeight(TextSize::mediumText));
}


// Updates Component colours when they're changed in the ColourConfigFile.
void Theme::LookAndFeel::colourChanged(const int colourId,
        const juce::Identifier& colourKey, const juce::Colour newColour)
{
    setColour(colourId, newColour);
}
