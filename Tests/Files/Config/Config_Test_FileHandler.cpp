#define TEST_RESOURCE_IMPLEMENTATION

#include "Config_Test_FileHandler.h"
#include "Config_Test_Resource.h"
#include "Config_Test_JSONKeys.h"

Config::Test::FileHandler::FileHandler() { }


// Gets the file's test integer value.
int Config::Test::FileHandler::getTestInt() const
{
    return getConfigValue<int>(JSONKeys::testInt);
}


// Gets the file's test string value.
juce::String Config::Test::FileHandler::getTestString() const
{
    return getConfigValue<juce::String>(JSONKeys::testString);
}


// Gets the file's test bool value.
bool Config::Test::FileHandler::getTestBool() const
{
    return getConfigValue<bool>(JSONKeys::testBool);
}


// Gets the file's test double value.
double Config::Test::FileHandler::getTestDouble() const
{
    return getConfigValue<double>(JSONKeys::testDouble);
}


// Gets the file's test array value.
juce::Array<int> Config::Test::FileHandler::getTestArray() const
{
    SharedResource::LockedPtr<const Resource> resource
            = getReadLockedResource();
    return resource->getTestArray();
}


// Gets the file's test object value.
Config::Test::ObjectData Config::Test::FileHandler::getTestObject() const
{
    SharedResource::LockedPtr<const Resource> resource
            = getReadLockedResource();
    return resource->getTestObject();
}


// Sets the file's test integer value.
void Config::Test::FileHandler::setTestInt(const int newInt)
{
    setConfigValue<int>(JSONKeys::testInt, newInt);
}


// Sets the file's test string value.
void Config::Test::FileHandler::setTestString(const juce::String newString)
{
    setConfigValue<juce::String>(JSONKeys::testString, newString);
}


// Sets the file's test bool value.
void Config::Test::FileHandler::setTestBool(const bool newBool)
{
    setConfigValue<bool>(JSONKeys::testBool, newBool);
}


// Sets the file's test double value.
void Config::Test::FileHandler::setTestDouble(const double newDouble)
{
    setConfigValue<double>(JSONKeys::testDouble, newDouble);
}


// Sets the single array data value stored in this configuration file.
void Config::Test::FileHandler::setTestArray(const juce::Array<int> newArray)
{
    SharedResource::LockedPtr<Resource> resource = getWriteLockedResource();
    resource->setTestArray(newArray);
}


// Sets the file's test object value.
void Config::Test::FileHandler::setTestObject(const ObjectData newObject)
{
    SharedResource::LockedPtr<Resource> resource = getWriteLockedResource();
    resource->setTestObject(newObject);
}


// Restores all default values.
void Config::Test::FileHandler::restoreToDefault()
{
    SharedResource::LockedPtr<Resource> resource = getWriteLockedResource();
    resource->restoreDefaultValues();
    resource->setTestArray({1, 2, 3});
    resource->setTestObject(ObjectData(5, false));
}
