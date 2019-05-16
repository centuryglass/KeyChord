#pragma once
#include "Config_DataKey.h"
/**
 * @file  Config_Test_JSONKeys.h
 *
 * @brief  Keys used in the test configuration file.
 */

namespace Config { namespace Test { namespace JSONKeys
{
    //###################### Integer value keys: ###############################
    // A value provided to use as a test integer value.
    static const Config::DataKey testInt
        ("test integer", Config::DataKey::intType);

    //####################### String value keys: ###############################
    // A value provided to use as a test string value.
    static const Config::DataKey testString
        ("test string", Config::DataKey::stringType);

    //######################## Boolean value keys: #############################
    // A value provided to use as a test boolean value.
    static const Config::DataKey testBool
        ("test boolean", Config::DataKey::boolType);

    //######################## Double value keys: ##############################
    // A value provided to use as a test double value.
    static const Config::DataKey testDouble
        ("test double", Config::DataKey::doubleType);

    // All basic data keys tracked in the Test configuration file:
    static const std::vector<Config::DataKey> allKeys
    {
        testInt,
        testString,
        testBool,
        testDouble
    };

    //#################### Custom data type keys: ##############################
    // A value provided to use as a test array value.
    static const juce::Identifier testArray("test array");

    // A value provided to use as a test object value.
    static const juce::Identifier testObject("test object");

    //##################### Test object keys: ###############################
    // An arbitrary integer value stored within test objects.
    static const juce::Identifier testObjectNum("test object num");
    // An arbitrary boolean value stored within test objects.
    static const juce::Identifier testObjectBool("test object bool");
} } }
