#pragma once
/**
 * @file  Assets_XDGDirectories.h
 *
 * @brief  Finds and prioritizes directory paths following the XDG Base
 *         Directory Specification.
 *
 * @see https://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
 */

#include "JuceHeader.h"

namespace Assets
{
    namespace XDGDirectories
    {
        /**
         * @brief  Gets the path of the single base directory where
         *         user-specific data files should be written.
         *
         * @return  The path to the user's data directory.
         */
        juce::String getUserDataPath();

        /**
         * @brief  Gets the path of the single base directory where
         *         user-specific configuration files should be written.
         *
         * @return  The path to the user's configuration directory.
         */
        juce::String getUserConfigPath();

        /**
         * @brief  Gets the path of the single base directory where
         *         user-specific cache files should be written.
         *
         * @return  The path to the user's cache directory.
         */
        juce::String getUserCachePath();

        /**
         * @brief  Gets the path of the single base directory where
         *         user-specific runtime files should be written.
         *
         * @return  The path to the user's runtime file directory.
         */
        juce::String getUserRuntimePath();

        /**
         * @brief  Gets the ordered list of directories to search for user data
         *         files.
         *
         * @return  The list of directories to search for data files, from
         *          highest priority to lowest.
         */
        juce::StringArray getDataSearchPaths();

        /**
         * @brief  Gets the ordered list of directories to search for user
         *         configuration files.
         *
         * @return  The list of directories to search for configuration files,
         *          from highest priority to lowest.
         */
        juce::StringArray getConfigSearchPaths();
    }
}
