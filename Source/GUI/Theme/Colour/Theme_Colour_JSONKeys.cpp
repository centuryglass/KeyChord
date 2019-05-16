#include "Theme_Colour_JSONKeys.h"
#include "Theme_Colour_ColourIds.h"
#include <map>

#ifdef JUCE_DEBUG
// Print the full namespace name before all debug output:
static const constexpr char* dbgPrefix = "Theme::Colour::JSONKeys::";
#endif

namespace ColourKeys = Theme::Colour::JSONKeys;

//=============================== Colour Keys: =================================
/**
 * @brief  Keys for colour values assigned to UI element colour categories.
 *
 *  All JUCE ColourId values are grouped under one of these categories. When
 * looking up colour values, if no value is explicitly assigned to a given
 * ColourId, the value assigned to the associated UICategory will be used.
 */
static const juce::Array<juce::Identifier> uiCategoryKeys=
{
    "Window background",
    "Widget background",
    "Widget",
    "Widget(off)",
    "Menu background",
    "Outline",
    "Focused outline",
    "Text",
    "Highlighted text",
    "Text field",
    "Highlighted text field",
};

namespace Theme { namespace Colour { namespace ColourIds {

/**
 * @brief  For each JSON colour key tracked in colours.json, keyMap maps that
 *         key to its JUCE ColourId value and UI element category.
 */
static const std::map<juce::Identifier, const Element*> keyMap
{
    {"Text button(off) background",
            &TextButton::buttonOff},
    {"Text button background",
            &TextButton::button},
    {"Label background",
            &Label::background},
    {"Label text",
            &Label::text},
    {"Label outline",
            &Label::outline},
    {"Combo box text",
            &ComboBox::text},
    {"Combo box background",
            &ComboBox::background},
    {"Combo box outline",
            &ComboBox::outline},
    {"Combo box arrow",
            &ComboBox::arrow},
    {"Combo box focused outline",
            &ComboBox::focusedOutline},
    {"Slider background",
            &Slider::background},
    {"Slider thumb",
            &Slider::thumb},
    {"Slider track",
            &Slider::track},
    {"Alert window background",
            &AlertWindow::background},
    {"Alert window text",
            &AlertWindow::text},
    {"Alert window outline",
            &AlertWindow::outline},
    {"Image0",
            &Image::image0},
    {"Image1",
            &Image::image1},
    {"Image2",
            &Image::image2},
    {"Image3",
            &Image::image3},
    {"Image4",
            &Image::image4},
    {"List editor background",
            &ListEditor::background},
    {"List editor item",
            &ListEditor::listItem},
    {"List editor selected item",
            &ListEditor::selectedListItem},
    {"List editor text",
            &ListEditor::text},
    {"File selection window",
            &FilePathEditor::fileWindow},
    {"File selection text",
            &FilePathEditor::text},
    {"Overlay spinner background",
            &OverlaySpinner::background},
    {"Overlay spinner text",
            &OverlaySpinner::text},
    {"Page background",
            &Page::background},
    {"Colour picker checkerboard(light)",
            &ColourPicker::checkerboardLight},
    {"Colour picker checkerboard(dark)",
            &ColourPicker::checkerboardDark},
    {"Colour picker colour outline",
            &ColourPicker::outline},
    {"Colour picker focused colour outline",
            &ColourPicker::focusedOutline},
    {"Switch background",
            &Switch::background},
    {"Switch handle",
            &Switch::handle},
    {"Switch handle(off)",
            &Switch::handleOff},
};

}}}


// Gets all JUCE ColourId values defined by the colour config file.
juce::Array<int> ColourKeys::getColourIds()
{
    juce::Array<int> idList;
    for (const auto iter : ColourIds::allElements)
    {
        idList.add(iter->getColourId());
    }
    return idList;
}


// Gets all UI category colour keys.
const juce::Array<juce::Identifier>& ColourKeys::getCategoryKeys()
{
    return uiCategoryKeys;
}


// Gets all keys used by the ColourConfigFile.
juce::Array<const juce::Identifier*> ColourKeys::getColourKeys()
{
    juce::Array<const juce::Identifier*> colourKeys;
    for (const auto& key : uiCategoryKeys)
    {
        colourKeys.add(&key);
    }
    for (const auto& keyPair : ColourIds::keyMap)
    {
        colourKeys.add(&keyPair.first);
    }
    return colourKeys;
}


// Gets the UICategory assigned to a JUCE ColourId value.
Theme::Colour::UICategory ColourKeys::getUICategory(const int colourId)
{
    if (colourId < 0)
    {
        DBG(dbgPrefix << __func__ << ": Invalid colourId value " << colourId);
        jassertfalse;
        return UICategory::none;
    }
    int index = -1;
    int min = 0, max = ColourIds::allElements.size() - 1;
    while (min <= max)
    {
        int testIndex = min + (max - min) / 2;
        int testId = ColourIds::allElements[testIndex]->getColourId();
        if (testId > colourId)
        {
            max = testIndex;
        }
        else if (testId < colourId)
        {
            if (testIndex == min)
            {
                ++min;
            }
            else
            {
                min = testIndex;
            }
        }
        else
        {
            index = testIndex;
            break;
        }
    }
    if (index < 0)
    {
        jassertfalse;
        return UICategory::none;
    }
    return ColourIds::allElements[index]->getColourCategory();
}


//=================== Lookup values using keys: ============================
// Finds the JUCE ColourId value of a UI element from its key.
int ColourKeys::getColourId(const juce::Identifier& colourKey)
{
    auto searchIter = ColourIds::keyMap.find(colourKey);
    if (searchIter == ColourIds::keyMap.end())
    {
        DBG(dbgPrefix << __func__ << ": No ID found for key "
                << colourKey.toString());
        return -1;
    }
    return searchIter->second->getColourId();
}


// Finds the UICategory type represented by a specific key.
Theme::Colour::UICategory ColourKeys::getCategoryType
(const juce::Identifier& categoryKey)
{
    int enumVal = uiCategoryKeys.indexOf(categoryKey);
    if (enumVal < 0)
    {
        DBG(dbgPrefix << __func__ << ": No category matches key "
                << categoryKey.toString());

        return UICategory::none;
    }
    return (UICategory) enumVal;
}


//====================== Lookup keys by value: ============================
// Gets the key used to store a specific JUCE ColourId in the colour config
// file.
const juce::Identifier& ColourKeys::getColourKey(const int colourId)
{
    static std::map<int, const juce::Identifier*> idMap;
    static juce::ReadWriteLock mapProtector;
    const juce::ScopedReadLock mapReadLock(mapProtector);
    if (idMap.empty())
    {
        const juce::ScopedWriteLock mapInitLock(mapProtector);
        for (const auto& iter : ColourIds::keyMap)
        {
            idMap[iter.second->getColourId()] = &iter.first;
        }
    }
    auto keySearch = idMap.find(colourId);
    if (keySearch == idMap.end())
    {
        return invalidKey;
    }
    return *keySearch->second;
}


// Finds the key representing a UICategory value.
const juce::Identifier& ColourKeys::getCategoryKey(const UICategory category)
{
    if (category == UICategory::none)
    {
        DBG(dbgPrefix << __func__ << ": No key, category == none");
        return invalidKey;
    }
    return uiCategoryKeys.getReference((int) category);
}
