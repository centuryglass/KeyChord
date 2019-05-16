#pragma once
/**
 * @file  Widget_ColourPicker.h
 *
 * @brief  Provides a UI component for selecting colours.
 */

#include "Layout_Group_Manager.h"
#include "Locale_TextUser.h"
#include "JuceHeader.h"

namespace Widgets { class ColourPicker; }

/**
 * @brief  A UI component that allows the user to select a colour value.
 */
class Widgets::ColourPicker : public juce::Component, public Locale::TextUser,
        private juce::Slider::Listener,
        private juce::TextEditor::Listener,
        private juce::Button::Listener
{
public:
    /**
     * @brief  Initializes the colour picker layout.
     *
     * @param numSavedColours  Sets the number of quick colour selection
     *                         buttons to show.
     *
     * @param initialColour    Sets the initial colour value to show in the
     *                         ColourPicker.
     */
    ColourPicker(const int numSavedColours = 5,
            const juce::Colour initialColour = juce::Colours::white);

    virtual ~ColourPicker() { }

    // JUCE ColourId values:
    enum ColourIds
    {
        // Light area of the checkerboard pattern drawn behind colour previews.
        checkerboardLight = 0x1900600,
        // Dark area of the checkerboard pattern drawn behind colour previews.
        checkerboardDark  = 0x1900601,
        // Outline colour for slider backgrounds and colour previews.
        outline           = 0x1900602,
        // Outline colour used on saved colour buttons when held down.
        focusedOutline    = 0x1900603
    };

    /**
     * @brief  Gets the current selected colour.
     *
     * @return  The current selected colour held by the ColourPicker.
     */
    juce::Colour getSelectedColour() const;

    /**
     * @brief  Sets a new selected colour value.
     *
     * @param colour       The new selected value.
     *
     * @param runCallback  Sets if the colour selection callback should be
     *                     called for this colour change.
     */
    void setSelectedColour(const juce::Colour colour,
            const bool runCallback = false);

    /**
     * @brief  Assigns a callback function to run when a colour is selected.
     *
     * @param callback  The function to run, passing in the new colour as a
     *                  parameter.
     */
    void setSelectionCallback
    (const std::function<void(juce::Colour)> callback);

private:
    /**
     * @brief  Applies the current selected colour to the sliders.
     */
    void setSlidersToColour();

    /**
     * @brief  Sets the text field to the current colour's 8 digit hex value.
     */
    void setEditorText();

    /**
     * @brief  If the current colour selection isn't one of the preset colour
     *         buttons, add it to the list and remove the oldest value from the
     *         list.
     */
    void updateColourButtons();

    /**
     * @brief  Updates the colour preview and text box when slider values
     *         change.
     */
    void sliderValueChanged(juce::Slider* slider) override;

    /**
     * @brief  Updates the sliders and colour preview after the text box value
     *         changes.
     */
    void textEditorFocusLost(juce::TextEditor& editor) override;

    /**
     * @brief  Updates the sliders and colour preview after the text box value
     *         changes.
     */
    void textEditorReturnKeyPressed(juce::TextEditor& editor) override;

    /**
     * @brief  Handles button click events within the ColourPicker component.
     *
     * @param button  The button that was clicked.
     *
     *  If a colour thumbnail is clicked, it is applied to the sliders, text
     * box, and preview component. If the apply button is clicked, the
     * selection colour is added to the preview thumbnails and the selection
     * callback runs.
     */
    void buttonClicked(juce::Button* button) override;

    /**
     * @brief  Reapplies the layout to all child components to match the
     *         updated component bounds.
     */
    void resized() override;

    /**
     * @brief  A square component that shows a colour value.
     */
    class ColourBox: public juce::Component
    {
    public:
        /**
         * @brief  Optionally sets the component's colour and border on
         *         construction.
         *
         * @param colour      The colour used to fill the component.
         *
         * @param drawBorder  Whether a border should be drawn around the
         *                    component.
         */
        ColourBox(const juce::Colour colour = juce::Colour(),
                const bool drawBorder = true);

        virtual ~ColourBox() { };

        /**
         * @brief  Gets the colour currently being drawn within the ColourBox.
         *
         * @return  The saved colour value.
         */
        juce::Colour getColour() const;

        /**
         * @brief  Changes the colour that will be drawn within the ColourBox.
         *
         * @param colour  The new colour value to show.
         */
        void setColour(const juce::Colour colour);

    private:
        /**
         * @brief  Draws the component with its current colour value.
         *
         * @param graphics  The JUCE graphics context used to draw the
         *                  component.
         */
        void paint(juce::Graphics& graphics) override;

        // Colour used to fill the component bounds:
        juce::Colour colour;

        // Whether a border should be drawn around the component:
        bool drawBorder;
    };

    /**
     * @brief  A ColourBox button, used for re-selecting previous selected
     *         colours.
     */
    class ColourButton : public juce::Button
    {
    public:
        /**
         * @brief  Optionally sets an initial colour value on construction.
         *
         * @param colour  The button's initial colour value.
         */
        ColourButton(const juce::Colour colour = juce::Colour());

        virtual ~ColourButton() { }

        /**
         * @brief  Gets the current colour value held by the button.
         *
         * @return  The saved colour value.
         */
        juce::Colour getColour() const;

        /**
         * @brief  Changes the colour value held by the button.
         *
         * @param colour  The new colour value to save.
         */
        void setColour(const juce::Colour colour);

    private:
        /**
         * @brief  Draws the button, changing the outline colour when the
         *         button is down.
         *
         * @param graphics           The JUCE graphics context used to draw the
         *                           button.
         *
         * @param isMouseOverButton  Whether the mouse cursor is currently over
         *                           the button.
         *
         * @param isButtonDown       Whether the button is currently being
         *                           clicked.
         */
        virtual void paintButton(juce::Graphics& graphics,
                bool isMouseOverButton, bool isButtonDown) override;

        /**
         * @brief  Updates the internal ColourBox to match changes to this
         *         button's bounds.
         */
        void resized() override;

        // Controls the button's appearance:
        ColourBox colourBox;
    };

    // Current colour selection:
    juce::Colour selectedColour;

    // Handles all child component layout:
    Layout::Group::Manager layoutManager;

    // Shows the current colour selection over a grid background:
    ColourBox colourPreview;

    // Holds the buttons used to re-select previously selected colours:
    juce::OwnedArray<ColourButton> colourButtons;
    // Stores how many colourButtons should be used:
    int numSavedColours = 0;

    // A callback action to run when the user selects a colour:
    std::function<void(juce::Colour)> selectionCallback;

    // Red, green, blue, and alpha colour component sliders:
    juce::Slider colourSliders [4];

    /**
     * @brief  Draws a gradient showing all slider values behind a colour
     *         slider.
     */
    class SliderBackground : public juce::Component
    {
    public:
        SliderBackground() { }

        virtual ~SliderBackground() { }

        /**
         * @brief  Sets the colour component used to draw this slider
         *         background.
         *
         * @param colourMask  The sliderColourMask of this component's slider.
         */
        void setColourComponent(const juce::uint32 colourMask);

        /**
         * @brief  Updates this component with the current selected colour
         *         value.
         *
         * @param colour  The colour selected by the ColourPicker holding this
         *                component.
         */
        void setColour(const juce::Colour colour);

    private:
        /**
         * @brief  Draws a gradient of all possible slider colour values.
         *
         *  Given the current selected colour, this shows what the selected
         * colour would become for each slider value.
         *
         * @param graphics  The JUCE graphics context used to draw the
         *                  component.
         */
        void paint(juce::Graphics& graphics) override;

        // Bitmask selecting the slider's colour component:
        juce::uint32 colourComponent = 0;
        // Darkest colour in the component's gradient:
        juce::Colour minColour;
        // Brightest colour in the component's gradient:
        juce::Colour maxColour;
    };

    // Backgrounds drawn behind all colour sliders:
    SliderBackground sliderBackgrounds [4];

    // Holds the selected colour value as an eight digit hex number string:
    juce::TextEditor colourField;

    // The button clicked to confirm the colour selection:
    juce::TextButton selectionButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColourPicker)
};
