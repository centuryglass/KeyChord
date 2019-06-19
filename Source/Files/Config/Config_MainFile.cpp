#include "Config_MainFile.h"
#define CONFIG_MAIN_IMPLEMENTATION
#include "Config_MainResource.h"
#include "Config_MainKeys.h"

Config::MainFile::MainFile() { }


// Checks if the application should be displayed in minimized mode.
bool Config::MainFile::getMinimized() const
{
    return getConfigValue<bool>(MainKeys::minimized);
}


// Gets whether the application is currently configured to snap to the bottom
// edge of the display.
bool Config::MainFile::getSnapToBottom() const
{
    return getConfigValue<bool>(MainKeys::snapToBottom);
}


// Gets whether the application is in immediate input mode.
bool Config::MainFile::getImmediateMode() const
{
    return getConfigValue<bool>(MainKeys::immediateMode);
}


// Sets whether the application should start up in minimized mode.
void Config::MainFile::setMinimised(const bool minimized)
{
    setConfigValue<bool>(MainKeys::minimized, minimized);
}


// Sets which edge of the display the window should snap to on construction.
void Config::MainFile::setSnapToBottom(const bool snapToBottom)
{
    setConfigValue<bool>(MainKeys::snapToBottom, snapToBottom);
}


// Sets whether the application should start in immediate mode.
void Config::MainFile::setImmediateMode(const bool immediateMode)
{
    setConfigValue<bool>(MainKeys::immediateMode, immediateMode);
}
