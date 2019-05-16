#include "Layout_Component_JSONResource.h"
#include "Layout_Component_JSONKeys.h"

namespace ComponentLayout = Layout::Component;
namespace ComponentKeys = Layout::Component::JSONKeys;

// SharedResource object key
const juce::Identifier ComponentLayout::JSONResource::resourceKey
        = "Layout::Component::JSONResource";

// Filename of the JSON configuration file
static const constexpr char * configFilename = "layout.json";

ComponentLayout::JSONResource::JSONResource() :
Config::FileResource(resourceKey, configFilename)
{
    using juce::DynamicObject;
    using juce::Identifier;
    const juce::Array<Identifier> keys = ComponentKeys::components;
    for (const Identifier& key : keys)
    {
        DynamicObject::Ptr componentData = initProperty<DynamicObject*>(key);
        componentLayouts[key] = ConfigLayout(componentData);
    }
    loadJSONData();
}


// Gets the configured layout for a particular component.
ComponentLayout::ConfigLayout ComponentLayout::JSONResource::getLayout
(const juce::Identifier& componentKey) const
{
    return componentLayouts.at(componentKey);
}


// Copy all ComponentSettings data back to the JSON file.
void ComponentLayout::JSONResource::writeDataToJSON()
{
    using juce::DynamicObject;
    using juce::Identifier;
    const juce::Array<Identifier>& keys = ComponentKeys::components;
    for (const Identifier& key : keys)
    {
        if (componentLayouts.count(key) != 0)
        {
            updateProperty<DynamicObject*>(key,
                    componentLayouts[key].getDynamicObject());
        }
    }
}


// Gets the key string and data type for each basic value stored in
// components.json.
const std::vector<Config::DataKey>&
ComponentLayout::JSONResource::getConfigKeys() const
{
    return ComponentKeys::basic;
}
