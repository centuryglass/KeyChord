#pragma once
/**
 * @file  Windows_XInterface.h
 *
 * @brief  Finds and manipulates open windows.
 */

namespace Windows { class XInterface; }

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "JuceHeader.h"

/**
 * @brief  Interacts with the X Window System to find and manipulate windows.
 *
 *  This class searches for windows by window title, application name, or
 * process ID. Once found, it can focus these windows, and raise them above
 * other windows on the display.
 *
 *  Most of the code in this module was adapted from xdotool, an extremely
 * useful utility for manipulating windows and automatically triggering mouse
 * and keyboard events.
 *
 * @see https://github.com/jordansissel/xdotool
 */
class Windows::XInterface
{
public:
    /**
     * @brief  Opens a connection to the X11 display manager on construction.
     *
     * @param displayName  The name of the display to access. If null, the
     *                     default display set by the $DISPLAY environment
     *                     variable will be used.
     */
    XInterface(const char* displayName = nullptr);

    /**
     * @brief  Closes the connection to the X11 display manager.
     */
    virtual ~XInterface();

    /**
     * @brief  Gets the XLib window object that represents this application's
     *         main window.
     *
     * @return  The XLib Window identifier representing this application's main
     *          window.
     */
    Window getMainAppWindow() const;

    /**
     * @brief  Gets the title of a window.
     *
     * @param window   An XLib window identifier.
     *
     * @return         The title of the window, or the empty string if the
     *                 window is invalid or has no title.
     */
    juce::String getWindowName(const Window window) const;

    /**
     * @brief  Gets the class of a window.
     *
     * @param window  An XLib window identifier.
     *
     * @return        The class of the window, or the empty string if the
     *                window is invalid or has no class value.
     */
    juce::String getWindowClass(const Window window) const;

    /**
     * @brief  Gets the className of a window.
     *
     * @param window   An XLib window identifier.
     *
     * @return         The className of the window, or the empty string if the
     *                 window is invalid or has no className.
     */
    juce::String getWindowClassName(const Window window) const;

    /**
     * @brief  Gets the ID of the process that created a window.
     *
     * @param window   An XLib window identifier.
     *
     * @return         The ID of the process that created the window, or -1 if
     *                 the window is invalid.
     */
    int getWindowPID(const Window window) const;

    /**
     * @brief  Performs a breadth-first search of the entire window tree,
     *         returning windows that fit some match criteria.
     *
     * @param verifyMatch             An arbitrary matching function that will
     *                                be used to determine which windows are
     *                                matching. Window values will be included
     *                                in the returned window array if and only
     *                                if the verifyMatch function returns true
     *                                when called with that window as a
     *                                parameter.
     *
     * @param stopAtFirstMatchDepth   If true, once a matching window is found,
     *                                the search will not check any windows
     *                                that are deeper in the window tree. If
     *                                false, the search will continue to search
     *                                further down on the window tree.
     *
     * @return                        All windows selected by the verifyMatch
     *                                function.
     */
    juce::Array<Window> getMatchingWindows(
                    const std::function<bool(const Window)> verifyMatch,
                    const bool stopAtFirstMatchDepth = true) const;

    /**
     * @brief  Gets all child windows of a given parent window.
     *
     * @param parent  The XLib window identifier of the parent window.
     *
     * @return        All child windows in stacking order from bottom to top,
     *                or an empty array if parent is an invalid window.
     */
    juce::Array<Window> getWindowChildren(const Window parent) const;

    /**
     * @brief  Finds all direct ancestors of a window and returns them in
     *         parent->child order.
     *
     * @param window  A valid XLib window identifier.
     *
     * @return        An array of windows, where the first value is the root
     *                window, the last is the window passed in to this
     *                function, and each window in the array is a child of the
     *                window before it and the parent of the window after it.
     */
    juce::Array<Window> getWindowAncestry(const Window window) const;

