#pragma once
/**
 * @file  Theme_Colour_ConfigPage.h
 *
 * @brief  Provides user controls for editing configurable UI colours.
 */

#include "Theme_Colour_ConfigListener.h"
#include "Widgets_ColourPicker.h"
#include "Page_Component.h"

namespace Theme { namespace Colour { class ConfigPage; } }

/**
 * @brief  A Page class that provides control components for editing
 *         configurable colour values.
 */
class Theme::Colour::ConfigPage : public Page::Component
{
public:
    /**
     * @brief  Initializes the page layout on construction.
     */
    ConfigPage();

    virtual ~ConfigPage() { }

private:
    /**
     * @brief  Updates the colour list whenever the page is resized.
     */
    void pageResized() override;

    /**
     * @brief  Sets the contents and properties of the page's list of
     *         configurable colour values.
     */
    class ColourListModel : public juce::ListBoxModel,
            public Theme::Colour::ConfigListener
    {
    public:
        /**
         * @brief  Connects the list model to the colourPicker component it will
         *         control.
         *
         * @param colourPicker  The widget that allows the user to select new
         *                      colour values.
         */
        ColourListModel(Widgets::ColourPicker& colourPicker);

        virtual ~ColourListModel() { }

        /**
         * @brief  Gets the number of rows in the colour list.
         *
         * @return  The number of configurable colour keys defined in
         *          Theme::Colour::JSONKeys.
         */
        int getNumRows() override;

        /**
         * @brief  Gets the text of a specific row item.
         *
         * @param index  The index of the row to access.
         *
         * @return       The text printed at the given row item index, or the
         *               empty string if the index is invalid.
         */
        juce::String getRowText(int index) const;

        /**
         * @brief  Updates the list's layout when its bounds are updated.
         *
         * @param list  The Component object that displays the list model's
         *              list.
         */
        void listResized(juce::ListBox& list);

        /**
         * @brief  Draws a single list item within the list component.
         *
         * @param rowNumber      The index of the row item to draw.
         *
         * @param g              The JUCE graphics context to use for component
         *                       drawing operations.
         *
         * @param width          The width in pixels of the list item.
         *
         * @param height         The height in pixels of the list item.
         *
         * @param rowIsSelected  Whether the row being drawn is the selected row
         *                       in the list.
         */
        void paintListBoxItem(
                int rowNumber,
                juce::Graphics& g,
                int width,
                int height,
                bool rowIsSelected) override;

        /**
         * @brief  Updates a list item component to fit a new row.
         *
         *  Currently, this just deletes non-null Component objects and returns
         * nullptr, preventing the list from reusing list components. Support
         * for reusing list components should be added whenever the Colour
         * configuration page class next gets updated.
         *
         * @param rowNumber      The index of the row item to update.
         *
         * @param isRowSelected  Whether the row being drawn is the selected row
         *                       in the list.
         *
         * @param toUpdate       An old list item component that will be
         *                       deleted.
         *
         * @return               A null value, to signal that the component has
         *                       been deleted and can't be reused.
         */
        juce::Component* refreshComponentForRow(int rowNumber,
                bool isRowSelected, juce::Component* toUpdate) override;

        void selectedRowsChanged(int lastRowSelected) override { }


    private:
        /**
         * @brief  Listens for updates to generic UI element categories.
         *
         * @param key  The key of an updated UI element category.
         */
        virtual void configValueChanged(const juce::Identifier& key) override;

        /**
         * @brief  When a list item is clicked, copy its colour to the preview
         *         component if the list item's colour is not completely
         *         transparent.
         *
         * @param row         The clicked row item.
         *
         * @param mouseEvent  Additional click event data.
         */
        virtual void listBoxItemClicked(int row,
                const juce::MouseEvent& mouseEvent) override;

        /**
         * @brief  Listens for updates to all tracked JUCE ColourId values.
         *
         * @param colourId   The updated ColourId value.
         *
         * @param colourKey  The key to either the updated colour, or to its
         *                   generic category colour if its specific colour
         *                   isn't set.
         *
         * @param newColour  The new colour value.
         */
        virtual void colourChanged(const int colourId,
                const juce::Identifier& colourKey,
                const juce::Colour newColour) override;

        // Height used when drawing list item names:
        int textHeight;

        // List of all configurable colour value keys:
        const juce::Array<const juce::Identifier*> colourKeys;

        // List of selected colours for each colour key:
        juce::Array<juce::Colour> colours;

        // Widget used to select new colour values:
        Widgets::ColourPicker& colourPicker;
    };

    // Displays the list of configurable colour values.
    juce::ListBox colourList;
    // Manages the contents of the colourList.
    ColourListModel listModel;
    // Selects new colour values for colour list items.
    Widgets::ColourPicker colourPicker;
};
