#include "Windows_FocusTracker.h"
#include "Windows_FocusInterface.h"

// SharedResource object key:
const juce::Identifier Windows::FocusTracker::resourceKey
        = "Windows::FocusTracker";

Windows::FocusTracker::FocusTracker() :
SharedResource::Resource(resourceKey) { }


// Checks if the main application window is focused.
bool Windows::FocusTracker::getFocusState() const
{
    return isFocused;
}


// Updates whether the main application window is currently focused.
void Windows::FocusTracker::setFocusState
(const bool windowFocused, const bool notifyListeners)
{
    if (windowFocused != isFocused)
    {
        isFocused = windowFocused;
        if (notifyListeners)
        {
            foreachHandler<FocusInterface>(
            [this](FocusInterface* focusListener)
            {
                if (isFocused)
                {
                    focusListener->windowFocusGained();
                }
                else
                {
                    focusListener->windowFocusLost();
                }
            });
        }
    }
}
