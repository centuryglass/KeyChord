#include "Application.h"
#include "MainWindow.h"
#include "Windows_XInterface.h"
#include "Windows_FocusControl.h"

#ifdef INCLUDE_TESTING
#include "Debug_ScopeTimerRecords.h"
#endif

#ifdef JUCE_DEBUG
#include <map>
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Application::";


// Make the window the size of the GameShell display on debug builds:
static const constexpr int dbgWidth = 320;
static const constexpr int dbgHeight = 240;
#endif

#ifdef INCLUDE_TESTING
// Sets if tests should run after the window is created and focused.
static bool runTests = false;

// Sets if tests should print all output.
static bool verboseTesting = false;

// Specific test categories to run:
static juce::StringArray testCategories;
#endif

// Used to determine how much of the screen height to use:
static const constexpr int standardHeightDivisor = 2;
static const constexpr int minimizedHeightDivisor = 9;

// Window where input will be sent, saved on launch.
static Window targetWindow = BadWindow;


// Gets the active application instance.
Application* Application::getInstance()
{
    return static_cast<Application*>(juce::JUCEApplication::getInstance());
}

/**
 * @brief  Gets the bounds of the primary display's user area.
 *
 * @return  The display bounds, or the bounds of the ClockworkPi GameShell 
 *          display in debug builds.
 */
static juce::Rectangle<int> getDisplayBounds()
{
#ifdef JUCE_DEBUG
    return juce::Rectangle<int>(0, 0, dbgWidth, dbgHeight);
#else
    return juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea;
#endif
}


// Gets the set of window flags currently applied to the application window.
int Application::getWindowFlags()
{
    int flags = 0;
    if (homeWindow != nullptr)
    {
        const juce::Rectangle<int> windowBounds = homeWindow->getLocalBounds();
        const juce::Rectangle<int> displayBounds = getDisplayBounds();
        if (displayBounds.getHeight() == windowBounds.getHeight())
        {
            flags |= (int) WindowFlag::showingHelp;
        }
    }
    if (mainConfig.getSnapToBottom())
    {
        flags |= (int) WindowFlag::snapToBottom;
    }
    if (mainConfig.getMinimized())
    {
        flags |= (int) WindowFlag::minimized;
    }
    return flags;
}

#ifdef JUCE_DEBUG
/**
 * @brief  Lists the full names of all window flags present in a combination
 *         of flags.
 *
 * @param windowFlags  Some combination of window flag values.
 *
 * @return             A string naming the selected flags.
 */
static juce::String getFlagNames(const int windowFlags)
{
    std::map<int, juce::String> flagNames =
    {
        { (int) Application::WindowFlag::showingHelp,  "showingHelp" },
        { (int) Application::WindowFlag::snapToBottom, "snapToBottom" },
        { (int) Application::WindowFlag::minimized,    "minimized" }
    };
    juce::String names("(");
    for (const auto& iter : flagNames)
    {
        if ((windowFlags & iter.first) != 0)
        {
            if (names.length() > 1)
            {
                names += ", ";
            }
            names += iter.second;
        }
    }
    if (names.length() == 1)
    {
        names += "none)";
    }
    else
    {
        names += ")";
    }
    return names;
}
#endif

// Recreates the MainWindow bounds to suit the current circumstances.
void Application::resetWindow(const int windowFlags)
{
    const juce::Rectangle<int> displayBounds = getDisplayBounds();
    juce::Rectangle<int> targetBounds = displayBounds;
    if ((windowFlags & (int) WindowFlag::showingHelp) == 0)
    {
        if ((windowFlags & (int) WindowFlag::minimized) != 0)
        {
            targetBounds.setHeight(targetBounds.getHeight() 
                    / minimizedHeightDivisor);
        }
        else
        {
            targetBounds.setHeight(targetBounds.getHeight()
                    / standardHeightDivisor);
        }
        if ((windowFlags & (int) WindowFlag::snapToBottom) != 0)
        {
            targetBounds.setY(displayBounds.getHeight()
                    - targetBounds.getHeight());
        }
    }
    DBG(dbgPrefix << __func__ << ": refreshing window with bounds "
            << targetBounds.toString() << ", window flags = " 
            << getFlagNames(windowFlags));

    // Create the new window, applying the target bounds:
    homeWindow.reset(new MainWindow(getApplicationName()));
    homeWindow->setContentNonOwned(mainView.get(), false);
    homeWindow->setVisible(true);
    homeWindow->setBounds(targetBounds);
    homeWindow->addToDesktop();

    // Ensure the application window is active and has keyboard focus:
    Windows::FocusControl focusControl;
    focusControl.takeFocus(mainView.get());
}


