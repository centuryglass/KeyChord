#pragma once
#include "JuceHeader.h"
/**
 * @file  Config_DataKey.h
 *
 * @brief  Holds the key string and data type of a value in a JSON
 *         configuration file.
 */

namespace Config { struct DataKey; }

/**
 * @brief  Provides the key and expected value type of a value stored in a
 *         Config::FileResource's JSON file.
 *
 *  DataKeys are immutable objects, as the keys and types of basic data values
 * in configuration files should not change. DataKeys may represent string,
 * integer, boolean, or double values.
 */
struct Config::DataKey
{
    /**
     * @brief  All valid key data types.
     */
    enum DataType
    {
        stringType,
        intType,
        boolType,
        doubleType
    };

    /**
     * @brief  Initializes a DataKey, constructing its key from a C-string.
     *
     * @param keyString  The key string identifying a value stored in a
     *                   configuration file.
     *
     * @param dataType   The data type of that stored value.
     */
    DataKey(const char* keyString, const DataType dataType);

    /**
     * @brief  Initializes a DataKey, constructing its key from an existing
     *         Identifier.
     *
     * @param key        The key identifying a value stored in a configuration
     *                   file.
     *
     * @param dataType   The data type of that stored value.
     */
    DataKey(const juce::Identifier& keyString, const DataType dataType);

    // The value's JSON key:
    const juce::Identifier key;

    // The value's data type:
    const DataType dataType;

    /**
     * @brief  Allows the DataKey to be used as if it was just its key value.
     *
     * @return  A reference to the DataKey's Identifier key.
     */
    operator const juce::Identifier&() const;
};
