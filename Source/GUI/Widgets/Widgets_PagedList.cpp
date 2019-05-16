#include "Widgets_PagedList.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Widgets::PagedList::";
#endif

// Default animation duration(milliseconds) when scrolling between list pages:
static const constexpr unsigned int defaultAnimDuration = 300;

// Initializes list navigation buttons when the list is constructed.
Widgets::PagedList::PagedList() :
upButton(NavButton::WindowEdge::up),
downButton(NavButton::WindowEdge::down),
navButtonListener(*this)
{
    addAndMakeVisible(upButton);
    addAndMakeVisible(downButton);
    upButton.addListener(&navButtonListener);
    downButton.addListener(&navButtonListener);
    setInterceptsMouseClicks(false, true);
}


// Changes the current selected list page, updating the list layout to show the
// selected page.
void Widgets::PagedList::setPageIndex
(const unsigned int newIndex, const bool animate, const unsigned int duration)
{
    using Transition = Layout::Transition::Type;
    if (newIndex < 0 || newIndex >= getPageCount())
    {
        DBG(dbgPrefix << __func__ << ": can't set page to invalid index "
                << (int) newIndex);
        return;
    }
    else if (newIndex == pageIndex)
    {
        DBG(dbgPrefix << __func__ << ": tried to move to page "
                << (int) newIndex << " but that's already the selected page.");
        return;
    }
    const Transition animationType = animate ?
        ((newIndex > pageIndex) ? Transition::moveUp : Transition::moveDown)
        : Transition::none;
    const unsigned int animationDuration = animate ? duration : 0;
    pageIndex = newIndex;
    refreshListContent(animationType, animationDuration);
    pageSelectionChanged();
}


// Provides the weight value used to set each list item's relative height.
unsigned int Widgets::PagedList::getListItemWeight
(const unsigned int index) const
{
    return 1;
}


// Finds the index of a list item Component within the list.
int Widgets::PagedList::getListItemIndex(juce::Component* listItem) const
{
    int itemIndex = listComponents.indexOf(listItem);
    if (itemIndex >= 0)
    {
        itemIndex += itemsPerPage * pageIndex;
        if (itemIndex >= getListSize())
        {
            itemIndex = -1;
        }
    }
    return itemIndex;
}


// Sets the number of list items that are displayed at one time.
void Widgets::PagedList::setItemsPerPage(const unsigned int perPage)
{
    if (perPage != itemsPerPage)
    {
        itemsPerPage = perPage;
        if (getParentComponent() != nullptr && !getBounds().isEmpty())
        {
            refreshListContent();
        }
    }
}


// Gets the number of list items shown on the page at one time.
unsigned int Widgets::PagedList::getItemsPerPage() const
{
    return itemsPerPage;
}


// Gets the number of pages in the list.
unsigned int Widgets::PagedList::getPageCount() const
{
    const unsigned int listSize = getListSize();
    unsigned int pageCount = listSize / itemsPerPage;
    if ((listSize % itemsPerPage) > 0)
    {
        pageCount++;
    }
    return pageCount;
}


// Sets the fraction of the list height that should be placed between list
// items.
void Widgets::PagedList::setYPaddingFraction(const float paddingFraction)
{
    yPaddingFraction = paddingFraction;
    if (getParentComponent() != nullptr && !getBounds().isEmpty())
    {
        refreshListContent();
    }
}


// Reloads list content, running updateListItem for each visible list item.
void Widgets::PagedList::refreshListContent
(const Layout::Transition::Type transition, const unsigned int duration,
        const bool animateUnmoved)
{
    unsigned int listSize = getListSize();
    pageIndex = std::min(pageIndex, getPageCount() - 1);
    int componentsSaved = listComponents.size();

    // Update the list layout if necessary:
    using namespace Layout::Group;
    RelativeLayout layout(layoutManager.getLayout());
    bool changesFound = false;

    for (int i = 0; i < itemsPerPage; i++)
    {
        int itemIndex = i + pageIndex * itemsPerPage;
        const int rowWeight = getListItemWeight(itemIndex);
        juce::Component* rowComponent = nullptr;
        if (itemIndex < listSize)
        {
            if (componentsSaved < i || listComponents[i] == nullptr)
            {
                listComponents.set(i, updateListItem(nullptr, itemIndex));
            }
            rowComponent = listComponents[i];
        }
        if (itemIndex < layout.rowCount())
        {
            const Layout::Group::Row& oldRow = layout.getRow(i);
            if (oldRow.getRowItem(0).getComponent() == rowComponent
                    && oldRow.getWeight() == rowWeight)
            {
                continue;
            }
        }
        layout.setRow(i, Row(rowWeight, { RowItem(rowComponent) }));
        changesFound = true;
    }

    if (changesFound || (animateUnmoved
                && (transition != Layout::Transition::Type::none)
                && (duration > 0)))
    {
        layout.setYPaddingFraction(yPaddingFraction);
        layout.setYMarginFraction(std::max(
                NavButton::yMarginFractionNeeded(NavButton::WindowEdge::up),
                NavButton::yMarginFractionNeeded(NavButton::WindowEdge::down)));
        layoutManager.transitionLayout(layout, this, transition, duration,
                animateUnmoved);
    }
    // Update individual list components:
    for (int i = 0; i < itemsPerPage; i++)
    {
        int itemIndex = i + pageIndex * itemsPerPage;
        if (itemIndex >= listSize)
        {
            break;
        }
        updateListItem(listComponents[i], itemIndex);
    }
    updateNavButtonVisibility(showNavButtons);
}


// Updates the list page navigation button visibility.
void Widgets::PagedList::updateNavButtonVisibility(const bool buttonsVisible)
{
    showNavButtons = buttonsVisible;
    upButton.setVisible(showNavButtons && pageIndex > 0);
    downButton.setVisible(showNavButtons && pageIndex < (getPageCount() - 1));
}


// Repositions list items and navigation buttons when the list is resized.
void Widgets::PagedList::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
    upButton.applyConfigBounds();
    downButton.applyConfigBounds();
    upButton.setBounds(getLocalArea(getParentComponent(),
            upButton.getBounds()));
    downButton.setBounds(getLocalArea(getParentComponent(),
            downButton.getBounds()));
}


// Scrolls the list when the navigation buttons are clicked.
void Widgets::PagedList::NavButtonListener::buttonClicked
(juce::Button* button)
{
    int newIndex = pagedList.pageIndex;
    if (button == &pagedList.upButton)
    {
        newIndex--;
    }
    else if (button == &pagedList.downButton)
    {
        newIndex++;
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Unexpected button \""
                << button->getName() << "\" captured by PagedList!");
        return;
    }
    if (newIndex >= 0 && newIndex < pagedList.getPageCount())
    {
        pagedList.setPageIndex(newIndex, true, defaultAnimDuration);
    }
}
