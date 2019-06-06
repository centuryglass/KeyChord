#define INPUT_KEY_CONFIG_IMPLEMENTATION

#include "Input_Key_JSONResource.h"
#include "Input_Key_JSONKeys.h"
#include "Text_CharTypes.h"
#include "Text_Values.h"

// Filename of the JSON configuration file:
static const constexpr char * configFilename = "keyBindings.json";

// SharedResource object key:
const juce::Identifier Input::Key::JSONResource::resourceKey 
        = "Input_Key_JSONResource";

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Input::Key::JSONResource::";
#endif

// Loads the list of key bindings on construction.
Input::Key::JSONResource::JSONResource() : 
Config::FileResource(resourceKey, configFilename)
{
    using juce::var;
    using juce::String;
    using juce::Identifier;

    // Load all bindings:
    for (const Identifier* key : JSONKeys::allKeys)
    {
        var bindingInfo = initProperty<var>(*key);
        if (! bindingInfo.isObject())
        {
            DBG(dbgPrefix << __func__ << ": Warning: key " << key->toString()
                    << " did not store an object value.");
            continue;
        }
        // Binding inner value keys:
        static const Identifier keyValue("key");
        static const Identifier keyName("name");
        static const Identifier charName("charName");


        if (! bindingInfo.hasProperty(keyValue) 
                || ! bindingInfo[keyValue].isString() 
                || bindingInfo.operator String().isEmpty())
        {
            DBG(dbgPrefix << __func__ << ": Warning: key " << key->toString()
                    << " does not specify a valid binding.");
            continue;
        }
        const String description(bindingInfo[keyValue].operator String());
        const String displayName(bindingInfo.getProperty(keyName,
                description).operator String());
        const Text::CharValue displayChar(Text::Values::getCharValue(
                bindingInfo.getProperty(charName, displayName)
                .operator String()));
        keyBindings.add(new Binding(*key, description, displayName,
                    displayChar));
    }
    loadJSONData();
}


// Gets an object representing a particular key binding.
const Input::Key::Binding* Input::Key::JSONResource::getKeyBinding
(const juce::Identifier& keyID) const
{
    for (const Binding* binding : keyBindings)
    {
        if (binding->getActionID() == keyID)
        {
            return binding;
        }
    }
    static const Binding nullBinding;
    return &nullBinding;
}


// Gets the set of all basic(non-array, non-object) properties tracked by this
// Resource.
const std::vector<Config::DataKey>& Input::Key::JSONResource::getConfigKeys()
        const  
{
    static const std::vector<Config::DataKey> emptyList;
    return emptyList;
}


// Checks if a key string is valid for this FileResource.
bool Input::Key::JSONResource::isValidKey(const juce::Identifier& key) const 
{
    return JSONKeys::allKeys.contains(&key);
}
