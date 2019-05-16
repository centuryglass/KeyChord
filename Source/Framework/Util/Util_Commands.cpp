#include "Util_Commands.h"
#include "Assets_JSONFile.h"
#include "Process_Launched.h"
#include "SharedResource_Resource.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Util::Commands::";
#endif

// Script directory key
static const juce::Identifier scriptDirKey("PROJECT_SCRIPTS");

// SharedResource object key
static const juce::Identifier jsonResourceKey("Util::CommandJSON");

// Default JSON command file name
static const juce::String defaultFilename("commands.json");

// Replacement JSON command file name
static const juce::String overrideFilename("overrideCommands.json");

/**
 * @brief  Gets the Identifier key used to store an action command.
 *
 * @param commandType  The requested system command.
 *
 * @return             The key needed to load the command from the JSON file.
 */
static const juce::Identifier& actionCommandKey
(const Util::CommandTypes::Action commandType)
{
    using ActionCommand = Util::CommandTypes::Action;
    switch(commandType)
    {
        // Shutdown the system:
        case ActionCommand::shutdown:
        {
            static const juce::Identifier shutdownKey("shutdown");
            return shutdownKey;
        }
        // Restart the system:
        case ActionCommand::restart:
        {
            static const juce::Identifier restartKey("restart");
            return restartKey;
        }
        // Enter sleep mode:
        case ActionCommand::sleep:
        {
            static const juce::Identifier sleepKey("sleep");
            return sleepKey;
        }
        // Exit sleep mode:
        case ActionCommand::wake:
        {
            static const juce::Identifier wakeKey("wake");
            return wakeKey;
        }
        // Calibrate the touch screen:
        case ActionCommand::calibrate:
        {
            static const juce::Identifier calibrateKey("calibrate");
            return calibrateKey;
        }
        // Sets the system volume percentage:
        case ActionCommand::setVolume:
        {
            static const juce::Identifier setVolumeKey("set volume");
            return setVolumeKey;
        }
        // Sets the display brightness percentage:
        case ActionCommand::setBrightness:
        {
            static const juce::Identifier setBrightnessKey("set brightness");
            return setBrightnessKey;
        }
    }
    DBG(dbgPrefix << __func__ << ": Error, unhandled command type!");
    jassertfalse;
    return juce::Identifier::null;
}

/**
 * @brief  Gets the Identifier key used to store an int command.
 *
 * @param commandType  The requested system command.
 *
 * @return             The key needed to load the command from the JSON file.
 */
static const juce::Identifier& intCommandKey
(const Util::CommandTypes::Int commandType)
{
    using IntCommand = Util::CommandTypes::Int;
    switch(commandType)
    {
        // Returns 0 if and only if the system is sleeping:
        case IntCommand::sleepCheck:
        {
            static const juce::Identifier sleepCheckKey("sleep check");
            return sleepCheckKey;
        }
        // Returns 0 if and only if a command is valid:
        case IntCommand::commandCheck:
        {
            static const juce::Identifier commandTestKey("command check");
            return commandTestKey;
        }
    }
    DBG(dbgPrefix << __func__ << ": Error, unhandled command type!");
    jassertfalse;
    return juce::Identifier::null;
}

/**
 * @brief  Gets the Identifier key used to store a text command.
 *
 * @param commandType  The requested system command.
 *
 * @return             The key needed to load the command from the JSON file.
 */
static const juce::Identifier& textCommandKey
(const Util::CommandTypes::Text commandType)
{
    using TextCommand = Util::CommandTypes::Text;
    switch(commandType)
    {
        // Prints the system volume percentage
        case TextCommand::getVolume:
        {
            static const juce::Identifier getVolumeKey("get volume");
            return getVolumeKey;
        }
        // Gets the display brightness percentage
        case TextCommand::getBrightness:
        {
            static const juce::Identifier getBrightnessKey("get brightness");
            return getBrightnessKey;
        }
        // Gets the system's IP address on the local network
        case TextCommand::getLocalIP:
        {
            static const juce::Identifier localIPKey("get local IP");
            return localIPKey;
        }
        // Gets the system's global IP address
        case TextCommand::getPublicIP:
        {
            static const juce::Identifier publicIPKey("get public IP");
            return publicIPKey;
        }
        // Checks if the system has a battery
        case TextCommand::hasBattery:
        {
            static const juce::Identifier findBatteryKey("find battery");
            return findBatteryKey;
        }
        // Gets the battery percentage
        case TextCommand::getBatteryPercent:
        {
            static const juce::Identifier percentKey("get battery percent");
            return percentKey;
        }
        // Gets whether the battery is charging
        case TextCommand::getBatteryCharging:
        {
            static const juce::Identifier chargingKey("is battery charging");
            return chargingKey;
        }
    }
    DBG(dbgPrefix << __func__ << ": Error, unhandled command type!");
    jassertfalse;
    return juce::Identifier::null;
}


