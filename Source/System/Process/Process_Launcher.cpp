#include "Process_Launcher.h"
#include "Util_Commands.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Process::Launcher::";
#endif

// Timer interval in milliseconds.
static const int timerFrequency = 2000;

// Milliseconds to wait before forcibly terminating a window focus operation.
static const int windowFocusTimeout = 1000;

// Milliseconds to wait before assuming the last application launch failed.
static const int pendingLaunchTimeout = 15000;

// Class key:
static const juce::Identifier localeClassKey = "Process::Launcher";

// Localized text value keys:
static const juce::Identifier couldNotOpenTextKey    = "couldNotOpen";
static const juce::Identifier notValidCommandTextKey = "notValidCommand";

Process::Launcher::Launcher() : Locale::TextUser(localeClassKey),
launchFailureCallback([](){}) { }

void Process::Launcher::setLaunchFailureCallback
(const std::function<void()> failureCallback)
{
    launchFailureCallback = failureCallback;
}


// Launches a new application, or focuses its window if the application is
// already running
void Process::Launcher::startOrFocusApp(const juce::String& command)
{
    DBG(dbgPrefix << __func__ << ": command = " << command);
    // Before adding another process to the list, clean out any old dead ones,
    // so they don't start piling up.
    std::vector<Launched*> toRemove;
    Launched* appInstance = nullptr;
    for (Launched* app : runningApps)
    {
        if (!app->isRunning())
        {
            toRemove.push_back(app);
        }
        else if (app->getLaunchCommand() == command)
        {
            appInstance = app;
        }
    }
    for (Launched* appProcess : toRemove)
    {
        DBG(dbgPrefix << __func__ << ": removing terminated app "
                << appProcess->getLaunchCommand());
        runningApps.removeObject(appProcess);
    }
    if (appInstance != nullptr)
    {
        if (appInstance->isRunning())
        {
            DBG(dbgPrefix << __func__ << ": app is already running,"
                    << " focusing the window");
            appInstance->activateWindow();
            return;
        }
        else
        {
            if (appInstance != nullptr)
            {
                DBG(dbgPrefix << __func__
                        << ": Old process is dead, re-launching");
            }
        }
    }
    Launched* newApp = startApp(command);
    if (newApp != nullptr)
    {
        runningApps.add(newApp);
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Failed to launch " << command);
    }
}


// Checks a string to see if it is a valid shell command.
bool Process::Launcher::testCommand(const juce::String& command)
{
    Util::Commands systemCommands;
    return systemCommands.runIntCommand(Util::CommandTypes::Int::commandCheck,
            command) == 0;
}


// Starts a new instance of an application process
Process::Launched* Process::Launcher::startApp(const juce::String& command)
{
    using juce::AlertWindow;
    using juce::String;
    DBG(dbgPrefix << __func__  << ": Launching: "  << command);
    if (!testCommand(command))
    {
        AlertWindow::showMessageBoxAsync
                (AlertWindow::AlertIconType::WarningIcon,
                localeText(couldNotOpenTextKey),
                String("\"") + command + String("\"")
                + localeText(notValidCommandTextKey));
        launchFailureCallback();
        return nullptr;
    }

    Launched* newApp = new Launched(command);
    timedProcess = newApp;
    lastLaunch = juce::Time::getMillisecondCounter();
    startTimer(timerFrequency);
    return newApp;
}


// Checks if the last launched application started successfully, and displays an
// error message if the application process died.
void Process::Launcher::timerCallback()
{
    if (timedProcess != nullptr)
    {
        if (timedProcess->isRunning())
        {
            // If the process is still going and we have yet to reach timeout,
            // wait longer.
            if (juce::Time::getMillisecondCounter() - lastLaunch
                    < pendingLaunchTimeout)
            {
                return;
            }
            else
            {
                DBG(dbgPrefix << __func__ << ": Process \""
                        << timedProcess->getLaunchCommand()
                        << "\" launch timed out");
            }
        }
        else
        {
            DBG(dbgPrefix << __func__ << ": process died, show message");
            juce::String output = timedProcess->getProcessOutput();
            juce::StringArray lines = juce::StringArray::fromLines(output);
            output = "";
            for (int i = lines.size() - 1;
                i > lines.size() - 6 && i >= 0; i--)
            {
                output = lines[i] + juce::String("\n") + output;
            }
            juce::AlertWindow::showMessageBoxAsync
                    (juce::AlertWindow::AlertIconType::WarningIcon,
                    localeText(couldNotOpenTextKey), output);
            timedProcess = nullptr;
        }
    }
    launchFailureCallback();
    stopTimer();
}


// Cancels pending checks on the last launched application if the application
// window loses focus.
void Process::Launcher::onSuspend()
{
    stopTimer();
}
