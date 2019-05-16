#pragma once
/**
 * @file  Hardware_Battery.h
 *
 * @brief  Tracks the state of the system's battery.
 */

#include "JuceHeader.h"

namespace Hardware { class Battery; }

/**
 * @brief  Finds and shares the current charge percentage and charging state of
 *         the system's battery.
 *
 *  Battery objects uses Util::Commands to get the status of the system's
 * battery. Hardware::I2CBus is used as a fallback battery information source
 * if system commands fail.
 */
class Hardware::Battery
{
public:
    /**
     * @brief  Determines the most appropriate way to monitor battery state on
     *         construction.
     */
    Battery();

    virtual ~Battery() { }

    /**
     * @brief  Stores battery charge percentage and whether the battery is
     *         currently charging.
     */
    struct Status
    {
        int percent = 0;
        bool isCharging = 0;
    };

    /**
     * @brief  Checks if the Battery object is able to detect and read battery
     *         information.
     *
     * @return  Whether the system battery status is available.
     */
    bool isBatteryAvailable() const;

    /**
     * @brief  Finds the current battery charge percentage and charging state.
     *
     * @return  An integer between 0 and 100 representing battery charge state,
     *          and a boolean value indicating if the battery is charging. If
     *          battery information can't be found, percent is set to -1.
     */
    Status getBatteryStatus();

private:
    /**
     * @brief  Possible battery information sources, listed from highest to
     *         lowest precedence.
     *
     * The most appropriate data source will be set by the constructor.
     */
    enum DataSource
    {
        // Use battery commands provided by Util::Commands.
        systemCommand,
        // Directly query the I2C bus to read the battery percentage.
        i2cBus,
        // Unable to find battery percentage, return no data.
        noBattery
    };
    DataSource dataSource;
};
