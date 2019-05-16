#include "Theme_Colour_JSONResource.h"
#include "Theme_Colour_JSONKeys.h"
#include "Config_Listener.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Theme::Colour::JSONResource::";
#endif

// SharedResource object key:
const juce::Identifier Theme::Colour::JSONResource::resourceKey
        = "Theme::Colour::JSONResource";

// Filename of the JSON configuration file:
static const constexpr char * configFilename = "colours.json";

Theme::Colour::JSONResource::JSONResource() :
Config::FileResource(resourceKey, configFilename)
{
    loadJSONData();
}


// Gets the set of all basic(non-array, non-object) properties tracked by this
// JSON resource.
const std::vector<Config::DataKey>&
Theme::Colour::JSONResource::getConfigKeys() const
{
    using juce::Identifier;
    using Config::DataKey;
    static std::vector<DataKey> keys;
    if (keys.empty())
    {
        for (const Identifier* key : JSONKeys::getColourKeys())
        {
            keys.push_back(DataKey(*key, DataKey::stringType));
        }
    }
    return keys;
}


// Checks if a single handler object is a Listener tracking updates of a single
// key value, and if so, notifies it that the tracked value has updated.
void Theme::Colour::JSONResource::notifyListener(ListenerInterface* listener,
        const juce::Identifier& key)
{
    using namespace Theme::Colour::JSONKeys;
    using juce::String;
    using juce::Identifier;
    Config::Listener<JSONResource>* configListener
            = dynamic_cast<Config::Listener<JSONResource>*> (listener);
    if (configListener != nullptr)
    {
        // Check for and notify listeners tracking colours by ColourId:
        String colourStr = getConfigValue<String>(key);
        int colourId = getColourId(key);
        if (colourStr.isEmpty())
        {
            if (colourId == -1)
            {
                // Category colour removed, this shouldn't happen.
                DBG(dbgPrefix << __func__ << ": Colour category "
                        << key << " value was removed!");
                jassertfalse;
            }
            // Colour removed, revert to category colour
            colourStr = getConfigValue<String>
                    (getCategoryKey(getUICategory(colourId)));
            jassert(colourStr.isNotEmpty());
        }
        const juce::Colour newColour(colourStr.getHexValue32());
        if (colourId > -1)
        {
            if (listener->isTrackedId(colourId))
            {
                listener->colourChanged(colourId, key, newColour);
            }
        }
        else // Colour category changed
        {
            UICategory category = getCategoryType(key);
            const juce::Array<int, juce::CriticalSection>& trackedColourIds
                    = listener->getTrackedIds();
            for (const int& trackedId : trackedColourIds)
            {
                if (getUICategory(trackedId) == category)
                {
                    const Identifier& idKey = getColourKey(trackedId);
                    if (idKey == invalidKey
                            || getConfigValue<String>(idKey).isEmpty())
                    {
                        listener->colourChanged(trackedId, key, newColour);
                    }
                }
            }
        }
    }
}
