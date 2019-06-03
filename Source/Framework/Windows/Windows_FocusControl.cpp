#include "Windows_FocusControl.h"
#include "Windows_XInterface.h"
#include "MainWindow.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Windows::FocusControl::";
#endif

// Milliseconds to wait between window focus attempts:
static const constexpr int focusWaitMs = 100;
// Each attempt to set and check window focus status should wait a little longer
// before the next test. Wait time multiplier:
static const constexpr float focusWaitMultiplier = 1.3;
// Milliseconds to wait before abandoning window focus attempts:
static const constexpr int focusTimeout = 20000;

Windows::FocusControl::FocusControl() 
{
    focusChecker.setCheckInterval(focusWaitMs, focusWaitMultiplier);
}


// Focuses the window with the given window ID, waiting until focus is gained
// successfully before doing anything else.
void Windows::FocusControl::focusWindow
(const int windowID, std::function<void()> onFailure)
{
    Windows::XInterface xInterface;
    xInterface.activateWindow(windowID);
    focusChecker.startCheck([&xInterface, windowID]()
    {
        return xInterface.isActiveWindow(windowID);        
    },
    [windowID]()
    {
        DBG(dbgPrefix << "::focusWindow: Focused window " << windowID);
    }, focusTimeout);
    focusChecker.waitForUpdate(false);
}


// Refocuses this application's window, and ensures the main component has
// keyboard focus.
void Windows::FocusControl::takeFocus()
{
    Windows::XInterface xInterface;
    int appWindowID = xInterface.getMainAppWindow();
    if (appWindowID == BadWindow)
    {
        DBG(dbgPrefix << __func__ << ": Application window not found!");
        return;
    }
    juce::Component* mainComponent = MainWindow::getOpenWindow()
            ->getChildComponent(0);
    xInterface.activateWindow(appWindowID);
    focusChecker.startCheck([&xInterface, appWindowID, mainComponent]()
    {
        if (mainComponent->isShowing() || mainComponent->isOnDesktop())
        {
            mainComponent->grabKeyboardFocus();
        }
        return xInterface.isActiveWindow(appWindowID) 
                && mainComponent->hasKeyboardFocus(true);        
    },
    [appWindowID]()
    {
        DBG(dbgPrefix << "::takeFocus: Focused window " << appWindowID);
    }, focusTimeout);
    focusChecker.waitForUpdate(false);
}
