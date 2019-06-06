#include "Application.h"
#include "MainWindow.h"
#include "Windows_XInterface.h"
#include "Windows_FocusControl.h"

#ifdef INCLUDE_TESTING
#include "Debug_ScopeTimerRecords.h"
#endif

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Application::";
#endif


#ifdef INCLUDE_TESTING
// Sets if tests should run after the window is created and focused.
static bool runTests = false;

// Sets if tests should print all output.
static bool verboseTesting = false;

// Specific test categories to run:
static juce::StringArray testCategories;
#endif

static Window targetWindow = BadWindow;

// Cleans up all application resources, then sets them up again.
void Application::restart()
{
    inputController.reset(nullptr);
    mainView.reset(nullptr);

    homeWindow.reset(new MainWindow(getApplicationName()));
    mainView.reset(new Component::MainView);
    inputController.reset(new Input::Controller(mainView.get(), targetWindow));
    homeWindow->setContentNonOwned(mainView.get(), false);
    homeWindow->setVisible(true);
    homeWindow->addToDesktop();
}
// Performs all required initialization when the application is first launched.
void Application::initialise(const juce::String &commandLine)
{
    // Save the window that will receive input:

    Windows::XInterface xWindows;
    if (targetWindow == BadWindow)
    {
        targetWindow = xWindows.getActiveWindow();
        DBG("Saving target window " << (int) targetWindow << " with name "
                << xWindows.getWindowName(targetWindow));
    }

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
    homeWindow.reset(new MainWindow(getApplicationName()));
    homeWindow->setLookAndFeel(lookAndFeel.get());

    // Initialize and apply the chord component and input handler:
    mainView.reset(new Component::MainView);
    homeWindow->setContentNonOwned(mainView.get(), false);
    homeWindow->setVisible(true);
    homeWindow->addToDesktop();
    mainView->setBounds(homeWindow->getLocalBounds());
    inputController.reset(new Input::Controller(mainView.get(), targetWindow));

    // Ensure the application window is active and has keyboard focus:
    Windows::FocusControl focusControl;
    focusControl.takeFocus();
    #ifdef INCLUDE_TESTING
    if (runTests)
    {
        runApplicationTests();
    }
    #endif
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
    return true;
}


// Kills this instance of the application when a new instance launches to
// replace it.
void Application::anotherInstanceStarted(const juce::String& commandLine)
{
    DBG(dbgPrefix << __func__ << ": New instance launching, closing this one.");
    quit();
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
