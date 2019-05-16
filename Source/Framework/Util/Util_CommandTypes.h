#pragma once
/**
 * @file  Util_CommandTypes.h
 *
 * @brief  Defines all system commands the Util::Commands class can run.
 */

namespace Util
{
    namespace CommandTypes
    {
        /**
         * @brief  Lists all system commands that run asynchronously without
         *         returning a value.
         */
        enum class Action
        {
            // Shutdown the system
            shutdown,
            // Restart the system
            restart,
            // Enter sleep mode
            sleep,
            // Exit sleep mode
            wake,
            // Calibrate the touch screen
            calibrate,
            // Sets the system volume percentage
            setVolume,
            // Sets the display brightness percentage
            setBrightness
        };

        /**
         * @brief  Lists all system commands that run until termination and
         *         return their integer exit code.
         */
        enum class Int
        {
            // Returns 0 if and only if the system is sleeping
            sleepCheck,
            // Returns 0 if and only if a command is valid
            commandCheck
        };

        /**
         * @brief  Lists all system commands that run until termination and
         *         return their text output.
         */
        enum class Text
        {
            // Prints the system volume percentage
            getVolume,
            // Checks if the system has a battery that this program can access
            hasBattery,
            // Gets the battery percentage
            getBatteryPercent,
            // Gets whether the battery is charging
            getBatteryCharging,
            // Gets the display brightness percentage
            getBrightness,
            // Prints the system's local IP address
            getLocalIP,
            // Prints the system's public IP address
            getPublicIP
        };
    }
}
