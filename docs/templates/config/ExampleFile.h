#pragma once
/**
 * @file  ExampleFile.h
 *
 * @brief  Reads and writes values stored in the example.json file.
 */

#include "Config_FileHandler.h"
#include "ExampleObject.h"

// Don't include ExampleResource in this header, just declare it here and
// include it in the .cpp file.
class ExampleResource;

/**
 * @brief  Accesses the example configuration file resource, providing an
 *         example of a typical Config::FileHandler class.
 */
class ExampleFile : private Config::FileHandler<ExampleResource>
{
public:
    ExampleFile();

    virtual ~ExampleFile() { }

    /**
     * @brief  Gets the file's example integer value.
     *
     * @return  The example file integer value.
     */
    int getExampleInt() const;

    /**
     * @brief  Gets the file's example string value.
     *
     * @return  The example file string value.
     */
    juce::String getExampleString() const;

    /**
     * @brief  Gets the file's example bool value.
     *
     * @return  The example file bool value.
     */
    bool getExampleBool() const;

    /**
     * @brief  Gets the file's example double value.
     *
     * @return  The example file double value.
     */
    double getExampleDouble() const;

    /**
     * @brief  Gets the file's example array value.
     *
     * @return  The example file array value.
     */
    juce::Array<int> getExampleArray() const;

    /**
     * @brief  Gets the file's example object value.
     *
     * @return  The example file object value.
     */
    ExampleObject getExampleObject() const;

    /**
     * @brief  Sets the file's example integer value.
     *
     * @param newInt  The new file integer value to set.
     */
    void setExampleInt(const int newInt);

    /**
     * @brief  Sets the file's example string value.
     *
     * @param newString  The new file string value to set.
     */
    void setExampleString(const juce::String newString);

    /**
     * @brief  Sets the file's example bool value.
     *
     * @param newBool  The new file bool value to set.
     */
    void setExampleBool(const bool newBool);

    /**
     * @brief  Sets the file's example double value.
     *
     * @param newDouble  The new file double value to set.
     */
    void setExampleDouble(const double newDouble);

    /**
     * @brief  Sets the file's example object value.
     *
     * @param newObject  The new object value to set.
     */
    void setExampleObject(const ExampleObject newObject);
};
