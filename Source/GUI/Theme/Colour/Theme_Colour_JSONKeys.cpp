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
    "Widget (off)",
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
    {"Main Window Background",
            &ResizableWindow::background},
    {"Active chord key text",
            &Component_ChordPreview::text},
    {"Inactive Chord Text",
            &Component_ChordPreview::inactiveText},
    {"Chord1 (Selected)",
            &Component_ChordPreview::chord1Selected},
    {"Chord2 (Selected)",
            &Component_ChordPreview::chord2Selected},
    {"Chord3 (Selected)",
            &Component_ChordPreview::chord3Selected},
    {"Chord4 (Selected)",
            &Component_ChordPreview::chord4Selected},
    {"Chord5 (Selected)",
            &Component_ChordPreview::chord5Selected},
    {"Empty key (Selected)",
            &Component_ChordPreview::emptySelected},
    {"Chord1 (Active)",
            &Component_ChordPreview::chord1Active},
    {"Chord2 (Active)",
            &Component_ChordPreview::chord2Active},
    {"Chord3 (Active)",
            &Component_ChordPreview::chord3Active},
    {"Chord4 (Active)",
            &Component_ChordPreview::chord4Active},
    {"Chord5 (Active)",
            &Component_ChordPreview::chord5Active},
    {"Empty key (Active)",
            &Component_ChordPreview::emptyActive},
    {"Chord1 (Open)",
            &Component_ChordPreview::chord1Open},
    {"Chord2 (Open)",
            &Component_ChordPreview::chord2Open},
    {"Chord3 (Open)",
            &Component_ChordPreview::chord3Open},
    {"Chord4 (Open)",
            &Component_ChordPreview::chord4Open},
    {"Chord5 (Open)",
            &Component_ChordPreview::chord5Open},
    {"Empty key (Open)",
            &Component_ChordPreview::emptyOpen},
    {"Chord1 (Blocked)",
            &Component_ChordPreview::chord1Blocked},
    {"Chord2 (Blocked)",
            &Component_ChordPreview::chord2Blocked},
    {"Chord3 (Blocked)",
            &Component_ChordPreview::chord3Blocked},
    {"Chord4 (Blocked)",
            &Component_ChordPreview::chord4Blocked},
    {"Chord5 (Blocked)",
            &Component_ChordPreview::chord5Blocked},
    {"Empty key (Blocked)",
            &Component_ChordPreview::emptyBlocked},
    {"Input preview text",
            &Component_InputView::text},
    {"Input preview background",
            &Component_InputView::background},
    {"Input preview outline",
            &Component_InputView::outline},
    {"Input preview highlight",
            &Component_InputView::inputHighlight}
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
