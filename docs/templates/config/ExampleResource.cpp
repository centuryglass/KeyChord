#define EXAMPLE_RESOURCE_IMPLEMENTATION

#include "ExampleResource.h"
#include "ExampleKeys.h"

// Filename of the JSON configuration file
static const constexpr char * configFilename = "example.json";

// SharedResource object key
const juce::Identifier ExampleResource::resourceKey = "ExampleResource";


ExampleResource::ExampleResource() :
FileResource(resourceKey, configFilename)
{
    using juce::DynamicObject;
    using juce::var;

    // Load object value data:
    DynamicObject::Ptr objectData 
            = initProperty<DynamicObject*>(ExampleKeys::exampleObject);

    // In this example, object data members are silently assigned default values
    // if the values in the configuration file are missing or have incorrect
    // types. When writing your own configuration files, consider if it would be
    // more appropriate to warn the user that the stored values were invalid.
    int objectNum = 0;
    bool objectBool = false;
    const var& numVar = objectData->getProperty(ExampleKeys::exampleObjectNum);
    const var& boolVar = objectData->getProperty(
            ExampleKeys::exampleObjectBool);
    if (numVar.isInt())
    {
        objectNum = (int) numVar;
    }
    if (boolVar.isBool())
    {
        objectBool = (bool) boolVar;
    }
    exampleObject.reset(new ExampleObject(objectNum, objectBool));

    // Load array value data:
    var arrayData = initProperty<var>(ExampleKeys::exampleArray);
    if (arrayData.isArray())
    {
        for (const var& arrayItem : *arrayData.getArray())
        {
            if (arrayItem.isInt())
            {
                exampleArray.add((int) arrayItem);
            }
            /*
            else
            {
                // In this example, invalid array values are simply ignored.
                // Depending on the circumstances, you may want to show an error
                // message or push a default value to the array instead.
            }
            */
        }
    }
    // Finish loading JSON data. Always call this method exactly once, at the
    // end of the constructor in your configuration resource class.
    loadJSONData();
}


// Gets the single array data value stored in this configuration file.
juce::Array<int> ExampleResource::getExampleArray() const
{
    return exampleArray;
}


// Gets the single object data value stored in this configuration file.
ExampleObject ExampleResource::getExampleObject() const
{
    return *exampleObject;
}


// Sets the single object data value stored in this configuration file.
void ExampleResource::setExampleObject(ExampleObject newExampleObject)
{
    exampleObject.reset(new ExampleObject(newExampleObject));
    writeChanges();
}


// Gets the set of all basic(non-array, non-object) properties tracked by this
// Resource.
const std::vector<Config::DataKey>& ExampleResource::getConfigKeys() const
{
    return ExampleKeys::allKeys;
}


// Checks if a key string is valid for this FileResource.
bool ExampleResource::isValidKey(const juce::Identifier& key) const
{
    return Config::FileResource::isValidKey(key)
            || (key == ExampleKeys::exampleArray)
            || (key == ExampleKeys::exampleObject);
}


// Writes all custom object and array data back to the JSON file.
void ExampleResource::writeDataToJSON()
{
    using juce::DynamicObject;
    using juce::Array;
    using juce::var;

    // Write back array data:
    Array<var> varArray;
    for (const int& arrayValue : exampleArray)
    {
        varArray.add(var(arrayValue));
    }
    updateProperty<var>(ExampleKeys::exampleArray, varArray);

    // Write back object data:
    DynamicObject::Ptr jsonObject = new DynamicObject;
    jsonObject->setProperty(ExampleKeys::exampleObjectNum,
            exampleObject->getExampleNum());
    jsonObject->setProperty(ExampleKeys::exampleObjectBool,
            exampleObject->getExampleBool());
    updateProperty<DynamicObject*>(ExampleKeys::exampleObject, jsonObject);
}
