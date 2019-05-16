#include "Widgets_ListEditor.h"
#include "Layout_Component_ConfigFile.h"
#include "Theme_Image_JSONKeys.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Widgets::ListEditor::";
#endif

// Text to display in the addItemButton:
static const constexpr char* addItemButtonText = "+";

// List Layout Values:

// Row weights:
static const constexpr int listRowWeight    = 30;
static const constexpr int editorRowWeight  = 10;
static const constexpr int rowPaddingWeight = 1;

// Horizontal editor row item weights:
static const constexpr int textEditorWeight       = 40;
static const constexpr int addButtonWeight        = 10;
static const constexpr int editorRowPaddingWeight = 1;

// Layout margin fractions:
static const constexpr double xMarginFraction = 0.02;
static const constexpr double yMarginFraction = 0.02;

// List outline thickness, as a fraction of the list's smallest side:
static const constexpr double outlineThicknessFraction = 0.01;
// Minimum size of the list outline in pixels:
static const constexpr int outlineMinimumPx = 1;

// Sets the initial list values to edit on construction.
Widgets::ListEditor::ListEditor(const juce::StringArray initialList) :
listItems(initialList),
listContainer("Widgets::ListEditor", nullptr),
addItemButton(addItemButtonText)
{
#if JUCE_DEBUG
    setName("Widgets::ListEditor");
#endif
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(listRowWeight,  { RowItem(&listContainer) }),
        Row(editorRowWeight,
        {
            RowItem(&newItemField, textEditorWeight),
            RowItem(&addItemButton, addButtonWeight)
        })
    });
    layout.setYPaddingWeight(rowPaddingWeight);
    layout.setXPaddingWeight(editorRowPaddingWeight);
    layout.setXMarginFraction(xMarginFraction);
    layout.setYMarginFraction(yMarginFraction);
    layoutManager.setLayout(layout, this);

    Layout::Component::ConfigFile config;
    newItemField.setFont(juce::Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));

    updateColours();

    listContainer.setModel(this);
    listContainer.addMouseListener(this, true);

    addItemButton.addListener(this);
}


// Gets the number of rows to use within the editor's list component.
int Widgets::ListEditor::getNumRows()
{
    return listItems.size();
}


// Gets the edited string list.
juce::StringArray Widgets::ListEditor::getListItems() const
{
    return listItems;
}


// Sets the contents of the edited list.
void Widgets::ListEditor::setListItems(const juce::StringArray newItems)
{
    listItems = newItems;
    listContainer.updateContent();
    repaint();
}


// Calls updateColours whenever component colour values are changed.
void Widgets::ListEditor::colourChanged()
{
    updateColours();
}


// Sets the colours of child components to match ListEditor colours
void Widgets::ListEditor::updateColours()
{
    using juce::TextButton;
    using juce::ListBox;
    using juce::ScrollBar;
    addItemButton.setColour(TextButton::textColourOnId,
            findColour(textColourId));
    listContainer.setColour(ListBox::backgroundColourId,
            findColour(backgroundColourId));
    listContainer.setColour(ListBox::outlineColourId,
            findColour(selectedListItemColourId));

    ScrollBar& scrollbar = listContainer.getVerticalScrollBar();
    scrollbar.setColour(ScrollBar::trackColourId,
            findColour(listItemColourId));
    scrollbar.setColour(ScrollBar::thumbColourId, findColour(textColourId));
}


// Receives notifications when ListItemComponent text is changed.
void Widgets::ListEditor::labelTextChanged(juce::Label* source)
{
    ListItemComponent * listItem = dynamic_cast<ListItemComponent*>(source);
    if (listItem != nullptr)
    {
        int rowNum = listItem->getComponentID().getIntValue();
        listItems.getReference(rowNum) = listItem->getText(true);
        DBG(dbgPrefix << __func__ << ": Text set to "
                << listItem->getText(true));
    }
    listContainer.updateContent();
    listContainer.repaint();
}


// Handles click events for all list items.
void Widgets::ListEditor::listBoxItemClicked
(int row, const juce::MouseEvent& mouseEvent)
{
    listContainer.selectRow(row);
}


// Handles double click events for all list items.
void Widgets::ListEditor::listBoxItemDoubleClicked
(int row, const juce::MouseEvent & mouseEvent)
{
    using juce::Label;
    Label* rowClicked = static_cast<Label*>
            (listContainer.getComponentForRowNumber(row));
    rowClicked->showEditor();
    juce::TextEditor* editor = rowClicked->getCurrentTextEditor();
    if (editor != nullptr)
    {
        editor->setBounds(rowClicked->getBounds());
    }
}


