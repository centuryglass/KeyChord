#pragma once
/**
 * @file  Widgets_FocusingPagedList.h
 *
 * @brief  Shows a scrolling list of items. List items can be selected, causing
 *         the list to focus on them and potentially display additional content
 *         relevant to the selected item.
 */

#include "Widgets_PagedList.h"
#include "Layout_Group_Manager.h"

namespace Widgets { class FocusingPagedList; }

/**
 * @brief  An abstract PagedList component that allows its list items to be
 *         selected, expanding the selected item to fill the component.
 *
 *  When a list item is clicked, it expands to fill the page, hiding all other
 * list items and the PagedList's navigation buttons. Implementations of
 * FocusingPagedList are responsible for providing and updating list item
 * components.
 */
class Widgets::FocusingPagedList : public PagedList
{
public:
    /**
     * @brief  Constructs the list with default list item margins and items per
     *         page.
     */
    FocusingPagedList();

    virtual ~FocusingPagedList() { }

    /**
     * @brief  Gets the index of the selected list item.
     *
     * @return  The selected index, or -1 if no list item is selected.
     */
    int getSelectedIndex() const;

    /**
     * @brief  Sets the selected list index.
     *
     * @param index  The index of the list item to select. If this value is
     *               outside of the list bounds, nothing will happen.
     */
    void setSelectedIndex(const int index);

    /**
     * @brief  Deselects the current selected list item, updating the component
     *         unless no list item was selected.
     */
    void deselect();

    /**
     * @brief  Creates or updates a component so it can be used as a specific
     *         unselected list item.
     *
     * @param listItem  A list item Button to update. This parameter may be
     *                  null, in which case a new Component should be created.
     *
     * @param index     The index where the list item will be used. Subclasses
     *                  should use this to select the appropriate properties or
     *                  content to apply to the Component.
     *
     * @return          The updated list Component.
     */
    virtual juce::Button* updateUnselectedListItem(juce::Button* listItem,
            const unsigned int index) = 0;

    /**
     * @brief  Creates or updates a component so it can be used as the selected
     *         list item.
     *
     * @param listItem  An list item Button to update. This parameter may be
     *                  null, in which case a new Component should be created.
     *
     * @return          The updated selected list Component.
     */
    virtual juce::Button* updateSelectedListItem(juce::Button* listItem) = 0;

private:
    /**
     * @brief  Runs any actions that should occur when the selected list item
     *         changes before the list's layout is updated.
     *
     *  By default, this takes no action. Subclasses should override this
     * function if they need to act when the selected list item changes.
     */
    virtual void selectionChanged() { }

    /**
     * @brief  Creates or updates a list component, selecting how to update the
     *         component based on whether it is selected.
     *
     * @param listItem  A list item Component to update. This parameter may be
     *                  null, in which case a new Component should be created.
     *
     * @param index     The index where the list item will be used. Subclasses
     *                  should use this to select the appropriate properties or
     *                  content to apply to the Component.
     *
     * @return          The updated list Component.
     */
    virtual juce::Component* updateListItem(juce::Component* listItem,
            const unsigned int index) final override;

    /**
     * @brief  Gets the weight value used to determine the height of a
     *         particular list item.
     *
     *  When no list item is selected, all row heights are equal. When a list
     * item is selected, the selected item uses the entire list height.
     *
     * @param index  The index of a list item, or of an empty space after the
     *               last list item.
     *
     * @return       Zero if a row item is selected and it is not the row item
     *               at the given index, one otherwise.
     */
    virtual unsigned int getListItemWeight(const unsigned int index)
            const override;

    /**
     * @brief  Selects or deselects list items when they are clicked.
     */
    class ListButtonHandler : public juce::Button::Listener
    {
    public:
        /**
         * @brief  Connects the button handler to its list.
         *
         * @param list  The FocusingPagedList that owns this object.
         */
        ListButtonHandler(FocusingPagedList& list) : list(list) { }

    private:
        /**
         * @brief  Toggles whether the clicked list item is currently selected.
         *
         * @param button  A button list item within this listener's list.
         */
        virtual void buttonClicked(juce::Button* button) override;

        FocusingPagedList& list;
    };
    ListButtonHandler listButtonHandler;

    // Current selected list index:
    int selectedIndex = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FocusingPagedList)
};
