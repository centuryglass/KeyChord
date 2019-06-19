#include "Output_Sending.h"
#include "Application.h"
#include "Output_Modifiers.h"
#include "Windows_XInterface.h"
#include "Windows_FocusControl.h"
#include "Text_Values.h"

/**
 * @brief  The set of Application::WindowFlags values that should be applied to
 *         the application window before shifting focus to the target window and
 *         sending input.
 *
 *   On ClockworkPi GameShell, shifting focus to the target window only works
 * from the default window position, where the window fills the bottom half of
 * the display. I suspect that this is because the GameShell's window manager
 * is doing something unusual, but I'm not really sure. For now, I'm just going
 * to move the window when sending input, and put it back afterwards.
 */
static const constexpr int windowReadyForSending =
        (int) Application::WindowFlag::snapToBottom;

// Commmand prefix used to transmit key presses to the focused window.
static const juce::String keyCommand("xdotool key ");

// Print the full namespace name before all debug output:
#ifdef JUCE_DEBUG
static const constexpr char* dbgPrefix = "Input::Sending::";
#endif

/**
 * @brief  Ensures the application window is positioned so that it focus can be
 *         transferred to the target window, saving and returning the previous
 *         window state.
 *
 * @return  The combination of window flags describing the window's previous
 *          state.
 */
static int prepareAppWindow()
{
    Application* application = Application::getInstance();
    jassert(application != nullptr);
    const int windowState = application->getWindowFlags();
    if (windowState != windowReadyForSending)
    {
        DBG(dbgPrefix << __func__ << ": Resetting window for key input:");
        application->resetWindow(windowReadyForSending);
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Window already prepared for key input");
    }
    return windowState;
}

/**
 * @brief  Attempts to focus a target window.
 *
 * @param targetWindow  The ID of the window that should be focused.
 *
 * @return              Whether focusing the window succeeded.
 */
static bool focusTarget(const int targetWindow)
{
    DBG(dbgPrefix << __func__ << ": Focusing target window " << targetWindow);
    Windows::FocusControl focusControl;
    bool focused = true;
    focusControl.focusWindow(targetWindow, [&focused]()
    {
        DBG(dbgPrefix << "failure callback: failed to focus target window.");
        focused = false;
    });
    return focused;

}

/**
 * @brief  Attempts to focus the KeyChord application window, restoring its
 *         previous state.
 *
 * @param restoredWindowFlags  A set of window flags that describe how the
 *                             window was placed.
 *
 * @return                     Whether focusing the window succeeded.
 */
static bool focusAppWindow(const int restoredWindowFlags = 0)
{
    Application* application = Application::getInstance();
    jassert(application != nullptr);
    if (restoredWindowFlags != windowReadyForSending)
    {
        DBG(dbgPrefix << __func__ << ": Restoring window flags:");
        application->resetWindow(restoredWindowFlags);
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": No need to restore window flags.");
    }
    Windows::XInterface xInterface;
    const Window appWindow = xInterface.getMainAppWindow();
    DBG(dbgPrefix << __func__ << ": Activating app window:");
    xInterface.activateWindow(appWindow);
    return xInterface.isActiveWindow(appWindow);
}

/**
 * @brief  Gets the string used to represent a single key input value to
 *         xdotool.
 *
 * @param keyValue   The value of the key that should be typed.
 *
 * @param modifiers  The modifier keys held down when the key is typed.
 *
 * @return           The string used to create that key event using xdotool.
 */
static juce::String getKeyString(const Text::CharValue keyValue,
        const juce::String modifiers = "")
{
    juce::String keyString = Text::Values::getXString(keyValue);
    if (keyString.isEmpty())
    {
        DBG(dbgPrefix << __func__ << ": Invalid key value " << (int) keyValue);
        jassertfalse;
        return keyString;
    }
    if (keyString == "'")
    {
        return modifiers + "\\'";
    }
    return modifiers + keyString;
}

/**
 * @brief  Runs the xdotool command that sends a key event to the focused
 *         window.
 *
 * @param keyString  A string representing the key event that should be sent.
 */
static void runXCommand(const juce::String keyString)
{
    if (keyString.isEmpty())
    {
        jassertfalse;
        return;
    }
    const juce::String commandString = keyCommand + keyString;
    DBG(dbgPrefix << __func__ << ": running command \"" << commandString
            << "\"");
    system(commandString.toRawUTF8());
}


// Sends a single key press event to a window.
void Output::Sending::sendKey(
        const Text::CharValue keyValue,
        const int modifierFlags,
        const int targetWindow)
{
    const int previousState = prepareAppWindow();
    const juce::String modifiers = Modifiers::getModString(modifierFlags);
    const bool focusedTarget = focusTarget(targetWindow);
    if (! focusedTarget)
    {
        DBG(dbgPrefix << __func__ << ": Failed to focus target window!");
        jassertfalse;
    }
    runXCommand(getKeyString(keyValue, modifiers));
    const bool restoreFocus = focusAppWindow(previousState);
    if (! restoreFocus)
    {
        DBG(dbgPrefix << __func__ << ": Failed to restore window focus!");
        jassertfalse;
    }
}


// Take all input from a buffer object, and send it to a window.
void Output::Sending::sendBufferedOutput
(Buffer& outputBuffer, const int targetWindow)
{
    const int previousState = prepareAppWindow();
    const bool focusedTarget = focusTarget(targetWindow);
    if (! focusedTarget)
    {
        DBG(dbgPrefix << __func__ << ": Failed to focus target window!");
        jassertfalse;
    }
    const juce::String modifiers
            = Modifiers::getModString(outputBuffer.getModifierFlags());
    const Text::CharString inputText = outputBuffer.getBufferedText();
    for (const Text::CharValue& keyValue : inputText)
    {
        runXCommand(getKeyString(keyValue, modifiers));
    }
    outputBuffer.clear();
    const bool restoreFocus = focusAppWindow(previousState);
    if (! restoreFocus)
    {
        DBG(dbgPrefix << __func__ << ": Failed to restore window focus!");
        jassertfalse;
    }
}
