#include "Widgets_ColourPicker.h"
#include "Layout_Component_ConfigFile.h"

// Stores previous colour choices shared between all ColourPickers:
static juce::Array<juce::Colour> savedColours;

// Bitmasks for selecting each slider's colour component. For each slider at
// index i, (colour & sliderColours[i]) selects that slider's colour component
// from the current colour.
static const juce::uint32 sliderColourMasks [4] =
{
    0x00ff0000,
    0x0000ff00,
    0x000000ff,
    0xff000000
};

// Defines the thumb colour of each colour slider
static const juce::uint32 sliderThumbColours [4] =
{
    0xffff0000,
    0xff00ff00,
    0xff0000ff,
    0x77777777
};

// Number of pixels of space to leave between colour buttons:
static const constexpr int buttonMarginPx = 4;

// Localized object class key:
static const juce::Identifier localeClassKey = "Widgets::ColourPicker";

// Localized text item keys:
static const constexpr char * setColourTextKey = "setColour";

// Initializes the colour picker layout.
Widgets::ColourPicker::ColourPicker
(const int numSavedColours, const juce::Colour colour):
Locale::TextUser(localeClassKey),
numSavedColours(numSavedColours),
selectionCallback([](juce::Colour c){}),
selectedColour(colour)
{
    using namespace Layout::Group;
    using juce::Slider;
    using juce::Colour;
    RelativeLayout layout(
    {
        Row(50,
        {
            RowItem(&colourPreview, 40),
            RowItem(nullptr, 10) // Empty space to hold colour buttons
        }),
        Row(10, { RowItem(&colourSliders[0]) }),
        Row(10, { RowItem(&colourSliders[1]) }),
        Row(10, { RowItem(&colourSliders[2]) }),
        Row(10, { RowItem(&colourSliders[3]) }),
        Row(10,
        {
            RowItem(&colourField),
            RowItem(&selectionButton)
        })
    });
    colourPreview.setColour(colour);
    for (int i = 0; i < numSavedColours; i++)
    {
        ColourButton* cButton = new ColourButton(savedColours[i]);
        colourButtons.add(cButton);
        addAndMakeVisible(cButton);
        cButton->addListener(this);
    }

    for (int i = 0; i < 4; i++)
    {
        colourSliders[i].setSliderStyle(Slider::LinearHorizontal);
        colourSliders[i].setRange(0, 0xff, 1);
        colourSliders[i].addListener(this);
        colourSliders[i].setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        colourSliders[i].setColour(Slider::thumbColourId,
                Colour(sliderThumbColours[i]));
        sliderBackgrounds[i].setColourComponent(sliderColourMasks[i]);
        addAndMakeVisible(sliderBackgrounds[i]);
        sliderBackgrounds[i].toBack();
    }

    Layout::Component::ConfigFile config;
    colourField.setFont(juce::Font(config.getFontHeight(
            Layout::Component::TextSize::smallText)));
    colourField.setJustification(juce::Justification::centred);
    colourField.addListener(this);

    selectionButton.setButtonText(localeText(setColourTextKey));
    selectionButton.addListener(this);

    layoutManager.setLayout(layout, this);
    setSlidersToColour();
    setEditorText();
}


// Gets the current selected colour.
juce::Colour Widgets::ColourPicker::getSelectedColour() const
{
    return selectedColour;
}


// Sets a new selected colour value.
void Widgets::ColourPicker::setSelectedColour
(const juce::Colour colour, const bool runCallback)
{
    selectedColour = colour;
    setSlidersToColour();
    setEditorText();
    if (runCallback)
    {
        updateColourButtons();
        selectionCallback(colour);
    }
}


// Assigns a callback function to run when a colour is selected.
void Widgets::ColourPicker::setSelectionCallback
(const std::function<void(juce::Colour)> callback)
{
    selectionCallback = callback;
}


// Applies the current colour selection to the sliders.
void Widgets::ColourPicker::setSlidersToColour()
{
    for (int i = 0; i < 4; i++)
    {
        sliderBackgrounds[i].setColour(selectedColour);
        juce::uint32 colourValue = selectedColour.getARGB()
                & sliderColourMasks[i];
        while (colourValue > 0xff)
        {
            colourValue >>= 8;
        }
        colourSliders[i].setValue(colourValue);
    }
}


// Sets the text field to the current colour's 8 digit hex value.
void Widgets::ColourPicker::setEditorText()
{
    juce::String colourStr = selectedColour.toString();
    while (colourStr.length() < 8)
    {
        colourStr = '0' + colourStr;
    }
    colourField.setText("0x"+colourStr);
}


// If the current colour selection isn't one of the preset colour buttons,
// add it to the list and remove the oldest value from the list.
void Widgets::ColourPicker::updateColourButtons()
{
    if (!savedColours.contains(selectedColour))
    {
        savedColours.insert(0, selectedColour);
        savedColours.resize(numSavedColours);
        for (int i = 0; i < numSavedColours; i++)
        {
            colourButtons[i]->setColour(savedColours[i]);
        }
    }
}


// Updates the colour preview and text box when slider values change.
void Widgets::ColourPicker::sliderValueChanged(juce::Slider* slider)
{
    using juce::uint32;
    using juce::Colour;
    uint32 colourValue = 0;
    for (int i = 0; i < 4; i++)
    {
        colourValue += ((uint32) colourSliders[i].getValue()
                 * (sliderColourMasks[i] & 0x01010101));
    }
    selectedColour = Colour(colourValue);
    for (int i = 0; i < 4; i++)
    {
        sliderBackgrounds[i].setColour(selectedColour);
    }
    setEditorText();
    colourPreview.setColour(selectedColour);
}


