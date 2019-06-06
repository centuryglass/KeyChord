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
    using juce::var;
    var cachedBuffer = initProperty<var>(MainKeys::cachedBuffer);
    if (cachedBuffer.isArray())
    {
        const juce::Array<var>* bufferArray = cachedBuffer.getArray();
        for (const var& bufferChar : *bufferArray)
        {
            jassert(bufferChar.isInt());
            inputCache.add(bufferChar.operator int());
        }
    }
    loadJSONData();
}

Config::MainResource::~MainResource() { }


// Loads the cached input buffer stored when restarting the application. Doing
// this removes this cached data from the file.
Text::CharString Config::MainResource::takeCachedBuffer()
{
    Text::CharString transferredBuffer = inputCache;
    inputCache.clear();
    writeChanges();
    return transferredBuffer;
}


// Saves the text input buffer to the configuration file so it can be restored
// after an immediate restart.
void Config::MainResource::cacheInputBuffer(const Text::CharString& inputBuffer)
{
    inputCache = inputBuffer;
    writeChanges();
}

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


// Checks if a key string is valid for this FileResource.
bool Config::MainResource::isValidKey(const juce::Identifier& key) const 
{
    return Config::FileResource::isValidKey(key) 
            || key == MainKeys::cachedBuffer;
}
    
/*
 * Writes input buffer data to the JSON file.
 */
void Config::MainResource::writeDataToJSON()
{
    juce::Array<juce::var> cacheData;
    for (const Text::CharValue& character : inputCache)
    {
        cacheData.add(juce::var((int) character));
    }
    setConfigValue<juce::var>(MainKeys::cachedBuffer, cacheData);
}
