#pragma once
#ifndef INPUT_KEY_CONFIG_IMPLEMENTATION
    #error "Private JSONResource included outside of Key::ConfigFile!"
#endif
/**
 * @file  Input_Key_JSONResource.h
 *
 * @brief  Controls access to the keyBindings.json configuration file.
 */

#include "Input_Key_Binding.h"
#include "Config_FileResource.h"
#include "JuceHeader.h"

namespace Input { namespace Key { class JSONResource; } }

class Input::Key::JSONResource : public Config::FileResource
{
public:
    // SharedResource object key:
    static const juce::Identifier resourceKey;

    /**
     * @brief  Loads the list of key bindings on construction.
     */
    JSONResource();

    virtual ~JSONResource() { }

    /**
     * @brief  Gets an object representing a particular key binding.
     *
     * @param keyID  An identifier selecting a specific keyboard action.
     *
     * @return       Stored data about that action and its bound key.
     */
    const Binding* getKeyBinding(const juce::Identifier& keyID) const;

private:
    /**
     * @brief  Gets the set of all basic(non-array, non-object) properties
     *         tracked by this Resource.
     *
     * @return  An empty list, as all stored values are custom Binding objects.
     */
    const std::vector<Config::DataKey>& getConfigKeys() const final override;

    /**
     * @brief  Checks if a key string is valid for this FileResource.
     *
     * @param key  A key string value to check.
     *
     * @return     Whether the key is a valid action ID.
     */
    bool isValidKey(const juce::Identifier& key) const override;

    // Map each binding ID to its stored values:
    juce::OwnedArray<Binding> keyBindings;
};
