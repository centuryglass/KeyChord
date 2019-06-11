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
static const constexpr float focusWaitMultiplier = 1.1;
// Milliseconds to wait before abandoning window focus attempts:
static const constexpr int focusTimeout = 2000;

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
        const bool isActive = xInterface.isActiveWindow(windowID);        
        if (! isActive)
        {
            xInterface.activateWindow(windowID);
        }
        return isActive;
    },
    [windowID]()
    {
        DBG(dbgPrefix << "focusWindow: Focused window " << windowID);
    }, focusTimeout);
    focusChecker.waitForUpdate(true);
}


// Refocuses this application's window, and ensures the main component has
// keyboard focus.
void Windows::FocusControl::takeFocus(juce::Component* mainComponent)
{
    Windows::XInterface xInterface;
    int appWindowID = xInterface.getMainAppWindow();
    if (appWindowID == BadWindow)
    {
        DBG(dbgPrefix << __func__ << ": Application window not found!");
        return;
    }
    xInterface.activateWindow(appWindowID);
    focusChecker.startCheck([&xInterface, appWindowID, mainComponent]()
    {
        DBG(dbgPrefix << "::takeFocus: checking focus: ");
        const bool keyboardFocused = mainComponent->hasKeyboardFocus(true);
        const bool windowFocused = xInterface.isActiveWindow(appWindowID);
        if (! windowFocused)
        {
            DBG(dbgPrefix 
                    << "::takeFocus: no window focus, trying again..: ");
            xInterface.activateWindow(xInterface.getMainAppWindow());
        }
        if (! keyboardFocused && (mainComponent->isShowing() 
                || mainComponent->isOnDesktop()))
        {
            DBG(dbgPrefix 
                    << "::takeFocus: trying to get missing keyboard focus: ");
            mainComponent->grabKeyboardFocus();
        }
        return xInterface.isActiveWindow(appWindowID) 
                && mainComponent->hasKeyboardFocus(true);        
    },
    [appWindowID]()
    {
        DBG(dbgPrefix << "::takeFocus: Focused window " << appWindowID);
    }, focusTimeout,
    []()
    {
        DBG(dbgPrefix << "::takeFocus: Failed to focus window!");
    });
    focusChecker.waitForUpdate(true);
}
