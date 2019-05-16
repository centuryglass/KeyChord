#pragma once
/**
 * @file  Config_AlertWindow.h
 *
 * @brief  Displays pop-up error messages when something goes wrong with reading
 *         or writing JSON configuration files.
 */

// TODO: Localized error messages for:
//      -Bad permissions on config files
//      -Inability to create new files
//      -Inappropriate data types in config files
//      -Missing default config files

#include "Locale_TextUser.h"
#include "JuceHeader.h"

namespace Config { class AlertWindow; }

/**
 * @brief  Alerts the user to a problem with reading or writing configuration
 *         files.
 */
class Config::AlertWindow : private Locale::TextUser
{
public:
    AlertWindow() : Locale::TextUser("ConfigAlertWindows") { }

    virtual ~AlertWindow() { }

    /**
     * @brief  Shows a simple alert window with error text.
     *
     * This should be replaced when ConfigErrorMessage is properly implemented.
     */
    void showPlaceholderError(juce::String errorMessage)
    {
        juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::AlertIconType::WarningIcon,
                "ConfigFile Error",
                errorMessage);
    }
};
