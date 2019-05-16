#pragma once
/**
 * @file  Assets_JSONFile.h
 *
 * @brief  Provides a simplified interface for reading and writing JSON file
 *         data.
 */
#include <exception>
#include "JuceHeader.h"

namespace Assets { class JSONFile; }

/**
 * @brief  Represents a single JSON asset file.
 *
 *  Each JSONFile object is assigned a single file, specified as a file path
 * value. This file should either not yet exist, or be a .json text file,
 * containing a single JSON object. On construction, the JSONFile object copies
 * all data from its JSON file, or creates the file if it does not yet exist.
 * Property values can then be read from the file with the getProperty function
 * or written to the file with the setProperty function.
 *
 *  All getProperty and setProperty calls perform type checking. Both functions
 * take a JSON key and an expected type template parameter, and throw
 * exceptions of type JSONFile::TypeException if data is stored in the file at
 * that key that does not match the expected type. If it is absolutely
 * necessary for a key to be able to hold multiple valid types, getProperty and
 * setProperty can read and write values as juce::var objects, which will
 * accept any valid JSON data type.
 *
 *  JSONFile is primarily intended to simplify file operations in ConfigFile
 * classes, and to help detect abnormalities and type errors within the JSON
 * configuration files used by those classes. In situations where JSON files
 * can be trusted to only contain valid data, or where they do not need to be
 * altered, using juce::JSON, juce::File, and juce::var may be more appropriate.
 */
class Assets::JSONFile
{
public:
    /**
     * @brief  Creates a JSON data file interface, creating a new JSON file or
     *         reading an existing JSON file's data.
     *
     * @param filePath        The path to a JSON file. This may be a path
     *                        relative to the application's assets folder, an
     *                        absolute path, or a path relative to the current
     *                        working directory.
     *
     * @throws FileException  If the file could not be read, or could not be
     *                        created.
     */
    JSONFile(const juce::String filePath);

    /**
     * @brief  Constructs the JSONFile object with no actual file.
     */
    JSONFile();

    /**
     * @brief  Checks if this object has a valid JSON file it controls.
     *
     * @return  Whether the object is managing a valid file.
     */
    bool isValidFile() const;

    /**
     * @brief  Saves all changes back to the source file if applicable.
     */
    virtual ~JSONFile();

    /**
     * @brief  Gets a property stored in the JSON data.
     *
     * @param key             The key string identifying the property.
     *
     * @tparam T              The type of value to return.
     *
     * @return                The value extracted from the JSON data.
     *
     * @throws TypeException  If no property with type T was found.
     */
    template<typename T > T getProperty(const juce::Identifier& key) const
    {
        jassert(!jsonData.isVoid());
        if (!jsonData.hasProperty(key))
        {
            throw TypeException(getTypeName<T>(), "void", key);
        }

        juce::var property = jsonData[key];
        typeCheck<T>(property, key);
        return extractProperty<T>(property);
    }

    /**
     * @brief  Sets a property stored in this JSON data.
     *
     * @param key             The key string used to set the value.
     *
     * @param newValue        A new value to store in the JSON object.
     *
     * @tparam T              The type of value being stored.
     *
     * @return                True if the property changed, false if the
     *                        existing value matched the new value.
     *
     * @throws TypeException  If a property exists that shares this key but is
     *                        not of type T.
     */
    template<typename T > bool setProperty
    (const juce::Identifier& key, T newValue)
    {
        using juce::var;
        jassert(!jsonData.isVoid());
        var newProperty(newValue);
        if (jsonData.hasProperty(key))
        {
            var oldProperty = jsonData[key];
            if (oldProperty == newProperty)
            {
                return false;
            }
            typeCheck<T>(oldProperty, key);
        }
        jsonData.getDynamicObject()->setProperty(key, var(newValue));
        unwrittenChanges = true;
        return true;
    }

    /**
     * @brief  Checks if JSON data contains data of a specific type assigned to
     *         a specific key.
     *
     * @param key  The key value to search for in the JSON data.
     *
     * @tparam T   The expected type of the value stored in the JSON data
     *
     * @return     Whether a value of type T exists at the given key.
     */
    template<typename T> bool propertyExists(const juce::Identifier& key) const
    {
        using juce::var;
        if (jsonData.isVoid())
        {
            return false;
        }
        if (!jsonData.hasProperty(key))
        {
            return false;
        }
        if (getTypeName<T>() == getTypeName<var>())
        {
            return !jsonData.getProperty(key, var()).isVoid();
        }
        return getTypeName<T>() == getTypeName
                (jsonData.getProperty(key, var()));
    }

