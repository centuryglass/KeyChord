#pragma once
/**
 * @file  Layout_Config_JSONResource.h
 *
 * @brief  Reads and writes component settings in the layout.json file.
 */

#include "Config_FileResource.h"
#include "Layout_Component_ConfigLayout.h"

namespace Layout { namespace Component { class JSONResource; } }

/**
 * @brief  A SharedResource class that loads and shares Component layouts loaded
 *         from the component layout configuration file.
 */
class Layout::Component::JSONResource : public Config::FileResource
{
public:
    // SharedResource object key
    static const juce::Identifier resourceKey;

    JSONResource();

    virtual ~JSONResource() { }

    /**
     * @brief  Gets the configured layout for a particular component.
     *
     * @param componentKey  A configurable UI component's key string.
     *
     * @return              The layout defined for that component.
     */
    ConfigLayout getLayout(const juce::Identifier& componentKey) const;

private:
    /**
     * @brief  Copies all ComponentSettings data back to the JSON file.
     */
    virtual void writeDataToJSON() final override;

    /**
     * @brief  Gets the key string and data type for each basic value stored
     *         in layout.json.
     *
     * @return  Key strings and data types for each bool, int, String, or
     *          double stored in components.json.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys()
        const final override;

    // Stores all component layouts loaded from the layout config file
    std::map<juce::Identifier, ConfigLayout> componentLayouts;
};