// Removes a row when the delete key is pressed.
void Widgets::ListEditor::deleteKeyPressed(int lastRowSelected)
{
        removeRow(lastRowSelected);
}


// Removes a row from the list component along with the corresponding index in
// the string list.
void Widgets::ListEditor::removeRow(const int rowNumber)
{
    if (rowNumber >= 0 && rowNumber < listItems.size())
    {
        listItems.remove(rowNumber);
        listContainer.updateContent();
    }

}


// Sets the list item's text and connects it to its ListEditor on construction.
Widgets::ListEditor::ListItemComponent::ListItemComponent
(const juce::String text, ListEditor* owner) :
juce::Label(text),
deleteButton(Theme::Image::JSONKeys::cancelButton)
{
    setJustificationType(juce::Justification::left);
    addAndMakeVisible(deleteButton);
    deleteButton.addListener(owner);
    deleteButton.setWantsKeyboardFocus(false);
    setInterceptsMouseClicks(false, true);
}


// Updates a delete button's component ID to mark its list index.
void Widgets::ListEditor::ListItemComponent::setDeleteButtonID
(const juce::String id)
{
    deleteButton.setComponentID(id);
}


// List item font margins:
static const constexpr double listTextMarginFraction = 0.03;
static const constexpr int listTextMarginMinimum     = 1;

// Delete button margins:
static const constexpr double deleteButtonMarginFraction = 0.03;
static const constexpr int deleteButtonMarginMinimum     = 2;

// Update the font and delete button to match when the list item's bounds
// change.
void Widgets::ListEditor::ListItemComponent::resized()
{
    const int fontMargin = std::max(listTextMarginMinimum,
            int(listTextMarginFraction * getHeight()));
    setFont(getFont().withHeight(fontMargin));

    const int buttonMargin = std::max(deleteButtonMarginMinimum,
            int(deleteButtonMarginFraction * getHeight()));
    const int buttonSize = getHeight() - 2 * buttonMargin;

    deleteButton.setBounds(getRight() - buttonMargin - buttonSize,
            buttonMargin, buttonSize, buttonSize);
}


// Creates or recycles a list component to fit a list row.
juce::Component * Widgets::ListEditor::refreshComponentForRow
(int rowNumber, bool isRowSelected, juce::Component * existingComponent)
{
    using juce::String;
    using juce::Label;
    if (rowNumber >= getNumRows())
    {
        if (existingComponent != nullptr)
        {
            delete existingComponent;
        }
        return nullptr;
    }

    ListItemComponent * rowLabel = static_cast<ListItemComponent*>
            (existingComponent);
    if (rowLabel == nullptr)
    {
        rowLabel = new ListItemComponent(listItems[rowNumber], this);
        rowLabel->addListener(this);
    }
    rowLabel->setText(listItems[rowNumber],
            juce::NotificationType::dontSendNotification);
    rowLabel->setColour(Label::textColourId, findColour(textColourId));
    rowLabel->setDeleteButtonID(String(rowNumber));
    rowLabel->setComponentID(String(rowNumber));
    rowLabel->setColour(Label::backgroundColourId,
            findColour(isRowSelected ? selectedListItemColourId :
                listItemColourId));
    return rowLabel;
}


// Adds the contents of the text editor as a new list item when the addItem
// button is clicked, or removes the associated list item if a delete button is
// clicked.
void Widgets::ListEditor::buttonClicked(juce::Button* buttonClicked)
{
    if (buttonClicked == &addItemButton)
    {
        const juce::String newListItem = newItemField.getText();
        if (newListItem.isNotEmpty())
        {
            listItems.add(newListItem);
            newItemField.clear();
            listContainer.updateContent();
            listContainer.repaint();
        }
    }
    else // otherwise it's a delete button
    {
        const int buttonRow = buttonClicked->getComponentID().getIntValue();
        removeRow(buttonRow);
    }
}


// Adds the contents of the text editor as a new list item when the addItem
// button is clicked, or removes the associated list item if a delete button is
// clicked.
void Widgets::ListEditor::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
    listContainer.setRowHeight(newItemField.getHeight());
    const int smallestSide = std::min(listContainer.getWidth(),
            listContainer.getHeight());
    const int outlineThickness = std::max(outlineMinimumPx,
            int(smallestSide * outlineThicknessFraction));
    listContainer.setOutlineThickness(outlineThickness);
}
