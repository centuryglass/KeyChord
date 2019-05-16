#pragma once
/**
 * @file  Widgets_ListEditor.h
 *
 * @brief  Provides a UI component used for editing a list of strings.
 */

#include "Widgets_DrawableImageButton.h"
#include "Layout_Group_Manager.h"
#include "Theme_Image_Component.h"
#include "JuceHeader.h"

namespace Widgets { class ListEditor; }

/**
 * @brief  A UI component used for editing a list of strings.
 *
 *  The user can add new strings, edit existing ones by double clicking them,
 * and delete list items. The edited list of strings can then be obtained with
 * getListItems().
 */
class Widgets::ListEditor : public juce::Component,
        private juce::Button::Listener,
        private juce::Label::Listener,
        private juce::ListBoxModel
{
public:
    /**
     * @brief  Sets the initial list values to edit on construction.
     *
     * @param initialList  The initial list contents.
     */
    ListEditor(const juce::StringArray initialList);

    virtual ~ListEditor() { }

    /**
     * @brief  Custom JUCE ColourId values.
     */
    enum ColourIds
    {
        backgroundColourId       = 0x1900100,
        listItemColourId         = 0x1900101,
        selectedListItemColourId = 0x1900102,
        textColourId             = 0x1900103
    };

    /**
     * @brief  Gets the number of rows to use within the editor's list
     *         component.
     *
     * @return  The number of items in the list.
     */
    virtual int getNumRows() override;

    /**
     * @brief  Gets the edited string list.
     *
     * @return  All editor list strings.
     */
    juce::StringArray getListItems() const;

    /**
     * @brief  Sets the contents of the edited list.
     *
     * @param newItems  The new list of strings to edit.
     */
    void setListItems(const juce::StringArray newItems);

private:
    /**
     * @brief  Calls updateColours whenever component colour values are changed.
     */
    void colourChanged() override;

    /**
     * @brief  Sets the colours of child components to match ListEditor colours.
     */
    void updateColours();

    /**
     * @brief  Receives notifications when ListItem text is changed.
     *
     * These changes will be copied back to the appropriate string in the list.
     *
     * @param source  The label component holding an updated list value.
     */
    virtual void labelTextChanged(juce::Label* source) override;

    /**
     * @brief  Handles click events for all list items.
     *
     *  Clicking a list box item selects that item's row.
     *
     * @param row         The clicked list item index.
     *
     * @param mouseEvent  Holds more information about the registered event.
     */
    virtual void listBoxItemClicked
    (int row, const juce::MouseEvent& mouseEvent) override;

    /**
     * @brief  Handles double click events for all list items.
     *
     * Double clicking a list box item makes it editable.
     *
     * @param row         The clicked list item index.
     *
     * @param mouseEvent  Holds more information about the registered event.
     */
    virtual void listBoxItemDoubleClicked
    (int row, const juce::MouseEvent & mouseEvent) override;

    /**
     * @brief  Removes a row when the delete key is pressed.
     *
     * @param lastRowSelected  The last selected row, which shall be deleted if
     *                         is still within the list bounds.
     */
    virtual void deleteKeyPressed(int lastRowSelected) override;

    /**
     * @brief  Removes a row from the list component along with the
     *         corresponding index in the string list.
     *
     * @param rowNumber  The list row to remove.
     */
    void removeRow(const int rowNumber);

    /**
     * @brief  An editable label with a delete button, used to represent and
     *         edit a single item in the list.
     */
    class ListItemComponent : public juce::Label
    {
    public:
        /**
         * @brief  Sets the list item's text and connects it to its ListEditor
         *         on construction.
         *
         * @param text   The initial list item text.
         *
         * @param owner  The ListEditor containing this list item.
         */
        ListItemComponent(const juce::String text, ListEditor * owner);

        virtual ~ListItemComponent() { }

        /**
         * @brief  Updates a delete button's component ID to mark its list
         *         index.
         *
         * @param id  An ID value marking the component's current place in the
         *            list.
         */
        void setDeleteButtonID(const juce::String id);

        /**
         * @brief  Update the font and delete button to match when the list
         *         item's bounds change.
         */
        void resized() override;

    private:
        // The list item's delete button:
        Theme::Image::Component<DrawableImageButton> deleteButton;
    };

    /**
     * @brief  Creates or recycles a list component to fit a list row.
     *
     * @param rowNumber      The list index that the component should represent.
     *
     * @param isRowSelected  Whether the list row is selected.
     *
     * @param toUpdate       Either an old list component to update, or nullptr
     *                       if a new component should be created.
     *
     * @return               The created or updated list component.
     */
    virtual juce::Component* refreshComponentForRow(
            int rowNumber,
            bool isRowSelected,
            juce::Component* toUpdate) override;

    /**
     * @brief  Adds the contents of the text editor as a new list item when the
     *         addItem button is clicked, or removes the associated list item
     *         if a delete button is clicked.
     *
     * @param buttonClicked  The button component that was clicked.
     */
    virtual void buttonClicked(juce::Button* buttonClicked) override;

    /**
     * @brief  A required list item painting function, implemented as an empty
     *         function as ListItemComponent will paint itself already.
     */
    virtual void paintListBoxItem(int rowNumber, juce::Graphics&g, int width,
            int height, bool rowIsSelected) override { }

    /**
     * @brief  Reapplies the layout and adjusts list item height to fit the new
     *         bounds.
     */
    virtual void resized() override;

    // Holds all list strings:
    juce::StringArray listItems;

    // Handles the layout of the list and the editor components:
    Layout::Group::Manager layoutManager;

    // Displays all list items:
    juce::ListBox listContainer;

    // Text field for entering a new list item:
    juce::TextEditor newItemField;

    // Adds the contents of newItemField as a new list item when clicked.
    juce::TextButton addItemButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ListEditor)
};
