#pragma once
/**
 * @file  Process_Launcher.h
 *
 * @brief  Launches and tracks new application processes, automatically
 *         switching window focus to the launched application.
 */

#include <functional>
#include <map>
#include "Process_Launched.h"
#include "Locale_TextUser.h"
#include "JuceHeader.h"
#include "Windows_FocusedTimer.h"

namespace Process { class Launcher; }

/**
 * @brief  Launches and manages independant windowed applications.
 *
 *  Process::Launcher runs application launch commands in new child processes,
 * monitoring them so that any errors can be handled appropriately.
 *
 *  Launcher attempts to avoid creating multiple processes simultaneously
 * running the same command. When given a launch command, if Launcher already
 * created a process with that command, and that process is still running,
 * Launcher will attempt to find and focus an application window belonging to
 * that process, rather than creating a new process.
 *
 *  If Launcher is given an invalid launch command, or the launched process
 * dies before it gains window focus while still within a launch timeout
 * period, the launch is considered a failure. If a callback function was set
 * using setLaunchFailureCallback, that callback function will run once each
 * time an application launch fails. If the launch fails because the launch
 * command was invalid, an AlertWindow will also be created to explain the
 * failure to the user.
 *
 * TODO: Read timeout periods from a ConfigFile.
 */
class Process::Launcher : public Windows::FocusedTimer,
        private Locale::TextUser
{
public:
    Launcher();

    virtual ~Launcher() { }

    /**
     * @brief  Assigns a function to call if launching an application fails.
     *
     * @param failureCallback   A function to run if an application fails to
     *                          launch.
     */
    void setLaunchFailureCallback(const std::function<void()> failureCallback);

    /**
     * @brief  Launches a new application, or focuses its window if the
     *         application is already running
     *
     * @param command   The shell command that starts the application.
     */
    void startOrFocusApp(const juce::String& command);

    /**
     * @brief  Checks a string to see if it is a valid shell command.
     *
     * @param command  The command string to test.
     *
     * @return         True if and only if the command string is a valid shell
     *                 command.
     */
    static bool testCommand(const juce::String& command);

private:
    /**
     * @brief  Starts a new instance of an application process.
     *
     *  If the launch command is invalid, the user will be shown a
     * juce::AlertWindow containing an error message.
     *
     * @param command   The command used to launch the process.
     *
     * @return          A pointer to the object representing the application
     *                  process, or nullptr if the command was invalid.
     */
    Launched* startApp(const juce::String& command);

    /**
     * @brief  Checks if the last launched application started successfully,
     *         and displays an error message if the application process died.
     */
    virtual void timerCallback() override;

    /**
     * @brief  Cancels pending checks on the last launched application if the
     *         application window loses focus.
     */
    virtual void onSuspend() override;

    // The function to run if application launching fails
    std::function<void()> launchFailureCallback;

    // Holds all running process objects created by the AppLauncher.
    juce::OwnedArray<Launched> runningApps;

    // Last application launch time from Time::getMillisecondCounter()
    juce::uint32 lastLaunch = 0;

    // Process to check up on when the timer finishes.
    Launched* timedProcess = nullptr;
};
