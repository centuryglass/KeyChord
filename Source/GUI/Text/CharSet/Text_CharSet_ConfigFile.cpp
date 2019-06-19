# define TEXT_CHARSET_CONFIG_IMPLEMENTATION

#include "Text_CharSet_ConfigFile.h"
#include "Text_CharSet_JSONKeys.h"
#include "Text_CharSet_JSONResource.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Text::CharSet::ConfigFile::";
#endif

Text::CharSet::ConfigFile::ConfigFile() { }


// Gets cached data for a configurable character set.
const Text::CharSet::Cache& Text::CharSet::ConfigFile::getActiveSet() const
{
    const SharedResource::LockedPtr<const JSONResource> charsetConfig
            = getReadLockedResource();
    return charsetConfig->getCharacterSet(charsetConfig->getActiveType());
}


// Gets a character set's displayed name.
juce::String Text::CharSet::ConfigFile::getSetName(const Type type) const
{
    using juce::String;
    const SharedResource::LockedPtr<const JSONResource> charsetConfig
            = getReadLockedResource();
    switch(type)
    {
        case Type::main:
            return charsetConfig->getConfigValue<String>(JSONKeys::mainSetName);
        case Type::alt:
            return charsetConfig->getConfigValue<String>(JSONKeys::altSetName);
        case Type::special:
            return charsetConfig->getConfigValue<String>(
                    JSONKeys::specialSetName);
        case Type::modifier:
            return "modifier (TODO: localize)";
    }
    DBG(dbgPrefix << __func__ << ": invalid set type \"" << (int) type
            << "\".");
    return String();
}


// Gets the character set type that is currently selected.
const Text::CharSet::Type Text::CharSet::ConfigFile::getActiveType() const
{
    const SharedResource::LockedPtr<const JSONResource> charsetConfig
            = getReadLockedResource();
    return charsetConfig->getActiveType();
}


// Updates the active character set type
void Text::CharSet::ConfigFile::setActiveType(const Type newActiveType)
{
    const SharedResource::LockedPtr<JSONResource> charsetConfig
            = getWriteLockedResource();
    charsetConfig->setActiveType(newActiveType);
}


// Checks if shifted character sets are currently in use.
bool Text::CharSet::ConfigFile::getShifted() const
{
    const SharedResource::LockedPtr<const JSONResource> charsetConfig
            = getReadLockedResource();
    return charsetConfig->getShifted();
}


// Sets whether the shifted versions of character sets will be used.
void Text::CharSet::ConfigFile::setShifted(const bool shifted)
{
    const SharedResource::LockedPtr<JSONResource> charsetConfig
            = getWriteLockedResource();
    charsetConfig->setShifted(shifted);
}
