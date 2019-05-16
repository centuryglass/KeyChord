#pragma once
/**
 * @file  Theme_Colour_JSONResource.h
 *
 * @brief  Reads and writes UI colours defined in the configurable colours.json
 *         file.
 */

#include "Config_FileResource.h"
#include "Config_Listener.h"
#include "Theme_Colour_ListenerInterface.h"

namespace Theme { namespace Colour { class JSONResource; } }

class Theme::Colour::JSONResource : public Config::FileResource
{
public:
    // SharedResource object key:
    static const juce::Identifier resourceKey;

    JSONResource();

    virtual ~JSONResource() { }

private:
    /**
     * @brief  Gets the set of all basic(non-array, non-object) properties
     *         tracked by this JSON resource.
     *
     * @return  The keys to all variables tracked in this config file.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys()
        const final override;

    /**
     * @brief  Checks if a single handler object is a Listener tracking updates
     *         of a single key value, and if so, notifies it that the tracked
     *         value has updated.
     *
     * @param listener  A Listener object attached to the JSON resource.
     *
     * @param key       The key to an updated configuration value.
     */
    void notifyListener(ListenerInterface* listener,
            const juce::Identifier& key);
};

