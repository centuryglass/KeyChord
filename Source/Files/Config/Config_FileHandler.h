#pragma once
/**
 * @file  Config_FileHandler.h
 *
 * @brief  Controls access to a FileResource object, managing all reading from
 *         and writing to a single JSON configuration file.
 */

#include "SharedResource_Handler.h"

namespace Config { template<class ResourceType> class FileHandler; }

/**
 * @brief  Handles all access to Config::FileResource objects.
 *
 *  FileHandler objects for a FileResource may be created at any point,
 * and all of them will safely share access to the same file and JSON data.
 *
 *  FileHandlers can safely read and write values with basic data types within
 * their ThreadResource. Accessing array and object data types requires a
 * FileHandler subclass with new functions for handling the JSON file's custom
 * data types.
 *
 * @tparam ResourceType  The FileResource subclass this FileHandler will access.
 */
template<class ResourceType>
class Config::FileHandler : public SharedResource::Handler<ResourceType>
{
protected:
    FileHandler() : SharedResource::Handler<ResourceType>() { }

public:
    virtual ~FileHandler() { }

    /**
     * @brief  Gets one of the values stored in the JSON configuration file.
     *
     * @tparam ValueType                    The type of value to retrieve.
     *
     * @param key                           The key identifying the desired
     *                                      value.
     *
     * @return                              The requested configuration value.
     *
     * @throws FileResource::FileException  If the file is missing or could not
     *                                      be read.
     *
     * @throws FileResource::TypeException  If no property with this key and
     *                                      type ValueType was found.
     */
    template<typename ValueType>
    ValueType getConfigValue(const juce::Identifier& key) const
    {
        SharedResource::LockedPtr<const ResourceType> jsonPtr
            = SharedResource::Handler<ResourceType>::getReadLockedResource();
        return jsonPtr->template getConfigValue<ValueType>(key);
    }

    /**
     * @brief  Sets a value stored in the JSON configuration file.
     *
     * @param key                           The key used to select the value.
     *
     * @param newValue                      A new value to store in the JSON
     *                                      object.
     *
     * @tparam ValueType                    The type of value being stored.
     *
     * @return                              True if the value changed, false if
     *                                      the existing value matched the new
     *                                      value.
     *
     * @throws FileResource::FileException  If the file is missing or could not
     *                                      be read.
     *
     * @throws FileResource::TypeException  If a property exists that shares
     *                                      this key but is not of type
     *                                      ValueType.
     */
    template<typename ValueType >
    bool setConfigValue(const juce::Identifier& key, ValueType newValue)
    {
        SharedResource::LockedPtr<ResourceType> jsonPtr
            = SharedResource::Handler<ResourceType>::getWriteLockedResource();
        return jsonPtr->template setConfigValue<ValueType>(key, newValue);
    }
};
