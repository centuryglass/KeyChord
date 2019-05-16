#include "Process_Launched.h"
#include "Process_Launcher.h"
#include "Windows_FocusListener.h"
#include "Windows_XInterface.h"
#include "Testing_DelayUtils.h"
#include "Util_Commands.h"
#include "JuceHeader.h"
#include <map>

// Simple commands to run, paired with expected output:
static const std::map<juce::String, juce::String> outputCommands =
{
    {"echo(test)", " (test)"},
    {"expr 1 + 1", "2"},
};

// Terminal applications to try running:
static const juce::StringArray termApps =
{
    "top",
    "vi"
};

// Simple windowed applications to try running:
static const juce::StringArray windowedApps =
{
    "xclock",
    "xeyes"
};

/**
 * @brief  A minimal FocusListener class used to check if the window is focused.
 */
class FocusChecker : public Windows::FocusListener
{
public:
    FocusChecker() { }
    virtual ~FocusChecker() { }
};

namespace Process { class LaunchedTest; }

/**
 * @brief  Tests that Process::Launcher correctly launches new processes using
 *         Process::Launched objects.
 */
class Process::LaunchedTest : public juce::UnitTest
{
public:
    LaunchedTest() : juce::UnitTest("Process::Launched testing", "Process") {}

    void runTest() override
    {
        using juce::String;
        String output;
        Util::Commands commandReader;

        beginTest("Process output test");
        for (auto& iter : outputCommands)
        {
            String command = iter.first;
            String expectedResult = iter.second;
            String commandName = command.initialSectionNotContaining(" ");
            if (Launcher::testCommand(commandName))
            {
                Launched outputProcess(command);
                outputProcess.waitForProcessToFinish(1000);
                expect(!outputProcess.isRunning(), String("\"")
                        + commandName
                        + "\" process still running, but should be finished.");
                String output = outputProcess.getProcessOutput().trim();
                expectEquals(output, expectedResult, String("\"") + commandName
                        + "\" process output was incorrect.");
                expect(!outputProcess.kill(),
                        "Kill command returned true, invalid if finished");
            }
            else
            {
                logMessage(String("Command \"") + commandName
                        + "\" doesn't appear to be valid, skipping test.");
            }
        }

        beginTest("Terminal application test");
        for (const String& command : termApps)
        {
            if (Launcher::testCommand(command))
            {
                Launched termProcess(command);
                expect(Testing::DelayUtils::idleUntil([&termProcess]()
                {
                    return termProcess.isRunning();
                }, 500, 5000), String("\"") + command
                        + "\" process is not running.");
                output = termProcess.getProcessOutput();
                expect(output.isEmpty(), String("Unexpected process output ")
                        + output);
                expect(termProcess.kill(), "Failed to kill process.");
            }
            else
            {
                logMessage(String("Command \"") + command
                        + "\" doesn't appear to be valid, skipping test.");
            }
        }

        beginTest("Invalid command handling test");
        String badCommand("DefinitelyNotAValidLaunchCommand");
        expect(!Launcher::testCommand(badCommand),
                "DefinitelyNotAValidLaunchCommand should have been invalid.");
        Launched bad("DefinitelyNotAValidLaunchCommand");
        expect(!Testing::DelayUtils::idleUntil([&bad]()
        {
            return bad.isRunning();
        }, 200, 2000), "Process running despite bad launch command.");
        output = bad.getProcessOutput();
        expectEquals(String(), output,
                "Bad process should have had no output.");
        expectEquals(String(bad.getExitCode()), String("0"),
            "Bad process error code should have been 0.");

        FocusChecker focusChecker;
        beginTest("Windowed launch and activation test");
        for (const String& command : windowedApps)
        {
            if (Launcher::testCommand(command))
            {
                Launched windowedApp(command);
                expect(Testing::DelayUtils::idleUntil([&windowedApp]()
                {
                    return windowedApp.isRunning();
                }, 100, 1000), String("\"") + command
                        + "\" process not is running.");
                windowedApp.activateWindow();
                expect(Testing::DelayUtils::idleUntil([&focusChecker]()
                {
                    return !focusChecker.getFocusState();
                }, 500, 8000),
                        "pocket-home window should not be focused.");
                windowedApp.kill();
                Windows::XInterface xwin;
                xwin.activateWindow(xwin.getMainAppWindow());
                expect(!windowedApp.isRunning(), String("\"") + command
                        + "\" process should be dead.");
            }
            else
            {
                logMessage(String("Command \"") + command
                        + "\" doesn't appear to be valid, skipping test.");
            }
        }
    }
};

static Process::LaunchedTest test;
