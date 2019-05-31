#pragma once
/**
 * @file  Input_Key_ConfigFile.h
 *
 * @brief  Loads key bindings from the key configuration resource.
 */

#include "Config_FileHandler.h"
#include "JuceHeader.h"

namespace Input
{
    namespace Key
    {
        class ConfigFile;
        class JSONResource;
        class Binding;
    }
}

class Input::Key::ConfigFile : public Config::FileHandler<JSONResource>
{
public:
    ConfigFile();

    virtual ~ConfigFile() { }

    /**
     * @brief  Gets the JUCE KeyPress bound to a particular action.
     *
     * @param bindingID  An Input::Key::JSONKeys ID value associated with the 
     *                   key binding.
     *
     * @return           A KeyPress object that may be used to detect if the
     *                   bound key is held down.
     */
    juce::KeyPress getBoundKey(const juce::Identifier& bindingID) const;

    /**
     * @brief  Gets a localized description of this key's action.
     *
     * @param bindingID  An Input::Key::JSONKeys ID value associated with the 
     *                   key binding.
     *
     * @return           The localized description text.
     */
    juce::String getActionDescription(const juce::Identifier& bindingID) const;

    /**
     * @brief  Gets the name that should be used to describe the bound key.
     *
     * @param bindingID  An Input::Key::JSONKeys ID value associated with the 
     *                   key binding.
     *
     * @return           The key name defined in the configuration file.
     */
    juce::String getKeyName(const juce::Identifier& bindingID) const;

    /**
     * @brief  Gets a single key character that may be used to represent the
     *         bound key.
     *
     * @param bindingID  An Input::Key::JSONKeys ID value associated with the 
     *                   key binding.
     *
     * @return           The key character defined in the configuration file.
     */
    unsigned int getKeyChar(const juce::Identifier& bindingID) const;

private:
    /**
     * @brief  Loads all key binding information for an action from the
     *         configuration file.
     *
     * @param bindingID  An Input::Key::JSONKeys ID value associated with the 
     *                   key binding.
     *
     * @return           A pointer to the key binding data object.
     */
    const Binding* getKeyBinding(const juce::Identifier& bindingID) const;
};
