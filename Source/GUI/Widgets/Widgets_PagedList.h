#pragma once
/**
 * @file  Widgets_PagedList.h
 *
 * @brief  An abstract interface for classes arranging components in a vertical
 *         scrolling list.
 */

#include "Widgets_NavButton.h"
#include "Layout_Group_Manager.h"

namespace Widgets { class PagedList; }

/**
 * @brief  Evenly spaces juce::Component objects in a vertical list, using up
 *         and down NavButtons to scroll the list one page at a time.
 *
 *  PagedList takes ownership of all Components in the list. When scrolling
 * through the list, existing Component list items are reused and updated.
 */
class Widgets::PagedList : public juce::Component
{
public:
    /**
     * @brief  Initializes list navigation buttons when the list is constructed.
     */
    PagedList();

    virtual ~PagedList() { }

    /**
     * @brief  Returns the number of items in the list.
     *
     * @return  The number of Component objects in the list.
     */
    virtual unsigned int getListSize() const = 0;

    /**
     * @brief  Changes the current selected list page, updating the list layout
     *         to show the selected page.
     *
     * @param newIndex  The new page index to show. No action will be taken if
     *                  this is not a valid page index.
     *
     * @param animate   Whether any list item transition should be animated.
     *
     * @param duration  Duration in milliseconds to run transition animations.
     */
    void setPageIndex(const unsigned int newIndex, const bool animate,
            const unsigned int duration);

protected:
    /**
     * @brief  Creates or updates a component to be used as a specific list
     *         item.
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
            const unsigned int index) = 0;

    /**
     * @brief  Provides the weight value used to set each list item's relative
     *         height.
     *
     *  All list items have vertical weight 1 by default. Subclasses should
     * override this function if they need to have rows of varying heights.
     *
     * @param index  A list index. This may be an index greater than the list
     *               bounds, if determining the weight of an empty list row
     *               after the last list item.
     *
     * @return       The weight value that should be used to determine the
     *               relative height of this row index.
     */
    virtual unsigned int getListItemWeight(const unsigned int index) const;

    /**
     * @brief  Finds the index of a list item Component within the list.
     *
     * @param listItem  A component that may be an item in the list.
     *
     * @return          The component's index in the list, or -1 if the
     *                  component is not a visible list item component.
     */
    int getListItemIndex(juce::Component* listItem) const;

    /**
     * @brief  Sets the number of list items that are displayed at one time.
     *
     * @param perPage  The number of Components to display per list page.
     */
    void setItemsPerPage(const unsigned int perPage);

    /**
     * @brief  Gets the number of list items shown on the page at one time.
     *
     * @return  The number of Components to display per list page.
     */
    unsigned int getItemsPerPage() const;

    /**
     * @brief  Gets the number of pages in the list.
     *
     * @return  The number of pages that can be displayed with the current list
     *          size and items per page.
     */
    unsigned int getPageCount() const;

    /**
     * @brief  Sets the fraction of the list height that should be placed
     *         between list items.
     *
     * @param marginFraction  The fraction of the list height places between
     *                        each pair of list items.
     */
    void setYPaddingFraction(const float marginFraction);

    /**
     * @brief  Reloads list content, running updateListItem for each visible
     *         list item.
     *
     * @param transition      Optional transition animation to apply when
     *                        updating list content.
     *
     * @param duration        Duration in milliseconds to run transition
     *                        animations.
     *
     * @param animateUnmoved  Whether transition animations should be applied
     *                        to list items when their bounds are not updated.
     *                        By default, transition animations are applied to
     *                        all list items.
     */
    void refreshListContent(
            const Layout::Transition::Type transition
                    = Layout::Transition::Type::none,
            const unsigned int duration = 0,
            const bool animateUnmoved = true);

    /**
     * @brief  Updates the list page navigation button visibility.
     *
     *  The up navigation button will always be hidden on the first page, and
     * the down navigation button will always be hidden on the last page.
     *
     * @param buttonsVisible  If true, navigation buttons will be shown when
     *                        relevant. If false, navigation buttons will
     *                        always be hidden.
     */
    void updateNavButtonVisibility(const bool buttonsVisible);

private:
    /**
     * @brief  Performs some action whenever the selected page index changes.
     *
     *  By default, this takes no action. PagedList subclasses should override
     * this function if they need to do something whenever the page selection
     * changes.
     */
    virtual void pageSelectionChanged() { }

    /**
     * @brief  Repositions list items and navigation buttons when the list is
     *         resized.
     */
    virtual void resized() override;

    // Navigation buttons used to scroll through the list:
    NavButton upButton;
    NavButton downButton;

    /**
     * @brief  Handles navigation button events.
     */
    class NavButtonListener : public juce::Button::Listener
    {
    public:
        /**
         * @brief  Connects the listener to the PagedList that owns it.
         *
         * @param pagedList  The PagedList object holding this listener.
         */
        NavButtonListener(PagedList& pagedList) : pagedList(pagedList) { }

        virtual ~NavButtonListener() { }

    private:
        /**
         * @brief  Scrolls the list when the navigation buttons are clicked.
         *
         * @param button  One of the two page navigation buttons.
         */
        virtual void buttonClicked(juce::Button* button) final override;

        PagedList& pagedList;
    };
    NavButtonListener navButtonListener;

    // Handles the list item layout:
    Layout::Group::Manager layoutManager;

    // Tracks if navigation buttons should be shown:
    bool showNavButtons = true;

    // All Components displayed in the list:
    juce::OwnedArray<juce::Component> listComponents;

    // The number of list items to display per page:
    unsigned int itemsPerPage = 5;

    // The index of the current list page:
    unsigned int pageIndex = 0;

    // The fraction of the list height to place between each list item pair:
    float yPaddingFraction = 0.05;
};
