#include "Layout_Component_JSONResource.h"
#include "Layout_Component_JSONKeys.h"


// SharedResource object key
const juce::Identifier Layout::Component::JSONResource::resourceKey
        = "Layout::Component::JSONResource";

// Filename of the JSON configuration file
static const constexpr char * configFilename = "layout.json";

Layout::Component::JSONResource::JSONResource() :
Config::FileResource(resourceKey, configFilename)
{
    loadJSONData();
}


// Gets the key string and data type for each basic value stored in
// components.json.
const std::vector<Config::DataKey>&
Layout::Component::JSONResource::getConfigKeys() const
{
    return Layout::Component::JSONKeys::basic;
}