// Updates the sliders and colour preview after the text box value changes.
void Widgets::ColourPicker::textEditorFocusLost(juce::TextEditor& editor)
{
    selectedColour = juce::Colour(editor.getText().getHexValue32());
    setEditorText();
    setSlidersToColour();
    colourPreview.setColour(selectedColour);
}


// Updates the sliders and colour preview after the text box value changes.
void Widgets::ColourPicker::textEditorReturnKeyPressed
(juce::TextEditor& editor)
{
    selectedColour = juce::Colour(editor.getText().getHexValue32());
    setEditorText();
    setSlidersToColour();
    colourPreview.setColour(selectedColour);
}


// Handles button click events within the ColourPicker component.
void Widgets::ColourPicker::buttonClicked(juce::Button* button)
{
    if (button == &selectionButton)
    {
        updateColourButtons();
        selectionCallback(selectedColour);
        return;
    }
    for (ColourButton* cButton : colourButtons)
    {
        if (cButton == button)
        {
            setSelectedColour(cButton->getColour());
            break;
        }
    }
}


// Reapplies the layout to all child components to match the updated component
// bounds.
void Widgets::ColourPicker::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
    for (int i = 0; i < 4; i++)
    {
        sliderBackgrounds[i].setBounds(colourSliders[i].getBounds());
    }
    if (numSavedColours > 0)
    {
        int btnHeight = (colourPreview.getHeight() / numSavedColours)
                    - buttonMarginPx;
        int btnWidth = getWidth() - colourPreview.getWidth()
            - 2 * buttonMarginPx;
        int left = colourPreview.getRight() + buttonMarginPx;
        int top = colourPreview.getY() + buttonMarginPx;
        for (ColourButton* btn : colourButtons)
        {
            btn->setBounds(left, top, btnWidth, btnHeight);
            top += btnHeight + buttonMarginPx;
        }
    }
}


// Optionally sets the component's colour and border on construction.
Widgets::ColourPicker::ColourBox::ColourBox
(const juce::Colour colour, const bool drawBorder) :
colour(colour),
drawBorder(drawBorder)
{
    setInterceptsMouseClicks(false, false);
}


// Gets the colour currently being drawn within the ColourBox.
juce::Colour Widgets::ColourPicker::ColourBox::getColour() const
{
    return colour;
}


// Changes the colour that will be drawn within the ColourBox.
void Widgets::ColourPicker::ColourBox::setColour(const juce::Colour colour)
{
    this->colour = colour;
    repaint();
}


// Draws the component with its current colour value.
void Widgets::ColourPicker::ColourBox::paint(juce::Graphics& graphics)
{
    juce::Rectangle<int> bounds = getLocalBounds();
    if (!colour.isOpaque())
    {
        float checkSize = bounds.getHeight() / 8;
        graphics.fillCheckerBoard(bounds.toFloat(), checkSize, checkSize,
                findColour(ColourPicker::checkerboardLight),
                findColour(ColourPicker::checkerboardDark));
    }
    graphics.setColour(colour);
    graphics.fillRect(bounds);
    if (drawBorder)
    {
        graphics.setColour(findColour(ColourPicker::outline));
        graphics.drawRect(bounds, 1 + getWidth() / 35);
    }
}


// Optionally sets an initial colour value on construction.
Widgets::ColourPicker::ColourButton::ColourButton(juce::Colour colour) :
colourBox(colour, true),
Button("colourButton")
{
    addAndMakeVisible(colourBox);
}


// Gets the current colour value held by the button.
juce::Colour Widgets::ColourPicker::ColourButton::getColour() const
{
    return colourBox.getColour();
}


// Changes the colour value held by the button.
void Widgets::ColourPicker::ColourButton::setColour(const juce::Colour colour)
{
    colourBox.setColour(colour);
}


// Draws the button, changing the outline colour when the button is down.
void Widgets::ColourPicker::ColourButton::paintButton
(juce::Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    g.setColour(findColour(isButtonDown ?
            Widgets::ColourPicker::outline
            : Widgets::ColourPicker::focusedOutline));
    g.drawRect(getLocalBounds(), getWidth()/35 + 2);
}


// Updates the internal ColourBox to match changes to this button's bounds.
void Widgets::ColourPicker::ColourButton::resized()
{
    colourBox.setBounds(getLocalBounds());
}


// Sets the colour component used to draw this slider background.
void Widgets::ColourPicker::SliderBackground::setColourComponent
(const juce::uint32 colourMask)
{
    colourComponent = colourMask;
}


// Updates this component with the current selected colour value.
void Widgets::ColourPicker::SliderBackground::setColour
(const juce::Colour colour)
{
    using juce::Colour;
    juce::uint32 colourARGB = colour.getARGB();
    minColour = Colour(colourARGB & (~colourComponent));
    maxColour = Colour(colourARGB | colourComponent);
    repaint();
}


// Draws a gradient of all possible slider colour values.
void Widgets::ColourPicker::SliderBackground::paint(juce::Graphics& g)
{
    float checkSize = getHeight() / 8;
    g.fillCheckerBoard(getLocalBounds().toFloat(), checkSize, checkSize,
            findColour(ColourPicker::ColourIds::checkerboardLight),
            findColour(ColourPicker::ColourIds::checkerboardDark));
    g.setGradientFill(juce::ColourGradient::horizontal(minColour, 0,
            maxColour, getWidth()));
    g.fillRect(getLocalBounds());
    g.setColour(findColour(ColourPicker::ColourIds::outline));
    g.drawRect(getLocalBounds(), checkSize);
}
