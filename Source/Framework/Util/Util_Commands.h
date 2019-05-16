#pragma once
/**
 * @file  Util_Commands.h
 *
 * @brief  Loads and runs miscellaneous system commands.
 */

#include "Util_CommandTypes.h"
#include "SharedResource_Handler.h"

namespace Util
{
    class Commands;
    // Private SharedResource class
    class CommandJSON;
}
/**
 * @brief  Accesses a read-only list of system commands.
 *
 *  This application needs to be able to get information about the system it's
 * running on, and it needs to control several basic system properties. These
 * system commands might not work on all systems, so rather than embedding them
 * in the program, Commands reads them from a JSON file.
 *
 *  All system commands are defined in the commands.json file in the
 * application data directory. This file is stored as read-only rather than as
 * a ConfigFile because these commands should be kept secure and do not need to
 * be changed during execution.
 *
 *  All system commands are sorted into three categories. ActionCommands
 * execute asynchronously, and return no data. IntCommands return the command
 * process exit code. TextCommands return all text output by the command
 * process. TextCommands must always exit returning 0, or their text output
 * will be lost.
 *
 *  The distinction between TextCommands and IntCommands only exists because
 * juce::ChildProcess objects cannot return their text output if they finish
 * with a nonzero exit code. These types should be merged into a single type
 * after adding a way to get text output from processes that exit irregularly.
 *
 *  Command definitions should include a single executable name plus an
 * optional object list. If more complex shell commands are needed, they should
 * be bundled into a shell script and placed in the scripts directory in the
 * application data directory. The commands.json file defines the full path to
 * the script directory with the "PROJECT_SCRIPTS" key. Any command text that
 * contains the literal string PROJECT_SCRIPTS will have that string replaced
 * with the correct script directory path.
 *
 *  To override the default commands, create a file called
 * overrideCommands.json and define any replacement commands there using the
 * same keys as commands.json. This file should be placed in the application
 * data directory. Commands defined in this file will take precedence over ones
 * in the default commands.json file. The overrideCommands.json file will not
 * be altered or removed when this application updates, even if the default
 * command file is updated.
 */
class Util::Commands : public SharedResource::Handler<CommandJSON>
{
public:
    Commands();

    virtual ~Commands() { }


    /**
     * @brief  Asynchronously runs a command in a new process.
     *
     * @param commandType  The system command to run.
     *
     * @param args         Arguments to pass to the command process.
     *
     * @return             True if the command was found and executed, false if
     *                     the command was not defined.
     */
    bool runActionCommand(const CommandTypes::Action commandType,
            const juce::String& args = "");

    /**
     * @brief  Runs a command, waits for it to finish, and return its exit code.
     *
     * @param commandType  The system command to run.
     *
     * @param args         Arguments to pass to the command process.
     *
     * @return             The command process exit code, or -1 if the command
     *                     was not defined.
     */
    int runIntCommand(const CommandTypes::Int commandType,
            const juce::String& args = "");

    /**
     * @brief  Runs a command, waits for it to finish, and returns its text
     *         output.
     *
     *  If the command process terminates with a nonzero exit code, no text
     * will be returned.
     *
     * @param commandType  The system command to run.
     *
     * @param args         Arguments to pass to the command process.
     *
     * @return             All text printed by the command process, or the
     *                     empty string if the command was not defined, or did
     *                     not terminate normally with an exit code of 0.
     */
    juce::String runTextCommand(const CommandTypes::Text commandType,
            const juce::String& args = "");

private:
    /**
     * @brief  Gets a system command string.
     *
     * @param commandKey  The key selecting the system command type.
     *
     * @param args        Any arguments that should be appended to the command.
     *
     * @return            The full command string, including arguments.
     */
    juce::String getCommandString(const juce::Identifier& commandKey,
            const juce::String& args = "");
};
