#include "Assets.h"
#include "Locale_TextUser.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Locale::TextUser::";
#endif

// Localized text data loaded from a JSON locale file:
std::map<juce::Identifier, std::map<juce::Identifier, juce::String>>
Locale::TextUser::localeData;

// The default POSIX locale, returned by the system when no locale is set:
static const juce::String unsetLocale = "C";

// The directory in the assets folder where locale files are placed:
static const juce::String localeDir = "locale/";

// The file extension shared by locale files:
static const juce::String fileExtension = ".json";


// Initializes all localized text data.
Locale::TextUser::TextUser(const juce::Identifier& className) :
className(className)
{
    if (!localeData.empty()) // Already loaded data, skip initialization.
    {
        return;
    }
    using juce::String;
    using juce::NamedValueSet;
    const juce::StringArray filesToTry =
    {
        getLocaleName(),
        getDefaultLocale()
    };

    juce::var jsonData;
    for (const String& localeName : filesToTry)
    {
        if (localeName.isEmpty() || localeName == unsetLocale)
        {
            continue;
        }
        jsonData = Assets::loadJSONAsset(localeDir + localeName
                + fileExtension, false);
        if (!jsonData.isVoid())
        {
            break;
        }
    }
    if (!jsonData.isObject())
    {
        // There should always be some locale file loaded, even if it's just
        // the default en_US file.
        jassertfalse;
        return;
    }
    NamedValueSet& textGroups = jsonData.getDynamicObject()->getProperties();
    for (auto group = textGroups.begin(); group != textGroups.end(); group++)
    {
        if (!group->value.isObject())
        {
            DBG(dbgPrefix << __func__ << ": no data found in group "
                    << group->name);
            continue;
        }
        NamedValueSet& groupStrings
            = group->value.getDynamicObject()->getProperties();
        for (auto textValue = groupStrings.begin();
                textValue != groupStrings.end(); textValue++)
        {
            localeData[group->name][textValue->name]
                = textValue->value.toString();
        }
    }
}


// Looks up a localized text string associated with this class.
juce::String Locale::TextUser::localeText
(const juce::Identifier& textKey) const
{
    if (!localeData.count(className))
    {
        DBG(dbgPrefix << __func__ << ": Couldn't find text values "
                << "for TextUser with key \"" << className.toString() << "\"");
        return juce::String();
    }
    std::map<juce::Identifier, juce::String>& userStrings
        = localeData[className];
    if (!userStrings.count(textKey))
    {
        DBG(dbgPrefix << __func__ << ": Couldn't find text value \""
                << textKey.toString() << "\" for TextUser with key \""
                << className.toString() << "\"");
        return juce::String();
    }
    return userStrings[textKey];
}
