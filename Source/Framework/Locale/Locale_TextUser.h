#pragma once
/**
 * @file  Locale_TextUser.h
 *
 * @brief  Provides access to all localized text strings by class.
 */

#include "Locale/Locale.h"
#include "JuceHeader.h"
#include <map>

namespace Locale { class TextUser; }

/**
 * @brief  Loads a set of localized strings from an appropriate locale file.
 */
class Locale::TextUser
{
public:
    /**
     * @brief  Creates a TextUser, loading locale data if necessary.
     *
     * @param className    Identifies the text values for a particular class in
     *                     the localization file. Only text values under this
     *                     name will be accessed.
     */
    TextUser(const juce::Identifier& className);

    virtual ~TextUser() { }

protected:
    /**
     * @brief  Looks up a localized text string associated with this class.
     *
     * @param key  One of this object's text keys.
     *
     * @return     The localized text string, or the empty string if text
     *             wasn't found.
     */
    juce::String localeText(const juce::Identifier& key) const;

private:
    // The key to all localized strings that belong to this class:
    const juce::Identifier& className;

    // All localized text data loaded from the JSON locale file:
    static std::map<juce::Identifier,
        std::map<juce::Identifier, juce::String>> localeData;
};
