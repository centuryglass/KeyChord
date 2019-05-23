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
    {"Chord Text",
            &ChordComponent::text},
    {"Inactive Chord Text",
            &ChordComponent::inactiveText},
    {"Chord1 (Selected)",
            &ChordComponent::chord1Selected},
    {"Chord2 (Selected)",
            &ChordComponent::chord2Selected},
    {"Chord3 (Selected)",
            &ChordComponent::chord3Selected},
    {"Chord4 (Selected)",
            &ChordComponent::chord4Selected},
    {"Chord5 (Selected)",
            &ChordComponent::chord5Selected},
    {"Empty key (Selected)",
            &ChordComponent::emptySelected},
    {"Chord1 (Active)",
            &ChordComponent::chord1Active},
    {"Chord2 (Active)",
            &ChordComponent::chord2Active},
    {"Chord3 (Active)",
            &ChordComponent::chord3Active},
    {"Chord4 (Active)",
            &ChordComponent::chord4Active},
    {"Chord5 (Active)",
            &ChordComponent::chord5Active},
    {"Empty key (Active)",
            &ChordComponent::emptyActive},
    {"Chord1 (Open)",
            &ChordComponent::chord1Open},
    {"Chord2 (Open)",
            &ChordComponent::chord2Open},
    {"Chord3 (Open)",
            &ChordComponent::chord3Open},
    {"Chord4 (Open)",
            &ChordComponent::chord4Open},
    {"Chord5 (Open)",
            &ChordComponent::chord5Open},
    {"Empty key (Open)",
            &ChordComponent::emptyOpen},
    {"Chord1 (Blocked)",
            &ChordComponent::chord1Blocked},
    {"Chord2 (Blocked)",
            &ChordComponent::chord2Blocked},
    {"Chord3 (Blocked)",
            &ChordComponent::chord3Blocked},
    {"Chord4 (Blocked)",
            &ChordComponent::chord4Blocked},
    {"Chord5 (Blocked)",
            &ChordComponent::chord5Blocked},
    {"Empty key (Blocked)",
            &ChordComponent::emptyBlocked},
    {"Active Modifier Text",
            &ChordComponent::activeModText},
    {"Active Modifier Fill",
            &ChordComponent::activeModFill},
    {"Active Modifier Line",
            &ChordComponent::activeModLine},
    {"Inactive Modifier Text",
            &ChordComponent::inactiveModText},
    {"Inactive Modifier Fill",
            &ChordComponent::inactiveModFill},
    {"Inactive Modifier Line",
            &ChordComponent::inactiveModLine}
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
