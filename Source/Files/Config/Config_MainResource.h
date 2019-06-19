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

namespace Config { class MainResource; }

class Config::MainResource : public Config::FileResource
{
public:
    // SharedResource object instance key:
    static const juce::Identifier resourceKey;

    MainResource();

    virtual ~MainResource();

private:
    /**
     * @brief  Gets the set of all basic(non-array, non-object) properties
     *         tracked by this Resource.
     *
     * @return  All specific string values stored by this resouve
     */
    const std::vector<Config::DataKey>& getConfigKeys() const final override;
};
