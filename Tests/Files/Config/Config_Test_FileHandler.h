#pragma once
/**
 * @file  Config_Test_FileHandler.h
 *
 * @brief  Reads and writes values stored in the example.json file.
 */

#include "Config_FileHandler.h"
#include "Config_Test_ObjectData.h"

namespace Config
{
    namespace Test
    {
        class Resource;
        class FileHandler;
    }
}

/**
 * @brief  Accesses the configuration file resource used to test the Config
 *         module.
 */
class Config::Test::FileHandler : private Config::FileHandler<Resource>
{
public:
    FileHandler();

    virtual ~FileHandler() { }

    /**
     * @brief  Gets the file's test integer value.
     *
     * @return  The test file integer value.
     */
    int getTestInt() const;

    /**
     * @brief  Gets the file's test string value.
     *
     * @return  The test file string value.
     */
    juce::String getTestString() const;

    /**
     * @brief  Gets the file's test bool value.
     *
     * @return  The test file bool value.
     */
    bool getTestBool() const;

    /**
     * @brief  Gets the file's test double value.
     *
     * @return  The test file double value.
     */
    double getTestDouble() const;

    /**
     * @brief  Gets the file's test array value.
     *
     * @return  The test file array value.
     */
    juce::Array<int> getTestArray() const;

    /**
     * @brief  Gets the file's test object value.
     *
     * @return  The test file object value.
     */
    ObjectData getTestObject() const;

    /**
     * @brief  Sets the file's test integer value.
     *
     * @param newInt  The new file integer value to set.
     */
    void setTestInt(const int newInt);

    /**
     * @brief  Sets the file's test string value.
     *
     * @param newString  The new file string value to set.
     */
    void setTestString(const juce::String newString);

    /**
     * @brief  Sets the file's test bool value.
     *
     * @param newBool  The new file bool value to set.
     */
    void setTestBool(const bool newBool);

    /**
     * @brief  Sets the file's test double value.
     *
     * @param newDouble  The new file double value to set.
     */
    void setTestDouble(const double newDouble);

    /**
     * @brief  Sets the file's test array value.
     *
     * @param newArray  The new array value to set.
     */
    void setTestArray(const juce::Array<int> newArray);

    /**
     * @brief  Sets the file's test object value.
     *
     * @param newObject  The new object value to set.
     */
    void setTestObject(const ObjectData newObject);

    /**
     * @brief  Restores all default values.
     */
    void restoreToDefault();
};
