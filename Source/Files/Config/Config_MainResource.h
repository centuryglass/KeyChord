#pragma once
#ifndef CONFIG_MAIN_IMPLEMENTATION
    #error Config_MainResource.h included outside of implementation files!
#endif
/**
 * @file  Config_MainResource.h
 *
 * @brief  Reads from and writes to the main configuration file.
 */

#include "Config_FileResource.h"
#include "JuceHeader.h"
#include "Text_CharTypes.h"

namespace Config { class MainResource; }

class Config::MainResource : public Config::FileResource
{
public:
    // SharedResource object instance key:
    static const juce::Identifier resourceKey;

    MainResource();

    virtual ~MainResource();
    
    /**
     * @brief  Loads the cached input buffer stored when restarting the 
     *         application.  Doing this removes this cached data from the file.
     *
     * @return   The cached buffer text.
     */
    Text::CharString takeCachedBuffer();

    /**
     * @brief  Saves the text input buffer to the configuration file so it can
     *         be restored after an immediate restart.
     *
     * @param inputBuffer  The buffer of pending input text.
     */
    void cacheInputBuffer(const Text::CharString& inputBuffer);

private:
    /**
     * @brief  Gets the set of all basic(non-array, non-object) properties
     *         tracked by this Resource.
     *
     * @return  All specific string values stored by this resouve
     */
    const std::vector<Config::DataKey>& getConfigKeys() const final override;

    /**
     * @brief  Checks if a key string is valid for this FileResource.
     *
     * @param key  A key string value to check.
     *
     * @return     Whether the key is a valid string value or character set.
     */
    bool isValidKey(const juce::Identifier& key) const override;

    /**
     * @brief  Writes input buffer data to the JSON file.
     */
    void writeDataToJSON() override;

    // Cached input buffer text:
    Text::CharString inputCache;
};
