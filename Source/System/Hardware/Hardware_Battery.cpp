#include "Hardware_Battery.h"
#include "Util_Math.h"
#include "Util_Commands.h"
#ifdef CHIP_FEATURES
#include "Hardware_I2CBus.h"
#endif

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Hardware::Battery::";
#endif


// Determines the most appropriate way to monitor battery state on construction.
Hardware::Battery::Battery()
{
    Util::Commands commandReader;
    // Check if Util::Commands can detect a battery:
    if (commandReader.runTextCommand(Util::CommandTypes::Text::hasBattery)
            .isNotEmpty())
    {
        DBG(dbgPrefix << __func__ << ": data source set to Util::Commands.");
        dataSource = systemCommand;
    }
    #ifdef CHIP_FEATURES
    else // Check if valid i2cBus data is available:
    {
        try
        {
            I2CBus i2c;
            i2c.batteryGaugePercent();
            DBG(dbgPrefix << __func__ << ": data source set to i2c bus.");
            dataSource = i2cBus;
        }
        catch(I2CBus::I2CException e)
        {
            DBG(dbgPrefix << __func__ << ": no available battery source.");
            dataSource = noBattery;
        }
    }
    #else
    dataSource = noBattery;
    #endif
}


// Checks if the Battery object is able to detect and read battery information.
bool Hardware::Battery::isBatteryAvailable() const
{
    return dataSource != noBattery;
}


// Find the current battery charge percentage and charging state
Hardware::Battery::Status Hardware::Battery::getBatteryStatus()
{
    Status currentStatus;
    currentStatus.percent = -1;
    if (dataSource == noBattery)
    {
        return currentStatus;
    }
#ifdef CHIP_FEATURES
    if (dataSource == i2cBus)
    {
        try
        {
            I2CBus i2c;
            currentStatus.isCharging = i2c.batteryIsCharging();
            currentStatus.percent = i2c.batteryGaugePercent();
        }
        catch(I2CBus::I2CException e)
        {
            DBG(e.getErrorMessage());
            DBG(dbgPrefix << __func__ << ": I2C bus access failed, disabling "
                    << "battery monitoring.");
            dataSource = noBattery;
        }
        return currentStatus;
    }
    else // dataSource == systemCommand
    {
        Util::Commands commandReader;
        const juce::String chargeStatus = commandReader.runTextCommand(
                Util::CommandTypes::Text::getBatteryCharging);
        const juce::String chargePercent = commandReader.runTextCommand(
                Util::CommandTypes::Text::getBatteryPercent);
        currentStatus.isCharging = (chargeStatus == "1");
        currentStatus.percent = chargePercent.getIntValue();
    }
#endif
    currentStatus.percent = Util::Math::median<int>(0, currentStatus.percent,
            100);
    return currentStatus;
}
