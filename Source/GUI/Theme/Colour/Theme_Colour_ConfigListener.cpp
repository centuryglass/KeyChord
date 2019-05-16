#include "Theme_Colour_ConfigListener.h"
#include "Theme_Colour_JSONResource.h"
#include "Theme_Colour_JSONKeys.h"

namespace ColourTheme = Theme::Colour;

ColourTheme::ConfigListener::ConfigListener() { }


// Sets this listener to receive updates when a specific JUCE ColourId value
// changes.
void ColourTheme::ConfigListener::addTrackedColourId(const int colourId)
{
    const juce::ScopedLock colourLock(trackedColourIds.getLock());
    trackedColourIds.addIfNotAlreadyThere(colourId);
}


// Stops this listener from receiving updates when a specific JUCE ColourId
// value changes.
void ColourTheme::ConfigListener::removeTrackedColourId(const int colourId)
{
    const juce::ScopedLock colourLock(trackedColourIds.getLock());
    trackedColourIds.removeAllInstancesOf(colourId);
}


// Checks if a specific ID is tracked by this Listener.
bool ColourTheme::ConfigListener::isTrackedId(const int colourId) const
{
    return trackedColourIds.contains(colourId);
}


// Gets the list of ColourId values tracked by this Listener.
const juce::Array<int, juce::CriticalSection>&
ColourTheme::ConfigListener::getTrackedIds() const
{
    return trackedColourIds;
}


// Calls configValueChanged for each tracked key, and calls colourChanged for
// each tracked colourId.
void ColourTheme::ConfigListener::loadAllConfigProperties()
{
    using namespace Theme::Colour::JSONKeys;
    using juce::String;
    using juce::Identifier;
    using juce::Colour;

    const juce::ScopedLock colourLock(trackedColourIds.getLock());
    for (const int& colourId : trackedColourIds)
    {
        const Identifier& idKey = getColourKey(colourId);
        if (idKey != invalidKey)
        {
            String colourStr = getConfigValue<String>(idKey);
            if (colourStr.isNotEmpty())
            {
                colourChanged(colourId, idKey,
                        Colour(colourStr.getHexValue32()));
                continue;
            }
        }
        // ID has no key, or specific colour not defined, use category colour:
        UICategory idCategory = getUICategory(colourId);
        if (idCategory != UICategory::none)
        {
            const Identifier& catKey = getCategoryKey(idCategory);
            colourChanged(colourId, catKey,
                    Colour(getConfigValue<String>(catKey).getHexValue32()));
        }
    }
    Config::Listener<JSONResource>::loadAllConfigProperties();
}


// Notifies the Listener when a colour value it tracks is updated.
void ColourTheme::ConfigListener::colourChanged(
        const int colourId,
        const juce::Identifier& updatedKey,
        const juce::Colour newColour)
{
    DBG("Theme::Colour::ConfigListener::" << __func__
            << ": Listener class didn't override notification function!");
    jassertfalse;
}
