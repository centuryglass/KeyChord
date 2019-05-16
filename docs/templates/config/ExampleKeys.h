#pragma once
#include "Config_DataKey.h"
/**
 * @file  ExampleKeys.h
 *
 * @brief  An example config key file.
 */

namespace ExampleKeys
{
    //###################### Integer value keys: ###############################
    // A value provided to use as an example integer value.
    static const Config::DataKey exampleInt
        ("example integer", Config::DataKey::intType);

    //####################### String value keys: ###############################
    // A value provided to use as an example string value.
    static const Config::DataKey exampleString
        ("example string", Config::DataKey::stringType);

    //######################## Boolean value keys: #############################
    // A value provided to use as an example boolean value.
    static const Config::DataKey exampleBool
        ("example boolean", Config::DataKey::boolType);

    //######################## Double value keys: ##############################
    // A value provided to use as an example double value.
    static const Config::DataKey exampleDouble
        ("example double", Config::DataKey::doubleType);

    // All basic data keys tracked in the Example configuration file:
    static const std::vector<Config::DataKey> allKeys
    {
        exampleInt,
        exampleString,
        exampleBool,
        exampleDouble
    };

    //#################### Custom data type keys: ##############################
    // A value provided to use as an example array value.
    static const juce::Identifier exampleArray("example array");

    // A value provided to use as an example object value.
    static const juce::Identifier exampleObject("example object");

    //##################### Example object keys: ###############################
    // An arbitrary integer value stored within example objects.
    static const juce::Identifier exampleObjectNum("example object num");
    // An arbitrary boolean value stored within example objects.
    static const juce::Identifier exampleObjectBool("example object bool");
}