    /**
     * @brief  Rewrites all data back to the config file, as long as there are
     *         changes to write.
     *
     * @throws FileException  If changes could not be written to the file.
     */
    void writeChanges();

    /**
     * @brief  Signals a failure to read from or write to the JSON config file.
     */
    struct FileException : public std::exception
    {
    public:
        /**
         * @brief  Creates a file exception representing a file IO error.
         *
         * @param filePath       The path of the file that couldn't be accessed.
         *
         * @param errorMessage   A short error message describing the failure.
         */
        FileException
        (const juce::String filePath, const juce::String errorMessage) :
        filePath(filePath), errorMessage(errorMessage) { }

        /**
         * @brief  Gets the path of the file that couldn't be accessed.
         *
         * @return  The JSON file's path.
         */
        juce::String getFilePath() const
        {
            return filePath;
        }

        /**
         * @brief  Gets an error message explaining the reason for the
         *         exception.
         *
         * @return  A short error message.
         */
        virtual const char* what() const noexcept override
        {
            return errorMessage.toRawUTF8();
        }

    private:
        const juce::String filePath;
        const juce::String errorMessage;
    };


    /**
     * @brief  Signals that a JSON object property did not have the expected
     *         type.
     */
    struct TypeException : public std::exception
    {
    public:
        /**
         * @brief  Creates a type exception for an unexpected type found in the
         *         file.
         *
         * @param expectedType  The name of the expected property type.
         *
         * @param foundType     The name of the actual property type.
         *
         * @param key           The key string of the property that caused the
         *                      error.
         */
        TypeException(const juce::String expectedType,
                const juce::String foundType, const juce::Identifier key) :
                expectedType(expectedType),
                foundType(foundType),
                key(key) { }

        /**
         * @brief  Gets the name of the expected type of the JSON property.
         *
         * @return  A string describing the expected property type.
         */
        juce::String getExpectedType() const
        {
            return expectedType;
        }

        /**
         * @brief  Gets the name of the actual type of the JSON property.
         *
         * @return  A string describing the actual property type.
         */
        juce::String getFoundType() const
        {
            return foundType;
        }

        /**
         * @brief  Gets the key string of the JSON property responsible for the
         *         exception
         *
         * @return  The property key string.
         */
        juce::Identifier getPropertyKey() const
        {
            return key;
        }

        /**
         * @brief  Gets a short error message suitable for debug output only.
         */
        virtual const char* what() const noexcept override
        {
            juce::String error = "Key \"";
            error += key;
            error += "\" Expected: ";
            error += expectedType;
            error += ", Found:";
            error += foundType;
            return error.toRawUTF8();
        }

    private:
        const juce::String expectedType;
        const juce::String foundType;
        const juce::Identifier key;
    };

private:
    /**
     * @brief  Checks if the type of a property matches the expected type,
     *         throwing an exception if they do not match.
     *
     * @param property        A property read from a JSON file.
     *
     * @param key             The property key string.
     *
     * @tparam T              The expected type of the value stored in the
     *                        property var.
     *
     * @throws TypeException  If the value stored in the property var is not of
     *                        type T.
     */
    template<typename T > void typeCheck
    (const juce::var& property, const juce::Identifier& key) const
    {
        using juce::String;
        String expected = getTypeName<T>();
        String found = getTypeName(property);
        if (expected != getTypeName<juce::var>() && expected != found)
        {
            throw TypeException(expected, found, key);
        }
    }

    /**
     * @brief  Removes and returns a value of type T from a var container.
     *
     *  The caller is responsible for ensuring that the container definitely
     * contains a value of type T.
     *
     * @param container  A JSON property read from the JSON file.
     *
     * @tparam T         The type of object stored in the container.
     *
     * @return           The extracted property value of type T.
     */
    template<typename T> T extractProperty(juce::var& container) const;

    /**
     * @brief  Gets the name of the data type held in the JSON var object.
     *
     * @param property  A var object that may or may not contain data.
     *
     * @return          A string describing the data in the property var.
     */
    juce::String getTypeName(const juce::var& property) const;

    /**
     * @brief  Gets the name of a data type that may be contained in a JSON var
     *         object.
     *
     * @tparam T  The type name to get.
     *
     * @return    The type's descriptive name string.
     */
    template<typename T >juce::String getTypeName() const;

private:
    // The path used to load this JSON file:
    juce::String filePath;

    // All JSON object data loaded from the file:
    juce::var jsonData;

    // Whether this object contains unsaved changes that need to be written to
    // the source file:
    bool unwrittenChanges = false;
};
