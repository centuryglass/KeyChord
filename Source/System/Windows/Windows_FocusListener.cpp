#include "Windows_FocusListener.h"
#include "Windows_FocusTracker.h"

// Connects to the FocusTracker on construction, creating the tracker instance
// if necessary.
Windows::FocusListener::FocusListener() { }


// Checks if the main application window is currently focused.
bool Windows::FocusListener::getFocusState() const
{
    SharedResource::LockedPtr<const FocusTracker> focusTracker
            = getReadLockedResource();
    return focusTracker->getFocusState();
}


// Signals that the main application window just gained focus.
void Windows::FocusListener::windowFocusGained() { }


// Signals that the main application window just lost focus.
void Windows::FocusListener::windowFocusLost() { }
