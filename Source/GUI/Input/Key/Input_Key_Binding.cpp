#include "Input_Key_Binding.h"
#include "Input_Key_JSONKeys.h"
#include "Text_CharSet_ConfigFile.h"


// Localized text object key:
static const juce::Identifier localeKey("Input_Key_Binding");
// Special character set selection locale keys:
static const juce::Identifier charSetPreKey("Select charSet pre");
static const juce::Identifier charSetPostKey("Select charSet post");


// Immutably sets all key binding information on construction.
Input::Key::Binding::Binding(
        const juce::Identifier& jsonKey,
        const juce::String keyDescription,
        const juce::String keyName,
        const Text::CharValue charName) :
    jsonKey(jsonKey),
    keyDescription(keyDescription),
    keyName(keyName),
    charName(charName),
    Locale::TextUser(localeKey) { }


// Constructs an invalid Binding with no stored data.
Input::Key::Binding::Binding() :
    Binding(juce::Identifier::null, "", "", 0) { }


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
    using std::map;
    using std::function;
    using juce::Identifier;
    using juce::String;

    // Because the user can reassign and rename character sets freely (except
    // the modifier set), localized text for selecting a character set is loaded
    // differently.
    static const map<const Identifier, Text::CharSet::Type> setMap =
    {
        { JSONKeys::selectMainSet,    Text::CharSet::Type::main },
        { JSONKeys::selectAltSet,     Text::CharSet::Type::alt },
        { JSONKeys::selectSpecialSet, Text::CharSet::Type::special },
    };
    try
    {
        const Text::CharSet::Type setType = setMap.at(jsonKey);
        Text::CharSet::ConfigFile charsetConfig;
        return localeText(charSetPreKey) 
                + charsetConfig.getSetName(setType)
                + localeText(charSetPostKey);
    }
    catch(const std::out_of_range& e)
    {
        return localeText(jsonKey);
    }
}


// Gets the name used to describe the bound key to the user.
const juce::String& Input::Key::Binding::getKeyName() const
{
    return keyName;
}


// Gets the character value used to represent this key binding.
const Text::CharValue Input::Key::Binding::getCharName() const
{
    return charName;
}
