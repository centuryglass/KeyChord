#include "Theme_Colour_ConfigPage.h"
#include "Theme_Colour_ConfigFile.h"
#include "Theme_Colour_JSONKeys.h"
#include "Layout_Component_ConfigFile.h"
#include "Windows_Info.h"

namespace ColourTheme = Theme::Colour;

// Initializes the page layout on construction.
ColourTheme::ConfigPage::ConfigPage() :
listModel(colourPicker),
colourList("colourList", &listModel)
{
    using namespace Layout::Group;
    RelativeLayout layout;
    if (Windows::Info::inPortraitMode())
    {
        layout = RelativeLayout(
        {
            Row(60, { RowItem(&colourPicker) }),
            Row(40, { RowItem(&colourList) })
        });
    }
    else
    {
        layout = RelativeLayout(
        {
            Row(40,
            {
                RowItem(&colourList, 60),
                RowItem(&colourPicker, 40)
            })
        });
    }
    layout.setYMarginFraction(0.1);
    layout.setYPaddingWeight(3);
    layout.setXPaddingWeight(3);
    setBackButton(BackButtonType::left);
    setLayout(layout);
    colourList.setOutlineThickness(2);
    colourPicker.setSelectionCallback([this](juce::Colour newColour)
    {
        juce::SparseSet<int> selected = colourList.getSelectedRows();
        if (!selected.isEmpty())
        {
            juce::Identifier selectedKey = listModel.getRowText(selected[0]);
            ConfigFile config;
            config.setColour(selectedKey, newColour);
        }
        repaint();
    });
}


// Updates the colour list whenever the page is resized.
void ColourTheme::ConfigPage::pageResized()
{
    listModel.listResized(colourList);
    colourList.updateContent();
    colourList.repaint();
}


// Connects the list model to the colourPicker component it will control.
ColourTheme::ConfigPage::ColourListModel::ColourListModel
(Widgets::ColourPicker& colourPicker) :
colourKeys(JSONKeys::getColourKeys()),
colourPicker(colourPicker)
{
    ConfigFile config;
    for (const juce::Identifier* key : colourKeys)
    {
        addTrackedKey(*key);
    }
    DBG("ColourTheme::ConfigPage::ColourListModel::" << __func__
            << ": adding " << colourKeys.size() << " colours");
    for (const juce::Identifier* key : colourKeys)
    {
        colours.add(config.getColour(*key));
    }
    DBG("ColourTheme::ConfigPage::ColourListModel::" << __func__
            << ": found " << colours.size() << " colours");
}


//  Gets the number of rows in the colour list.
int ColourTheme::ConfigPage::ColourListModel::getNumRows()
{
    return colourKeys.size();
}


// Gets the text of a specific row item.
juce::String ColourTheme::ConfigPage::ColourListModel::getRowText
(int index) const
{
    return colourKeys[index]->toString();
}


// Updates the list's layout when its bounds are updated.
void ColourTheme::ConfigPage::ColourListModel::listResized
(juce::ListBox& list)
{
    Layout::Component::ConfigFile layoutConfig;
    textHeight = layoutConfig.getFontHeight(
            Layout::Component::TextSize::smallText);
    list.setRowHeight(textHeight * 1.5);
    list.updateContent();
    list.repaint();
}


//  Updates a list item component to fit a new row.
void ColourTheme::ConfigPage::ColourListModel::paintListBoxItem(
        int rowNumber,
        juce::Graphics& graphics,
        int width,
        int height,
        bool rowIsSelected)
{
    ConfigFile config;
    juce::Rectangle<int> fillArea(0, 0, width, height);
    float checkSize = fillArea.getHeight() / 4;
    graphics.fillCheckerBoard(fillArea.toFloat(), checkSize, checkSize,
            config.getColour(Widgets::ColourPicker::checkerboardLight),
            config.getColour(Widgets::ColourPicker::checkerboardDark));
    graphics.setColour(colours[rowNumber]);
    graphics.fillRect(fillArea);
    fillArea.reduce(height / 5, height / 5);
    graphics.setColour((colours[rowNumber].getAlpha() < 0x88) ?
            config.getColour(Widgets::ColourPicker::checkerboardLight)
                    .contrasting()
            : colours[rowNumber].contrasting());
    if (rowIsSelected)
    {
        graphics.fillRoundedRectangle(fillArea.toFloat(), textHeight / 5);
        graphics.setColour((colours[rowNumber].getAlpha() < 0x88) ?
                config.getColour(Widgets::ColourPicker::checkerboardLight)
                : colours[rowNumber]);
    }
    graphics.setOpacity(1);
    graphics.setFont(juce::Font(textHeight));
    graphics.drawText(colourKeys[rowNumber]->toString(), fillArea,
            juce::Justification::centred);
}


// Updates a list item component to fit a new row.
juce::Component*
ColourTheme::ConfigPage::ColourListModel::refreshComponentForRow
(int rowNumber, bool isRowSelected, juce::Component* toUpdate)
{
    std::unique_ptr<juce::Component> oldComponent;
    oldComponent.reset(toUpdate);
    return nullptr;
}


// Listens for updates to generic UI element categories.
void ColourTheme::ConfigPage::ColourListModel::configValueChanged
(const juce::Identifier& key)
{
    ConfigFile config;
    int colourIndex = -1;
    for (int i = 0; i < colourKeys.size(); i++)
    {
        if (*colourKeys[i] == key)
        {
            colourIndex = i;
            break;
        }
    }
    if (colourIndex >= 0)
    {
        juce::Colour newColour = config.getColour(key);
        colours.set(colourIndex, newColour);
        DBG("ColourTheme::ConfigPage::ColourListModel::" << __func__
                << ": Setting colour " << key << " index " << colourIndex
                << " to " << newColour.toString());
    }
    else
    {
        DBG("ColourTheme::ConfigPage::ColourListModel::" << __func__
                << ": Unknown colour key " << key);
    }

}


// When a list item is clicked, copy its colour to the preview component if the
// list item's colour is not completely transparent.
void ColourTheme::ConfigPage::ColourListModel::listBoxItemClicked
(int row, const juce::MouseEvent& mouseEvent)
{
    juce::Colour rowColour = colours[row];
    if (rowColour.getAlpha() > 0)
    {
        colourPicker.setSelectedColour(rowColour, false);
    }
}


// Listens for updates to all tracked juce ColourId values.
void ColourTheme::ConfigPage::ColourListModel::colourChanged(
        const int colourId,
        const juce::Identifier& colourKey,
        const juce::Colour newColour)
{
    using namespace JSONKeys;
    ConfigFile config;
    const juce::Identifier& idKey = getColourKey(colourId);
    int colourIndex = colourKeys.indexOf(&idKey);
    if (colourIndex >= 0)
    {
        colours.set(colourIndex, newColour);
        DBG("ColourTheme::ConfigPage::ColourListModel::" << __func__
                << ": Setting colour " << idKey << " index " << colourIndex
                << " to " << newColour.toString());
    }
    else
    {
        DBG("ColourPage::" << __func__ << ": Unknown colour key " << idKey);
    }
}