    /**
     * @brief  Gets all siblings of a window and returns the list sorted from
     *         bottom to top.
     *
     * @param window  A valid XLib window identifier.
     *
     * @return        An array of all windows(including the window parameter)
     *                that have the same parent window as the window parameter.
     */
    juce::Array<Window> getWindowSiblings(const Window window) const;

    /**
     * @brief  Finds the parent of a window.
     *
     * @param window  An XLib window identifier.
     *
     * @return        The window's parent, or 0 if the window is invalid or has
     *                no parent.
     */
    Window getWindowParent(const Window window) const;

    /**
     * @brief  Checks if a specific window is active.
     *
     * @param window  An XLib window identifier.
     *
     * @return        Whether the window exists, has nonzero size, is on the
     *                current desktop, has keyboard focus, and is not covered
     *                by other windows.
     */
    bool isActiveWindow(const Window window) const;

    /**
     * @brief  Activates a window.
     *
     *  This will switch the active desktop to the one containing this window,
     * bring the window to the front, and set it as the focused window.
     *
     * @param window   The XLib ID of the window to activate.
     */
    void activateWindow(const Window window) const;

    /**
     * @brief  Finds the current selected desktop index.
     *
     * @return   The index of the current active desktop, or -1 if the system
     *           does not support multiple desktops.
     */
    int getDesktopIndex() const;

    /**
     * @brief  Sets the current active desktop index.
     *
     *  This will do nothing if the new index is the same as the current index,
     * the index is not a valid desktop index, or the system does not support
     * multiple desktops.
     *
     * @param desktopIndex  The index of the desktop to set as active.
     */
    void setDesktopIndex(const int desktopIndex) const;

    /**
     * @brief  Gets the index of the desktop that contains a specific window.
     *
     * @param window   The XLib window identifier to check.
     *
     * @return         The index of the desktop containing the window, or -1 if
     *                 the window is invalid or the system does not support
     *                 multiple desktops.
     */
    int getWindowDesktop(const Window window) const;

#if JUCE_DEBUG
    /**
     * @brief  Prints comprehensive debug information about a window.
     *
     * @param window  The XLib window identifier used to find window
     *                information.
     */
    void printWindowInfo(const Window window) const;

    /**
     * @brief  Recursively prints the entire window tree under some root
     *         window, from back to front.
     *
     * @param root   The root window to start searching under. If root is 0 and
     *               depth is 0, this will be replaced with the default root
     *               window.
     *
     * @param depth  The current tree depth.
     */
    void printWindowTree(const Window root = 0, const int depth = 0) const;
#endif

private:
    /**
     * @brief  Recursively searches for a specific window's ancestry.
     *
     * @param parents     The current window search tree in parent->child order.
     *
     * @param searchWin   The window ID being searched for among the child
     *                    windows of the last window in the parents array.
     *
     * @return            The parents array updated to include searchWin and
     *                    all of its parents, or an empty array if searchWin is
     *                    not a child of the last window in the parents array.
     */
    juce::Array<Window> recursiveWindowSearch
    (const juce::Array<Window>& parents, const Window searchWin) const;

    /**
     * @brief  Holds any type of XLib window property data.
     */
    struct WindowProperty;

    /**
     * @brief  Gets an arbitrary window property.
     *
     * @param window    The window ID to check for the given property.
     *
     * @param property  The value used by XLib to describe the requested
     *                  property.
     *
     * @return          A structure containing the requested property, or one
     *                  containing no data if the property was not found.
     */
    WindowProperty getWindowProperty
    (const Window window, const Atom property) const;

    /**
     * @brief  Checks if a particular property type is supported by the window
     *         manager.
     *
     * @param property  The name of a property set by the window manager or
     *                  pager.
     *
     * @return          Whether this property is supported by the current
     *                  window manager or pager.
     */
    bool xPropertySupported(const char* property) const;

    // XLib display pointer, used to connect to the X Window system.
    Display* display = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XInterface)
};
