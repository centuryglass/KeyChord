#include "Assets_XDGDirectories.h"
#include <cstdlib>

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Assets::XDGDirectories::";
#endif

//###############   XDG Path environment variables   ##########################
namespace EnvVariables
{
    // Stores the default user data directory:
    static const constexpr char* dataDir     = "XDG_DATA_HOME";
    // Stores the default user config directory:
    static const constexpr char* configDir   = "XDG_CONFIG_HOME";
    // Stores the default user cache directory:
    static const constexpr char* cacheDir    = "XDG_CACHE_HOME";
    // Stores the default user runtime file directory:
    static const constexpr char* runtimeDir  = "XDG_RUNTIME_DIR";
    // Stores the list of paths to search for user data:
    static const constexpr char* dataPaths   = "XDG_DATA_DIRS";
    // Stores the list of paths to search for user config files:
    static const constexpr char* configPaths = "XDG_CONFIG_DIRS";
}


//######################   Default Path Values   ##############################
// Default paths to use if the XDG environment variables are not set.
namespace DefaultDirs
{
    // Default user data directory, relative to $HOME:
    static const juce::String dataDir    = "/.local/share";
    // Default user config directory, relative to $HOME:
    static const juce::String configDir  = "/.config";
    // Default user cache directory, relative to $HOME:
    static const juce::String cacheDir   = "/.cache";
    // Default data paths to search after DataDir:
    static const juce::String dataPaths  = "/usr/local/share:/usr/share";
    // Default config paths to search after ConfigDir:
    static const juce::String configPaths = "/etc/xdg";
}

/**
 * @brief  Returns a string from an environment variable, or a default string
 *         if the environment variable is unset or empty.
 *
 * @param envVar      The environment variable to get.
 *
 * @param defaultStr  The alternate default string.
 *
 * @return            The environment variable, or the default string if the
 *                    variable isn't found.
 */
static juce::String getEnvOrDefaultString
(const char* envVar, const juce::String defaultStr)
{
    using juce::String;
    const char* envString = std::getenv(envVar);
    if (envString == nullptr || envString[0] == '\0')
    {
        return String(defaultStr);
    }
    return String(envString);
}

/**
 * @brief  Gets the user's home directory.
 *
 * @return  The user's home directory path.
 */
static juce::String homePath()
{
    return juce::String(getenv("HOME"));
}


// Gets the path of the single base directory where user-specific data files
// should be written.
juce::String Assets::XDGDirectories::getUserDataPath()
{
    return getEnvOrDefaultString(EnvVariables::dataDir,
            homePath() + DefaultDirs::dataDir);
}


// Gets the path of the single base directory where user-specific configuration
// files should be written.
juce::String Assets::XDGDirectories::getUserConfigPath()
{
    return getEnvOrDefaultString(EnvVariables::configDir,
            homePath() + DefaultDirs::configDir);
}


// Gets the path of the single base directory where user-specific cache files
// should be written.
juce::String Assets::XDGDirectories::getUserCachePath()
{
    return getEnvOrDefaultString(EnvVariables::cacheDir,
            homePath() + DefaultDirs::cacheDir);
}


// Gets the path of the single base directory where user-specific runtime files
// should be written.
juce::String Assets::XDGDirectories::getUserRuntimePath()
{
    using juce::String;
    String runtimePath = getEnvOrDefaultString(EnvVariables::runtimeDir, "");
    if (runtimePath.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": " << EnvVariables::runtimeDir
                << " is not defined!");
        jassertfalse;
    }
    return runtimePath;
}


// Gets the ordered list of directories to search for user data files.
juce::StringArray Assets::XDGDirectories::getDataSearchPaths()
{
    juce::String paths = getUserDataPath() + ":";
    paths += getEnvOrDefaultString(EnvVariables::dataPaths,
            DefaultDirs::dataPaths);
    return juce::StringArray::fromTokens(paths, ":", "");
}


// Gets the ordered list of directories to search for user configuration files.
juce::StringArray Assets::XDGDirectories::getConfigSearchPaths()
{
    juce::String paths = getUserConfigPath() + ":";
    paths += getEnvOrDefaultString(EnvVariables::configPaths,
            DefaultDirs::configPaths);
    return juce::StringArray::fromTokens(paths, ":", "");
}