/**
 * @brief  The private SharedResource class used to load and store system
 *         commands.
 */
class Util::CommandJSON : public SharedResource::Resource
{
private:
    // Project script directory path:
    juce::String scriptDir;

    // JSON file storing default command definitions:
    Assets::JSONFile defaultCommands;

    // Alternate JSON file storing replacement command definitions:
    Assets::JSONFile overrideCommands;

public:
    friend class Commands;

    CommandJSON() : SharedResource::Resource(jsonResourceKey),
    overrideCommands(overrideFilename),
    defaultCommands(defaultFilename)
    {
        scriptDir = getCommandString(scriptDirKey);
        if (scriptDir.isEmpty())
        {
            DBG(dbgPrefix << __func__ << ": " << scriptDir <<
                    " is not a valid script directory.");
        }
    }

    virtual ~CommandJSON() { }

    /**
     * @brief  Gets a command from a command file, using override commands
     *         before default commands if possible.
     *
     * @param key  The key to a system command.
     *
     * @return     The corresponding command string, or the empty string if the
     *             command isn't found.
     */
    juce::String getCommandString(const juce::Identifier& key) const
    {
        using juce::String;
        String command;
        if (overrideCommands.isValidFile()
                && overrideCommands.propertyExists<String>(key))
        {
            command = overrideCommands.getProperty<String>(key);
        }
        if (command.isEmpty() && defaultCommands.isValidFile()
                && defaultCommands.propertyExists<String>(key))
        {
            command = defaultCommands.getProperty<String>(key);
        }
        if (command.isNotEmpty())
        {
            String dirKeyString = scriptDirKey.toString();
            int index = command.indexOf(dirKeyString);
            if (index != -1)
            {
                command = command.substring(0, index) + scriptDir
                    + command.substring(index + dirKeyString.length());
            }
        }
        return command;
    }
};

Util::Commands::Commands() :
SharedResource::Handler<CommandJSON>(jsonResourceKey) { }


// Asynchronously runs a command in a new process.
bool Util::Commands::runActionCommand(const CommandTypes::Action commandType,
        const juce::String& args)
{
    juce::String command
            = getCommandString(actionCommandKey(commandType), args);
    if (command.isEmpty())
    {
        return false;
    }
    juce::Thread::launch([command]() { system(command.toRawUTF8()); });
    return true;
}


// Runs a command, waits for it to finish, and return its exit code.
int Util::Commands::runIntCommand(const CommandTypes::Int commandType,
        const juce::String& args)
{
    juce::String command = getCommandString(intCommandKey(commandType), args);
    if (command.isEmpty())
    {
        return -1;
    }
    return system(command.toRawUTF8());
}


// Runs a command, waits for it to finish, and returns its text output.
juce::String Util::Commands::runTextCommand
(const CommandTypes::Text commandType, const juce::String& args)
{
    juce::String command = getCommandString(textCommandKey(commandType), args);
    if (command.isEmpty())
    {
        return juce::String();
    }
    Process::Launched commandProcess(command);
    commandProcess.waitForProcessToFinish();
    return commandProcess.getProcessOutput().trim();
}


// Gets a system command string.
juce::String Util::Commands::getCommandString
(const juce::Identifier& commandKey, const juce::String& args)
{
    using juce::String;
    SharedResource::LockedPtr<const CommandJSON> json
            = getReadLockedResource();
    juce::String command = json->getCommandString(commandKey);
    if (command.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": No command defined for key "
                << commandKey.toString());
        return String();
    }
    if (json->scriptDir.isNotEmpty())
    {
        String dirKeyString = scriptDirKey.toString();
        int index = command.indexOf(dirKeyString);
        if (index != -1)
        {
            command = command.substring(0, index) + json->scriptDir
                + command.substring(index + dirKeyString.length());
        }
    }
    if (args.isNotEmpty())
    {
        command += " ";
        command += args;
    }
    return command;
}
