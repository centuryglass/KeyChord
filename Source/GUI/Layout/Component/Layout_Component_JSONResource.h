#pragma once
/**
 * @file  Layout_Config_JSONResource.h
 *
 * @brief  Reads and writes component settings in the layout.json file.
 */

#include "Config_FileResource.h"

namespace Layout { namespace Component { class JSONResource; } }

class Layout::Component::JSONResource : public Config::FileResource
{
public:
    // SharedResource object key
    static const juce::Identifier resourceKey;

    JSONResource();

    virtual ~JSONResource() { }

private:
    /**
     * @brief  Gets the key string and data type for each basic value stored
     *         in layout.json.
     *
     * @return  Key strings and data types for each bool, int, String, or
     *          double stored in components.json.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys()
        const final override;
};
