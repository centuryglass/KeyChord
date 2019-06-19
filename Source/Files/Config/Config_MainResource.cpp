#define CONFIG_MAIN_IMPLEMENTATION
#include "Config_MainResource.h"
#include "Config_MainKeys.h"

// SharedResource object instance key:
const juce::Identifier Config::MainResource::resourceKey("Config_MainResource");

// Configuration file name:
static const juce::String configFilename("config.json");


Config::MainResource::MainResource() :
Config::FileResource(resourceKey, configFilename)
{
    loadJSONData();
}

Config::MainResource::~MainResource() { }


// Gets the set of all basic(non-array, non-object) properties tracked by this
// Resource.
const std::vector<Config::DataKey>& Config::MainResource::getConfigKeys() const
{
    static const std::vector<Config::DataKey> keyList =
    {
        MainKeys::minimized,
        MainKeys::snapToBottom,
        MainKeys::immediateMode
    };
    return keyList;
}
