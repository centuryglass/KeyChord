#pragma once
#ifndef TEST_RESOURCE_IMPLEMENTATION
    #error "Test::Resource included outside of Config::Test implementation."
#endif
/**
 * @file  Config_Test_Resource.h
 *
 * @brief  A basic json configuration resource class used for testing.
 */

#include "Config_Test_ObjectData.h"
#include "Config_FileResource.h"
#include "JuceHeader.h"

namespace Config { namespace Test { class Resource; } }

/**
 * @brief  An test configuration file resource class.
 */
class Config::Test::Resource : public Config::FileResource
{
public:
    // SharedResource object key
    static const juce::Identifier resourceKey;

    Resource();

    virtual ~Resource() { }

    /**
     * @brief  Gets the single array data value stored in this configuration
     *         file.
     *
     * @return  The testArray value.
     */
    juce::Array<int> getTestArray() const;

    /**
     * @brief  Sets the single array data value stored in this configuration
     *         file.
     *
     * @param newArray  The new array value.
     */
    void setTestArray(const juce::Array<int> newArray);

    /**
     * @brief  Gets the single object data value stored in this configuration
     *         file.
     *
     * @return  The testObject value.
     */
    ObjectData getTestObject() const;

    /**
     * @brief  Sets the single object data value stored in this configuration
     *         file.
     *
     * @param newObjectData  The new testObject value.
     */
    void setTestObject(ObjectData newObjectData);

private:
    /**
     * @brief  Gets the set of all basic(non-array, non-object) properties
     *         tracked by this Resource.
     *
     * @return  The keys to all basic variables tracked in this config file.
     */
    const std::vector<Config::DataKey>& getConfigKeys() const final override;

    /**
     * @brief  Checks if a key string is valid for this FileResource. This
     *         function only needs to be overridden if your config file stores
     *         array or object data values.
     *
     * @param key  A key string value to check.
     *
     * @return     Whether the key is valid for this file.
     */
    bool isValidKey(const juce::Identifier& key) const override;

    /**
     * @brief  Writes all custom object or array data back to the JSON file.
     *
     *  This function only needs to be overridden if your config file stores
     * object or array data values.
     */
    void writeDataToJSON() override;

    // Cached data from the test array value:
    juce::Array<int> testArray;

    // Cached data from the test object value:
    std::unique_ptr<ObjectData> testObject;
};