// Recreates the MainWindow, updating window flags to match the current selected
// configuration.
void Application::resetUpdatingFlags()
{
    resetWindow(getWindowFlags());
}


// Performs all required initialization when the application is first launched.
void Application::initialise(const juce::String &commandLine)
{
    // Extract and process arguments:
    using juce::StringArray;
    StringArray args;
    args.addTokens(commandLine, true);

    // If help text is requested, print it and immediately quit:
    if (args.contains("--help"))
    {
        using std::cerr;
        cerr << "arguments:" << std::endl;
        cerr << "  --help           Print this help text\n";
        #ifdef INCLUDE_TESTING
        cerr << "  --test           Run program tests\n";
        cerr << "     -categories   Run only tests within listed categories\n";
        cerr << "     -v            Verbose test output\n";
        #endif
        quit();
        return;
    }

    // Skip normal init and run tests if they're enabled and requested:
    #ifdef INCLUDE_TESTING
    runTests = args.contains("--test");
    if (runTests)
    {
        verboseTesting = args.contains("-v");
        int categoryIndex = args.indexOf("-categories");
        if (categoryIndex != -1 && (args.size() > (categoryIndex + 1)))
        {
            testCategories.addArray(StringArray::fromTokens(
                        args[categoryIndex + 1].unquoted(), false));
        }

        // Use an empty window when testing.
        homeWindow.reset(new juce::DocumentWindow(getApplicationName(),
                juce::Colours::dimgrey, juce::DocumentWindow::allButtons));
        homeWindow->setBounds(0, 0, 50, 50);
        homeWindow->setLookAndFeel(
                &juce::LookAndFeel::getDefaultLookAndFeel());
        homeWindow->setUsingNativeTitleBar(true);
        homeWindow->setResizable(true, false);
        homeWindow->setVisible(true);
        homeWindow->addToDesktop();
        return;
    }
    #endif

    // Save the window that will receive input:
    Windows::XInterface xWindows;
    if (targetWindow == BadWindow)
    {
        targetWindow = xWindows.getActiveWindow();
        DBG("Saving target window " << (int) targetWindow << " with name "
                << xWindows.getWindowName(targetWindow));
    }

    // Setup UI theme object:
    lookAndFeel.reset(new Theme::LookAndFeel);
    juce::LookAndFeel::setDefaultLookAndFeel(lookAndFeel.get());
    mainView.reset(new Component::MainView);
    inputController.reset(new Input::Controller(mainView.get(), targetWindow,
                outputBuffer));
    resetWindow(getWindowFlags());
}


// Performs all necessary cleanup steps before the application can be safely
// closed.
void Application::shutdown()
{
    DBG(dbgPrefix << __func__ << ": Closing application resources.");
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);

    inputController.reset(nullptr);
    mainView.reset(nullptr);
    homeWindow.reset(nullptr);
    lookAndFeel.reset(nullptr);
    #ifdef INCLUDE_TESTING
    Debug::ScopeTimerRecords::printRecords();
    #endif
    DBG(dbgPrefix << __func__ << ": Ending process.");

}


// Gets the name of this application.
const juce::String Application::getApplicationName()
{
    return ProjectInfo::projectName;
}


// Gets the application's version string.
const juce::String Application::getApplicationVersion()
{
    return ProjectInfo::versionString;
}


// Checks if multiple versions of this application may run simultaneously.
bool Application::moreThanOneInstanceAllowed()
{
    return false;
}


// Kills this instance of the application when a new instance launches to
// replace it.
void Application::anotherInstanceStarted(const juce::String& commandLine)
{
    DBG(dbgPrefix << __func__ << ": New instance launching, closing this one.");
    systemRequestedQuit();
}



#ifdef INCLUDE_TESTING
// Runs application tests and shuts down the application.
void Application::runApplicationTests()
{
    juce::UnitTestRunner tester;
    tester.setPassesAreLogged(verboseTesting);
    if (testCategories.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": Running all application tests.");
        tester.runAllTests();
    }
    else
    {
        for (const juce::String& category : testCategories)
        {
            DBG(dbgPrefix << __func__ << ": Running test category: "
                    << category);
            tester.runTestsInCategory(category);
        }
    }
    DBG(dbgPrefix << __func__ << ": Finished running application tests.");
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
#endif
