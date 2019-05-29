#include "Input_Key_ConfigFile.h"
#define INPUT_KEY_CONFIG_IMPLEMENTATION

#include "Input_Key_JSONResource.h"
#include "Input_Key_Binding.h"

Input::Key::ConfigFile::ConfigFile() { }


// Gets the JUCE KeyPress bound to a particular action.
juce::KeyPress Input::Key::ConfigFile::getBoundKey
(const juce::Identifier& bindingID) const
{
    return getKeyBinding(bindingID)->getKeyPress();
}


// Gets a localized description of this key's action.
juce::String Input::Key::ConfigFile::getActionDescription
(const juce::Identifier& bindingID)
        const
{
    return getKeyBinding(bindingID)->getActionDescription();
}


// Gets the name that should be used to describe the bound key.
juce::String Input::Key::ConfigFile::getKeyName
(const juce::Identifier& bindingID)
        const
{
    return getKeyBinding(bindingID)->getKeyName();
}


// Gets a single key character that may be used to represent the bound key.
juce::juce_wchar Input::Key::ConfigFile::getKeyChar
(const juce::Identifier& bindingID)
        const
{
    return getKeyBinding(bindingID)->getCharName();
}

/*
 * Loads all key binding information for an action from the configuration file.
 */
const Input::Key::Binding* Input::Key::ConfigFile::getKeyBinding
(const juce::Identifier& bindingID) const
{
    SharedResource::LockedPtr<const JSONResource> jsonResource
            = getReadLockedResource();
    return jsonResource->getKeyBinding(bindingID);
}
