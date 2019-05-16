#pragma once
/**
 * @file  Locale.h
 *
 * @brief  Gets the system's selected locale.
 */

namespace juce { class String; }

namespace Locale
{
    /**
     * @brief  Gets the name of the system locale.
     *
     * @return   The short locale name, used for naming locale text files and
     *           identifying locale in .desktop files.
     */
    juce::String getLocaleName();

    /**
     * @brief  Gets a default locale to use when the system locale is undefined
     *         or unsupported.
     *
     * @return  The American English locale name, to use by default if more
     *          appropriate localization data is unavailable.
     */
    juce::String getDefaultLocale();
}
