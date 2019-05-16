#define TEST_RESOURCE_IMPLEMENTATION
#include "Config_Test_Listener.h"
#include "Config_Test_Resource.h"

Config::Test::Listener::Listener() { }


// Gets the last updated key this listener received.
juce::String Config::Test::Listener::getLastUpdated() const
{
    if (lastUpdated == nullptr)
    {
        return "nullptr";
    }
    return lastUpdated->toString();
}


// Saves the updated key when the configuration file is updated.
void Config::Test::Listener::configValueChanged
(const juce::Identifier& propertyKey)
{
    lastUpdated = &propertyKey;
}
