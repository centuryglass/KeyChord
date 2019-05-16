#include "Windows_MainWindow.h"
#include "Windows_FocusListener.h"
#include "Windows_FocusTracker.h"
#include "SharedResource_Handler.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Windows::MainWindow::";
#endif

namespace Windows { class FocusUpdater; }

/**
 * @brief  The private FocusTracker handler class used by the MainWindow to
 *         update the window focus state.
 */
class Windows::FocusUpdater : public FocusListener
{
public:
    FocusUpdater() { }

    virtual ~FocusUpdater() { }

    /**
     * @brief  Updates whether the main application window is currently focused.
     *
     * @param windowFocused    Whether the MainWindow object is the focused
     *                         window.
     *
     * @param notifyListeners  Whether listeners should be notified if the
     *                         window focus state is changed.
     */
    void setFocusState(const bool windowFocused,
            const bool notifyListeners = true)
    {
        SharedResource::LockedPtr<FocusTracker> focusTracker
                = getWriteLockedResource();
        focusTracker->setFocusState(windowFocused, notifyListeners);
    }
};

// The FocusUpdater used by the sole MainWindow instance. The MainWindow will
// check if this value is non-null to make sure another MainWindow doesn't
// already exist.
static std::unique_ptr<Windows::FocusUpdater> focusUpdater(nullptr);

// Ensures the focus tracker is initialized and sets initial window properties.
Windows::MainWindow::MainWindow(const juce::String& title,
        juce::Colour backgroundColour, int requiredButtons) :
juce::DocumentWindow(title, backgroundColour, requiredButtons)
{
    if (focusUpdater != nullptr)
    {
        DBG(dbgPrefix << __func__ << ": MainWindow object already exists!");
        jassertfalse;
    }
    else
    {
        focusUpdater.reset(new FocusUpdater);
    }
}


// Closes the window's connection to the FocusTracker resource.
Windows::MainWindow::~MainWindow()
{
    if (focusUpdater == nullptr)
    {
        DBG(dbgPrefix << __func__
                << ": FocusTracker connection already null!");
        jassertfalse;
    }
    else
    {
        focusUpdater.reset(nullptr);
    }
}


// Ensures all future window focus changes will be sent to the FocusTracker
// resource.
void Windows::MainWindow::startFocusTracking()
{
    trackingEnabled = true;
    activeWindowStatusChanged();

}


// Ensures all listeners are notified when the window focus state changes.
void Windows::MainWindow::activeWindowStatusChanged()
{
    juce::DocumentWindow::activeWindowStatusChanged();
    if (!trackingEnabled)
    {
        return;
    }
    const bool isFocused = isActiveWindow();
    if (isFocused)
    {
        // Window focus regained, close modal components so AlertWindows don't
        // get stuck.
        juce::ModalComponentManager::getInstance()->cancelAllModalComponents();
    }
    DBG(dbgPrefix << __func__ << ": Window focus "
            << (isFocused ? "gained" : "lost") << ", notifying listeners.");
    focusUpdater->setFocusState(isFocused);
}
