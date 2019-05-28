#include "HomeApplication.h"
#include "HomeWindow.h"
#include "Windows_XInterface.h"
#include "Util_ShutdownListener.h"
#include "Util_TempTimer.h"

#ifdef INCLUDE_TESTING
#include "Debug_ScopeTimerRecords.h"
#endif

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "HomeApplication::";
#endif

// Milliseconds to wait between window focus attempts:
static const constexpr int focusWaitMs = 100;
// Each attempt to set and check window focus status should wait a little longer
// before the next test. Wait time multiplier:
static const constexpr float focusWaitMultiplier = 1.3;
// Milliseconds to wait before abandoning window focus attempts:
static const constexpr int focusTimeout = 20000;

#ifdef INCLUDE_TESTING
// Sets if tests should run after the window is created and focused.
static bool runTests = false;

// Sets if tests should print all output.
static bool verboseTesting = false;

// Specific test categories to run:
static juce::StringArray testCategories;
#endif


// Performs all required initialization when the application is first launched.
void HomeApplication::initialise(const juce::String &commandLine)
{
    // Save the window that will receive input:
    Windows::XInterface xWindows;
    Window targetWindow = xWindows.getActiveWindow();
    DBG("Saving target window " << (int) targetWindow << " with name "
            << xWindows.getWindowName(targetWindow));

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

    // Create the UI appearance management object and initialize the window
    // normally:
    lookAndFeel.reset(new Theme::LookAndFeel);
    juce::LookAndFeel::setDefaultLookAndFeel(lookAndFeel.get());
    homeWindow.reset(new HomeWindow(getApplicationName()));
    homeWindow->setLookAndFeel(lookAndFeel.get());

    // Initialize and apply the chord component and input handler:
    chordPreview.reset(new Component::ChordPreview);
    homeWindow->setContentNonOwned(chordPreview.get(), false);
    homeWindow->setVisible(true);
    homeWindow->addToDesktop();
    chordPreview->setBounds(homeWindow->getLocalBounds());
    inputController.reset(new Input::Controller(chordPreview.get(),
                targetWindow, xWindows.getMainAppWindow()));

    // Ensure the application window is active and has keyboard focus:
    focusChecker.setCheckInterval(focusWaitMs, focusWaitMultiplier);
    const auto onFocus = [this]()
    {
        DBG(dbgPrefix << __func__
                << ": Main window focused, enabling focus tracking:");
        static_cast<Windows::MainWindow*>(homeWindow.get())
                ->startFocusTracking();
        #ifdef INCLUDE_TESTING
        if (runTests)
        {
            runApplicationTests();
        }
        #endif
    };
    focusChecker.startCheck([this]() { return focusAppWindow(); },
            onFocus, focusTimeout,
    [this]()
    {
        DBG(dbgPrefix << "initialise"
                << ": Window focus attempts timed out.");
        #if defined(JUCE_DEBUG)
        Windows::XInterface xWindows;
        Window appWindow = xWindows.getMainAppWindow();
        if (appWindow == BadWindow)
        {
            DBG(dbgPrefix << "initialise"
                    << ": Failed because main window ID was bad.");
        }
        else
        {
            DBG(dbgPrefix << "initialise"
                    << ": window active: "
                    << (xWindows.isActiveWindow(appWindow) ? "true" : "false")
                    << ", keyboard focused: "
                    << (homeWindow->hasKeyboardFocus(true) ? "true" : "false"));
        }
        xWindows.printWindowTree();
        #endif

        static_cast<Windows::MainWindow*>(homeWindow.get())
                ->startFocusTracking();
    });
}


// Performs all necessary cleanup steps before the application can be safely
// closed.
void HomeApplication::shutdown()
{
    DBG(dbgPrefix << __func__ << ": Closing application resources.");
    Util::ShutdownBroadcaster::broadcastShutdown();
    homeWindow.reset(nullptr);
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    inputController.reset(nullptr);
    chordPreview.reset(nullptr);
    lookAndFeel.reset(nullptr);
    #ifdef INCLUDE_TESTING
    Debug::ScopeTimerRecords::printRecords();
    #endif
    DBG(dbgPrefix << __func__ << ": Ending process.");

}


// Gets the name of this application.
const juce::String HomeApplication::getApplicationName()
{
    return ProjectInfo::projectName;
}


// Gets the application's version string.
const juce::String HomeApplication::getApplicationVersion()
{
    return ProjectInfo::versionString;
}


// Checks if multiple versions of this application may run simultaneously.
bool HomeApplication::moreThanOneInstanceAllowed()
{
    return false;
}

#ifdef INCLUDE_TESTING
// Runs application tests and shuts down the application.
void HomeApplication::runApplicationTests()
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

// Attempts to activate the application window and grab keyboard focus.
bool HomeApplication::focusAppWindow()
{
    if (homeWindow == nullptr)
    {
        return false;
    }
    Windows::XInterface xWindows;
    Window appWindow = xWindows.getMainAppWindow();
    if (appWindow == BadWindow)
    {
        return false;
    }
    bool isActive = xWindows.isActiveWindow(appWindow);
    if (! isActive)
    {
        xWindows.activateWindow(appWindow);
        isActive = xWindows.isActiveWindow(appWindow);
    }
    bool hasKeyboardFocus = homeWindow->hasKeyboardFocus(true);
    if (! hasKeyboardFocus)
    {
        homeWindow->grabKeyboardFocus();
        hasKeyboardFocus = homeWindow->hasKeyboardFocus(true);
    }
    return isActive && hasKeyboardFocus;
}
