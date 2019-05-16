#include "Process_Launched.h"
#include "Process_Data.h"
#include "Process_State.h"
#include "JuceHeader.h"
#include "Windows_XInterface.h"
#include <unistd.h>
#include <cstdint>

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Process::Launched::";
#endif

// If a juce::ChildProcess crashes, attempts to wait for it to terminate will
// wait forever if not given a timeout period. Instead of waiting indefinitely,
// LaunchedProcess will repeatedly wait for processCheckInterval milliseconds,
// checking the process state between waiting periods.
static const constexpr int processCheckInterval = 500;

// Runs an application launch command in a new child process on construction.
Process::Launched::Launched(const juce::String launchCommand) :
launchCommand(launchCommand)
{
    if (childProcess.start(launchCommand))
    {
        Data currentProcess(getpid());
        juce::Array<Data> childProcesses = currentProcess.getChildProcesses();
        jassert(!childProcesses.isEmpty());
        for (const Data& process : childProcesses)
        {
            if (process.isValid())
            {
                if (launchCommand.contains(process.getExecutableName()))
                {
                    processId = process.getProcessId();
                    break;
                }
            }
        }
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Failed to launch " << launchCommand);
        outputValid = false;
    }
}


// Gets the launch command used to start the process.
juce::String Process::Launched::getLaunchCommand() const
{
    return launchCommand;
}


// Checks if the launched application is still running.
bool Process::Launched::isRunning()
{
    bool processRunning = childProcess.isRunning();
    if (processRunning)
    {
        Data processData(processId);
        DBG(dbgPrefix << __func__ << ": process " << launchCommand
                << " is in state " << stateString(processData.getLastState()));
        if (!processData.isValid()
                || processData.getLastState() == State::dead)
        {
            DBG(dbgPrefix << __func__
                    << ": Launched process is dead/missing, but juce::Process "
                    << "object reports that it is still running.");
            DBG(dbgPrefix << __func__
                    << ": Attempting to kill invalid process:");
            const bool processKilled = kill();
            jassert(processKilled);
            processRunning = false;
        }
    }
    return processRunning;
}


// Attempts to terminate the application process.
bool Process::Launched::kill()
{
    outputValid = false;
    return childProcess.kill();
}


// Waits for the application process to end.
void Process::Launched::waitForProcessToFinish(const int timeoutMs)
{
    const bool noTimeout = (timeoutMs == -1);
    juce::uint32 endTime = juce::Time::getMillisecondCounter() + timeoutMs;
    Data process(processId);
    while (childProcess.isRunning()
            && process.getLastState() != State::zombie
            && process.getLastState() != State::stopped
            && process.getLastState() != State::dead
            && process.getLastState() != State::unknown
            && process.getLastState() != State::invalid
            && (noTimeout || juce::Time::getMillisecondCounter() < endTime))

    {
        int waitTime = processCheckInterval;
        if (timeoutMs >= 0)
        {
            juce::uint32 timeLeft = endTime
                    - juce::Time::getMillisecondCounter();
            if (timeLeft < waitTime)
            {
                waitTime = timeLeft;
            }
        }
        childProcess.waitForProcessToFinish(waitTime);
        process = Data(processId);
    }
#ifdef JUCE_DEBUG
    if (juce::Time::getMillisecondCounter() >= endTime
            && childProcess.isRunning())
    {
        DBG(dbgPrefix << __func__ << ": process " << launchCommand
                << " did not finish, process state="
                << stateString(process.getLastState()));
    }
#endif
}


// Gets all text output by the process to stdout and stderr.
juce::String Process::Launched::getProcessOutput()
{
    if (!outputValid)
    {
        return juce::String();
    }
    if (!isRunning())
    {
        return childProcess.readAllProcessOutput();
    }
    else
    {
        Data process(processId);
        DBG(dbgPrefix << __func__ << ": process " << launchCommand
                << " is still active in state "
                << stateString(process.getLastState())
                << ", can't get output");
        return juce::String();
    }
}


// Gets the exit code from the launched process.
juce::uint32 Process::Launched::getExitCode()
{
    if (isRunning())
    {
        return UINT32_MAX;
    }
    return childProcess.getExitCode();
}


// Moves the application's windows in front of all other windows and focuses
// them.
void Process::Launched::activateWindow()
{
    if (!isRunning())
    {
        DBG(dbgPrefix << __func__ << ": application is not running!");
        return;
    }
    if (processId == -1)
    {
        DBG(dbgPrefix << __func__ << ": process is not found!");
        return;
    }

    Windows::XInterface xWindows;
    juce::Array<Window> appWindows = xWindows.getMatchingWindows(
            [this, &xWindows](Window window)
    {
        return xWindows.getWindowPID(window) == processId
                && xWindows.getWindowName(window).isNotEmpty()
                && xWindows.getWindowDesktop(window) != -1;
    }, false);
    if (appWindows.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": no windows found!");
        return;
    }
    for (const Window& window : appWindows)
    {
        DBG(dbgPrefix << __func__ << ": Activating window for "
                << launchCommand);
        xWindows.activateWindow(window);
    }
}
