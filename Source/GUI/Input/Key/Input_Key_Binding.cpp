#include "Input_Key_Binding.h"

/* Localized text object key: */
static const juce::Identifier localeKey("Input_Key_Binding");


// Immutably sets all key binding information on construction.
Input::Key::Binding::Binding(
        const juce::Identifier& jsonKey,
        const juce::String keyDescription,
        const juce::String keyName,
        const unsigned int charName) :
    jsonKey(jsonKey),
    keyDescription(keyDescription),
    keyName(keyName),
    charName(charName),
    Locale::TextUser(localeKey) { }


// Constructs an invalid Binding with no stored data.
Input::Key::Binding::Binding() :
    Binding(juce::Identifier::null, "", "", 0) { }

// Checks if this key action does not have a valid bound key value.
bool Input::Key::Binding::unBound() const
{
    return keyDescription.isNotEmpty() && getKeyPress().isValid();
}


// Gets the JUCE::KeyPress object for this binding's keyboard key.
juce::KeyPress Input::Key::Binding::getKeyPress() const
{
    return juce::KeyPress::createFromDescription(keyDescription);
}


// Gets the identifier used to select this binding's assigned action.
const juce::Identifier& Input::Key::Binding::getActionID() const
{
    return jsonKey;
}


// Gets the localized description of this binding's action.
juce::String Input::Key::Binding::getActionDescription() const
{
    return localeText(jsonKey);
}


// Gets a name used to describe the bound key to the user.
const juce::String& Input::Key::Binding::getKeyName() const
{
    return keyName;
}


// Gets a character used to represent this key.
const unsigned int Input::Key::Binding::getCharName() const
{
    return charName;
}
