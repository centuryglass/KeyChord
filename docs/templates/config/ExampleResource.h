#pragma once
#ifndef EXAMPLE_RESOURCE_IMPLEMENTATION
    #error "ExampleResource included outside of Config Example implementation."
#endif
/**
 * @file  ExampleResource.h
 *
 * @brief  An example of a json configuration resource class.
 */

#include "ExampleObject.h"
#include "Config_FileResource.h"
#include "JuceHeader.h"

/**
 * @brief  An example configuration file resource class.
 */
class ExampleResource : public Config::FileResource
{
public:
    // SharedResource object key
    static const juce::Identifier resourceKey;

    ExampleResource();

    virtual ~ExampleResource() { }

    /**
     * @brief  Gets the single array data value stored in this configuration
     *         file.
     *
     * @return  The exampleArray value.
     */
    juce::Array<int> getExampleArray() const;

    /**
     * @brief  Gets the single object data value stored in this configuration
     *         file.
     *
     * @return  The exampleObject value.
     */
    ExampleObject getExampleObject() const; 

    /**
     * @brief  Sets the single object data value stored in this configuration
     *         file.
     *
     * @param newExampleObject  The new exampleObject value.
     */
    void setExampleObject(ExampleObject newExampleObject);

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

    // Cached data from the example array value:
    juce::Array<int> exampleArray;

    // Cached data from the example object value:
    std::unique_ptr<ExampleObject> exampleObject; 
};
