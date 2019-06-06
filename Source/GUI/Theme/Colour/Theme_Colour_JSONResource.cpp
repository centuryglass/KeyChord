#include "Theme_Colour_JSONResource.h"
#include "Theme_Colour_JSONKeys.h"

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
