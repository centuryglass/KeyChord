#pragma once
/**
 * @file  Config_FileResource.h
 *
 * @brief  Reads and writes data from a JSON configuration file.
 */

#include "Config_ListenerInterface.h"
#include "SharedResource_Resource.h"
#include "SharedResource_Handler.h"
#include "Config_DataKey.h"
#include "Assets_JSONFile.h"
#include "JuceHeader.h"
#include <iostream>
#include <map>

namespace Config { class FileResource; }
namespace Config { struct DataKey; }

/**
 * @brief  Provides an abstract base for classes that read and write JSON
 *         configuration files.
 *
 *  Each FileResource subclass is responsible for a single JSON file containing
 * data that can be altered by the user. Along with reading and writing data,
 * FileResource allows objects to be defined as Listener objects, which will
 * receive notifications whenever data keys they select are changed.
 *
 *  As an implementation of the SharedResource class, each FileResource
 * subclass will have only one object instance at a time, to prevent concurrent
 * access to the JSON file, and limit the amount of file I/O necessary. Each
 * FileResource subclass should be accessed only through a ConfigFile subclass
 * specific to that FileResource.
 *
 *  A default version of each FileResource's JSON resource file should be
 * placed in the configuration subdirectory of the asset folder. Any missing or
 * invalid parameters in config files will be replaced with values from the
 * default file.
 *
 *  FileResource reads from each JSON file only once per program instance, so
 * any external changes to the file that occur while the program is running
 * will most likely be ignored and may be overwritten.
 */
class Config::FileResource : public SharedResource::Resource
{
protected:
    /**
     * @brief  Loads the resource's JSON data files.
     *
     * @param resourceKey     The SharedResource object key for the specific
     *                        FileResource variant being created.
     *
     * @param configFilename  The name of a JSON file to read or create in the
     *                        config file directory. There should be a file with
     *                        the same name in the asset folder filled with
     *                        default values.
     */
    FileResource(const juce::Identifier& resourceKey,
            const juce::String& configFilename);

public:
    /**
     * @brief  Writes any pending changes to the file before destruction.
     */
    virtual ~FileResource();

    /**
     * @brief  Gets one of the values stored in the JSON configuration file.
     *
     * @param key                       The key string that maps to the desired
     *                                  value.
     *
     * @tparam ValueType                The value's data type.
     *
     * @return                          The value read from the config file.
     */
    template<typename ValueType >
    ValueType getConfigValue(const juce::Identifier& key) const
    {
        if (!isValidKey(key))
        {
            DBG("Config::FileResource::" << __func__
                    << ": Attempted changing invalid key \""
                    << key.toString());
            jassertfalse;
            return ValueType();
        }
        try
        {
            return configJson.getProperty<ValueType>(key);
        }
        catch(Assets::JSONFile::TypeException e)
        {
            std::cerr << "Config::FileResource::" << __func__
                << ": Failed to load key \"" << e.getPropertyKey().toString()
                << "\" in file \"" << filename << "\", expected type: "
                << e.getExpectedType() <<", actual type: " << e.getFoundType()
                << ", exception = " << e.what() << "\n";
        }
        catch(Assets::JSONFile::FileException e)
        {
            DBG("Config::FileResource::" << __func__
                    << ": Failed to access file \"" << filename
                    << "\", exception = " << e.what());
        }
        return ValueType();
    }

    /**
     * @brief  Sets one of this FileResource's values, notifying listeners and
     *         writing to the JSON file if the value is changed.
     *
     * @param key               The key string that maps to the value being
     *                          updated.
     *
     * @param newValue          The new value to save to the file.
     *
     * @tparam ValueType        The value data type.
     *
     * @return                  True if the value changed, false if the new
     *                          value matched the old value.
     */
    template<typename ValueType>
    bool setConfigValue(const juce::Identifier& key, ValueType newValue)
    {
        if (!isValidKey(key))
        {
            DBG("Config::FileResource::" << __func__
                    << ": Attempted changing invalid key \""
                    << key.toString());
            jassertfalse;
            return false;
        }
        if (updateProperty<ValueType>(key, newValue))
        {
            configJson.writeChanges();
            int nListeners = 0;
            int nTracked = 0;
            foreachHandler<ListenerInterface>(
                [this, &key, &nListeners, & nTracked]
            (ListenerInterface* listener)
            {
                nListeners++;
                if (listener->isKeyTracked(key))
                {
                    nTracked++;
                    listener->configValueChanged(key);
                }
            });
            DBG("Config::FileResource::" << __func__ << ": Value with key \""
                    << key.toString() << "\" changed in file \"" << filename
                    << "\". Found " << nListeners
                    << " listener(s), and notified " << nTracked
                    << " listener(s) tracking that key.");
            return true;
        }
        return false;
    }

