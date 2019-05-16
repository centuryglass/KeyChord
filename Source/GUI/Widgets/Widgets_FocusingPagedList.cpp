#include "Widgets_FocusingPagedList.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "FocusingPagedList::";
#endif

// Default number of list items per page:
static const constexpr unsigned int defaultItemsPerPage = 5;

// Default list padding fraction to place between list items:
static const constexpr float defaultPaddingFraction = 0.02;

// Default milliseconds to take when(un)focusing list content:
static const constexpr unsigned int focusDuration = 300;

// List item border width in pixels:
static const constexpr unsigned int borderWidth = 4;

// Constructs the list with default list item margins and items per page.
Widgets::FocusingPagedList::FocusingPagedList() : listButtonHandler(*this)
{
    setItemsPerPage(defaultItemsPerPage);
    setYPaddingFraction(defaultPaddingFraction);
}


// Gets the index of the selected list item.
int Widgets::FocusingPagedList::getSelectedIndex() const
{
    return selectedIndex;
}


// Sets the selected list index.
void Widgets::FocusingPagedList::setSelectedIndex(const int index)
{
    if ((index >= -1) && (index < (int) getListSize())
            && (index != selectedIndex))
    {
        selectedIndex = index;
        selectionChanged();
        refreshListContent(Layout::Transition::Type::toDestination,
                focusDuration);
        updateNavButtonVisibility(index < 0);
    }
}


// Deselects the current selected list item, updating the component unless no
// list item was selected.
void Widgets::FocusingPagedList::deselect()
{
    setSelectedIndex(-1);
}


// Creates or updates a list component, selecting how to update the component
// based on whether it is selected.
juce::Component* Widgets::FocusingPagedList::updateListItem
(juce::Component* listItem, const unsigned int index)
{
    juce::Button* listButton = static_cast<juce::Button*>(listItem);

    if (selectedIndex == index)
    {
        listButton = updateSelectedListItem(listButton);
    }
    else
    {
        listButton = updateUnselectedListItem(listButton, index);
    }
    if (listItem == nullptr)
    {
        listButton->addListener(&listButtonHandler);
    }
    return static_cast<juce::Component*>(listButton);
}


// Gets the weight value used to determine the height of a particular list item.
unsigned int Widgets::FocusingPagedList::getListItemWeight
(const unsigned int index) const
{
    if (selectedIndex == index || selectedIndex < 0)
    {
        return 1;
    }
    return 0;
}


// Handles list item selection.
void Widgets::FocusingPagedList::ListButtonHandler::buttonClicked
(juce::Button* button)
{
    const int buttonIndex
            = list.getListItemIndex(static_cast<juce::Component*>(button));
    // The button must be a valid list item.
    jassert(buttonIndex >= 0);

    if (buttonIndex != list.getSelectedIndex())
    {
        list.setSelectedIndex(buttonIndex);
    }
    else
    {
        list.deselect();
    }
}
