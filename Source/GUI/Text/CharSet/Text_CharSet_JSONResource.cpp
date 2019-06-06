#define TEXT_CHARSET_CONFIG_IMPLEMENTATION

#include "Text_CharSet_JSONResource.h"
#include "Text_CharSet_JSONKeys.h"

// SharedResource object key:
const juce::Identifier Text::CharSet::JSONResource::resourceKey
        = "Text_CharSet_JSONResource";

// Configuration file name:
static const juce::String configFilename("charSets.json");

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Text::CharSet::JSONResource::";
#endif

// Loads all character sets on construction.
Text::CharSet::JSONResource::JSONResource() : 
Config::FileResource(resourceKey, configFilename)
{
    using juce::var;
    using juce::String;

    // Stores a character set's key and type:
    struct SetLoadingData
    {
        const juce::Identifier& key;
        const Type type;
    };
    const SetLoadingData setList[] =
    {
        { JSONKeys::mainCharSet,    Type::main },
        { JSONKeys::altCharSet,     Type::alt },
        { JSONKeys::specialCharSet, Type::special }
    };
    for (const SetLoadingData& setData : setList)
    {
        DBG(dbgPrefix << __func__ << ": initializing character set \""
                << setData.key.toString() << "\"");

        var setArray = initProperty<var>(setData.key);
        characterSets.add(Cache(setArray));
    }
    characterSets.add(Cache::getModCharset());
    loadJSONData();
}


// Gets a character set.
const Text::CharSet::Cache& Text::CharSet::JSONResource::getCharacterSet
(const Type setType) const
{
    return characterSets.getReference((int) setType);
}


// Gets the character set type that is currently selected.
Text::CharSet::Type Text::CharSet::JSONResource::getActiveType() const
{
    return activeType;
}


// Checks if shifted character sets are currently in use.
bool Text::CharSet::JSONResource::getShifted() const
{
    return shifted;
}


// Updates the active character set type
void Text::CharSet::JSONResource::setActiveType(const Type newActiveType) 
{
    activeType = newActiveType;
}


// Sets whether the shifted versions of character sets will be used.
void Text::CharSet::JSONResource::setShifted(const bool shifted)
{
    this->shifted = shifted;
}

// Gets the set of all basic(non-array, non-object) properties tracked by this
// Resource.
const std::vector<Config::DataKey>& Text::CharSet::JSONResource::getConfigKeys()
    const  
{
    static const std::vector<Config::DataKey> keyList =
    {
        JSONKeys::mainSetName,
        JSONKeys::altSetName,
        JSONKeys::specialSetName,
    };
    return keyList;
}


// Checks if a key string is valid for this FileResource.
bool Text::CharSet::JSONResource::isValidKey
(const juce::Identifier& key) const 
{
    using namespace JSONKeys;
    const juce::Array<juce::Identifier> keyList =
    {
        mainSetName.key,
        altSetName.key,
        specialSetName.key,
        mainCharSet,
        altCharSet,
        specialCharSet
    };
    return keyList.contains(key);
}