    /**
     * @brief  Sets a configuration data value back to its default setting.
     *
     *  If this changes the value, listeners will be notified and changes will
     * be saved.
     *
     * @param key  A key value defined in the config file.
     */
    virtual void restoreDefaultValue(const juce::Identifier& key);

    /**
     * @brief  Restores all values in the configuration file to their defaults.
     *
     *  All updated values will notify their Listeners and be written to the
     * JSON file.
     */
    virtual void restoreDefaultValues();

protected:
    /**
     * @brief  Loads all initial configuration data from the JSON config file.
     *
     *  This checks for all expected data keys, and replaces any missing or
     * invalid values with ones from the default config file. FileResource
     * subclasses should call this once, after they load any custom object or
     * array data.
     */
    void loadJSONData();

    /**
     * @brief  Checks if a key string is valid for this FileResource.
     *
     * @param key  A key string value to check.
     *
     * @return     Whether the key is valid for this file.
     */
    virtual bool isValidKey(const juce::Identifier& key) const;

    /**
     * @brief  Get the set of all basic(non-array, non-object) properties
     *         tracked by this FileResource.
     *
     * @return  The keys to all variables tracked in this config file.
     */
    virtual const std::vector<DataKey>& getConfigKeys() const = 0;

    /**
     * @brief  Checks for an expected property value in the JSON config data.
     *
     *  If the value is not found or has an invalid type, the property will be
     * copied from the default config file.
     *
     * @param key                       The key string of a parameter that
     *                                  should be defined in this config file.
     *
     * @tparam T                        The expected type of the JSON property.
     *
     * @return                          The initialized property value.
     */
    template<typename T> T initProperty(const juce::Identifier& key)
    {
        try
        {
            if (!configJson.propertyExists<T>(key))
            {
                DBG("Config::FileResource::" << __func__ << ": Key \""
                        << key.toString() << "\" not found in file \""
                        << filename << "\", checking default config file");
                if (!defaultJson.propertyExists<T>(key))
                {
                    DBG("Config::FileResource::" << __func__ << ": Key \""
                            << key.toString()
                            << "\" missing in config and default config!");
                }
                T property = defaultJson.getProperty<T>(key);
                configJson.setProperty<T>(key, property);
                return property;
            }
            return configJson.getProperty<T>(key);
        }
        catch(Assets::JSONFile::FileException e)
        {
            DBG("Config::FileResource::" << __func__
                    << ": Failed to access file \"" << filename
                    << "\" exception = " << e.what());
        }
        catch(Assets::JSONFile::TypeException e)
        {
            DBG("Config::FileResource::" << __func__
                    << ": Failed to load default value for \""
                    << e.getPropertyKey().toString() << "\" in file "
                    << filename <<"\", expected type: " << e.getExpectedType()
                    <<", actual type: "  << e.getFoundType()
                    << ", exception = " << e.what());
        }
        return T();
    }

    /**
     * @brief  Updates a property in the JSON config file.
     *
     *  This will not check key validity, immediately write any changes, or
     * notify listeners.
     *
     * @param key        The key string that maps to the value being updated.
     *
     * @param newValue   The new value to save to the file.
     *
     * @tparam T         The value data type.
     *
     * @return           True if JSON data changed, false if newValue was
     *                   identical to the old value with the same key.
     */
    template<typename T>
    bool updateProperty(const juce::Identifier& key, T newValue)
    {
        try
        {
            return configJson.setProperty<T>(key, newValue);
        }
        catch(Assets::JSONFile::FileException e)
        {
            DBG("Config::FileResource::" << __func__
                    << ": Failed to access file \"" << filename
                    << "\" exception = " << e.what());
        }
        catch(Assets::JSONFile::TypeException e)
        {
            DBG("Config::FileResource::" << __func__
                    << ": Failed to change value with key \""
                    << e.getPropertyKey().toString() << "\" in file "
                    << filename << "\", expected type: " << e.getExpectedType()
                    << ", actual type: "  << e.getFoundType()
                    << ", exception = " << e.what());
        }
        return false;
    }

    /**
     * @brief  Re-writes all data back to the config file, as long as there are
     *         changes to write.
     */
    void writeChanges();

private:
    /**
     * @brief  Sets a configuration data value back to its default setting,
     *         notifying listeners if the value changes.
     *
     *  This does not ensure that the key is valid for this ConfigFile. An
     * AlertWindow will be shown if any problems occur while accessing JSON
     * data.
     *
     * @param key  The key of the value that will be restored.
     */
    void restoreDefaultValue(const DataKey& key);

    /**
     * @brief  Writes any custom object or array data back to the JSON file.
     *
     *  FileResource subclasses with custom object or array data must override
     * this function to write that data back to the file.
     */
    virtual void writeDataToJSON() { }

    // The name of this JSON config file:
    const juce::String filename;

    // Configuration values read from the file:
    Assets::JSONFile configJson;

    // Default config file values:
    Assets::JSONFile defaultJson;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileResource)
};
