#define TEST_RESOURCE_IMPLEMENTATION

#include "Config_Test_Resource.h"
#include "Config_Test_JSONKeys.h"

// Filename of the JSON configuration file
static const constexpr char * configFilename = "configTest.json";

// SharedResource object key
const juce::Identifier Config::Test::Resource::resourceKey = "TestResource";


Config::Test::Resource::Resource() :
FileResource(resourceKey, configFilename)
{
    using juce::DynamicObject;
    using juce::var;

    // Load object value data:
    DynamicObject::Ptr objectData
            = initProperty<DynamicObject*>(JSONKeys::testObject);

    // In this test, object data members are silently assigned default values
    // if the values in the configuration file are missing or have incorrect
    // types. When writing your own configuration files, consider if it would be
    // more appropriate to warn the user that the stored values were invalid.
    int objectNum = 0;
    bool objectBool = false;
    const var& numVar = objectData->getProperty(JSONKeys::testObjectNum);
    const var& boolVar = objectData->getProperty(
            JSONKeys::testObjectBool);
    if (numVar.isInt())
    {
        objectNum = (int) numVar;
    }
    if (boolVar.isBool())
    {
        objectBool = (bool) boolVar;
    }
    testObject.reset(new ObjectData(objectNum, objectBool));

    // Load array value data:
    var arrayData = initProperty<var>(JSONKeys::testArray);
    if (arrayData.isArray())
    {
        for (const var& arrayItem : *arrayData.getArray())
        {
            if (arrayItem.isInt())
            {
                testArray.add((int) arrayItem);
            }
            // else
            // {
            // // In this test, invalid array values are simply ignored.
            // // Depending on the circumstances, you may want to show an error
            // // message or push a default value to the array instead.
            // }
        }
    }
    // Finish loading JSON data. Always call this method exactly once, at the
    // end of the constructor in your configuration resource class.
    loadJSONData();
}


// Gets the single array data value stored in this configuration file.
juce::Array<int> Config::Test::Resource::getTestArray() const
{
    return testArray;
}


// Sets the single array data value stored in this configuration file.
void Config::Test::Resource::setTestArray(const juce::Array<int> newArray)
{
    testArray = newArray;
    writeChanges();
}


// Gets the single object data value stored in this configuration file.
Config::Test::ObjectData Config::Test::Resource::getTestObject() const
{
    return *testObject;
}


// Sets the single object data value stored in this configuration file.
void Config::Test::Resource::setTestObject(ObjectData newObjectData)
{
    testObject.reset(new ObjectData(newObjectData));
    writeChanges();
}


// Gets the set of all basic(non-array, non-object) properties tracked by this
// Resource.
const std::vector<Config::DataKey>& Config::Test::Resource::getConfigKeys()
        const
{
    return JSONKeys::allKeys;
}


// Checks if a key string is valid for this FileResource.
bool Config::Test::Resource::isValidKey(const juce::Identifier& key) const
{
    return Config::FileResource::isValidKey(key)
            || (key == JSONKeys::testArray)
            || (key == JSONKeys::testObject);
}


// Writes all custom object and array data back to the JSON file.
void Config::Test::Resource::writeDataToJSON()
{
    using juce::DynamicObject;
    using juce::Array;
    using juce::var;

    // Write back array data:
    Array<var> varArray;
    for (const int& arrayValue : testArray)
    {
        varArray.add(var(arrayValue));
    }
    updateProperty<var>(JSONKeys::testArray, varArray);

    // Write back object data:
    DynamicObject::Ptr jsonObject = new DynamicObject;
    jsonObject->setProperty(JSONKeys::testObjectNum,
            testObject->getTestNum());
    jsonObject->setProperty(JSONKeys::testObjectBool,
            testObject->getTestBool());
    updateProperty<DynamicObject*>(JSONKeys::testObject, jsonObject);
}
