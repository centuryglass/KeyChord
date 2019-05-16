#include "Windows_XInterface.h"
#include <cstring>
#include <cstdlib>
#include <X11/Xutil.h>

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Windows::XInterface::";
#endif

// All relevant window property names:

// The list of all supported window properties:
static const constexpr char* supportedFeatureProperty = "_NET_SUPPORTED";
// The current active window ID:
static const constexpr char* activeWindowProperty = "_NET_ACTIVE_WINDOW";
// The index of the current active desktop:
static const constexpr char* currentDesktopProperty = "_NET_CURRENT_DESKTOP";
// The desktop index where a window is located:
static const constexpr char* windowDesktopProperty = "_NET_WM_DESKTOP";
// The id of the process that created a window:
static const constexpr char* windowProcessProperty = "_NET_WM_PID";

// Holds any type of XLib window property data.
struct Windows::XInterface::WindowProperty
{
    // XLib window property identifier:
    Atom type = 0;
    // Size of bits used to store the property on the server:
    // This value may be more or less than the size of the returned type,
    // but it will always be nonzero if holding valid property data.
    int size = 0;
    // Number of items in the property data array:
    unsigned long numItems = 0;
    // Window property data array:
    unsigned char* data = nullptr;

    WindowProperty() { }

    /**
     * @brief  Constructs a WindowProperty struct as a copy of another
     *         WindowProperty.
     *
     * @param toCopy  Another property structure to copy.
     */
    WindowProperty(const WindowProperty& toCopy)
    {
        WindowProperty temp(toCopy);
         *this = std::move(temp);
    }

    /**
     * @brief  Constructs a WindowProperty struct using data taken from a
     *         temporary WindowProperty.
     *
     * @param rvalue  A temporary WindowProperty data source.
     */
    WindowProperty(WindowProperty&& rvalue)
    {
        data = rvalue.data;
        type = rvalue.type;
        size = rvalue.size;
        numItems = rvalue.numItems;
        rvalue.data = nullptr;
    }

    /**
     * @brief  Ensures that WindowProperty data is freed on destruction.
     */
    ~WindowProperty()
    {
        if (data != nullptr)
        {
            free(data);
            data = nullptr;
        }
    }

    /**
     * @brief  Assigns data from another WindowProperty structure to this
     *         structure.
     *
     * @param rhs  The property data structure to copy.
     *
     * @return     This WindowProperty structure.
     */
    WindowProperty& operator= (const WindowProperty& rhs)
    {
        type = rhs.type;
        size = rhs.size;
        numItems = rhs.numItems;
        if (data != nullptr)
        {
            free(data);
            data = nullptr;
        }
        if (rhs.data != nullptr)
        {
            data = static_cast<unsigned char*> (malloc(numItems * size));
            memcpy(data, rhs.data, numItems * size);
        }
        data = rhs.data;
        return *this;
    }

    /**
     * @brief  Gets a value stored in the property structure's data array.
     *
     * @param dataIndex  An optional index of the value within the data array.
     *
     * @tparam DataType  The type of data value being extracted from the array.
     *
     * @return           The extracted data value, or DataType() if the type or
     *                   index given were invalid.
     */
    template<typename DataType>
    DataType getDataValue(const int dataIndex = 0)
    {
        if (data == nullptr || size == 0)
        {
            // No valid property data
            jassertfalse;
        }
        else if (dataIndex < 0 || dataIndex >= numItems)
        {
            // Index is out of bounds
            jassertfalse;
        }
        else
        {
            DataType* typedData = reinterpret_cast<DataType*>(data);
            return typedData[dataIndex];
        }
        return DataType();
    }
};

// Opens a connection to the X11 display manager on construction.
Windows::XInterface::XInterface(const char* displayName) :
display(XOpenDisplay(displayName)) { }


// Closes the connection to the X11 display manager.
Windows::XInterface::~XInterface()
{
    if (display != nullptr)
    {
        XCloseDisplay(display);
        display = nullptr;
    }
}


// Gets the XLib window object that represents this application's main window.
Window Windows::XInterface::getMainAppWindow() const
{
    juce::Component * rootComponent
            = juce::Desktop::getInstance().getComponent(0);
    Window windowHandle;
    if (rootComponent == nullptr)
    {
        // window does not exist!
        return BadWindow;
    }
    else
    {
        windowHandle = (Window) rootComponent->getWindowHandle();
    }
    return windowHandle;
}


// Gets the title of a window.
juce::String Windows::XInterface::getWindowName(const Window window) const
{
    XTextProperty textProp;
    char** nameList = nullptr;
    XGetWMName(display, window, &textProp);
    juce::String name;
    if (textProp.nitems > 0)
    {
        int count = 0;
        Xutf8TextPropertyToTextList(display, &textProp, &nameList, &count);
        if (count > 1)
        {
            DBG(dbgPrefix << __func__ << ": " << "Window has "
                    << count << " names, returning comma-separated list.");
        }
        for (int i = 0; i < count; i++)
        {
            juce::String partialName = juce::CharPointer_UTF8(nameList[i]);
            if (!name.isEmpty() && !partialName.isEmpty())
            {
                name += ",";
            }
            name += partialName;
        }
    }
    XFreeStringList(nameList);
    XFree(textProp.value);
    return name;
}


// Gets the class of a window.
juce::String Windows::XInterface::getWindowClass(const Window window) const
{
    XClassHint classHint;
    if (!XGetClassHint(display, window, &classHint))
    {
        return juce::String();
    }
    juce::String classStr(classHint.res_name);
    XFree(classHint.res_name);
    XFree(classHint.res_class);
    return classStr;
}


// Gets the className of a window.
juce::String Windows::XInterface::getWindowClassName(const Window window) const
{
    XClassHint classHint;
    if (!XGetClassHint(display, window, &classHint))
    {
        return juce::String();
    }
    juce::String classStr(classHint.res_class);
    XFree(classHint.res_name);
    XFree(classHint.res_class);
    return classStr;
}


// Gets the ID of the process that created a window.
int Windows::XInterface::getWindowPID(const Window window) const
{
    Atom pidAtom = XInternAtom(display, windowProcessProperty, false);
    WindowProperty pidProp = getWindowProperty(window, pidAtom);
    if (pidProp.numItems == 0 || pidProp.size == 0 || pidProp.data == nullptr)
    {
        return -1;
    }
    return (int) pidProp.getDataValue<unsigned long>();
}


// Performs a breadth-first search of the entire window tree, returning windows
// that fit some match function.
juce::Array<Window> Windows::XInterface::getMatchingWindows(
            const std::function<bool(const Window)> verifyMatch,
            const bool stopAtFirstMatchDepth) const
{
    using juce::Array;
    const int screenCount = ScreenCount(display);
    Array<Window> matches;
    Array<Window> currentDepth;
    Array<Window> nextDepth;
    for (int i = 0; i < screenCount; i++)
    {
        currentDepth.add(RootWindow(display, i));
    }
    while (!currentDepth.isEmpty())
    {
        for (const Window& window : currentDepth)
        {
            if (verifyMatch(window))
            {
                matches.add(window);
            }
            if (stopAtFirstMatchDepth && !matches.isEmpty())
            {
                continue;
            }
            nextDepth.addArray(getWindowChildren(window));
        }
        if (stopAtFirstMatchDepth && !matches.isEmpty())
        {
            currentDepth.clear();
        }
        else
        {
            currentDepth = nextDepth;
            nextDepth.clear();
        }
    }
#if JUCE_DEBUG
    for (int i = 0; i < matches.size(); i++)
    {
        DBG(dbgPrefix << __func__ << ": Matching window " << i << ":");
        printWindowInfo(matches[i]);
    }
#endif
    return matches;
}


// Gets all child windows of a given parent window.
juce::Array<Window> Windows::XInterface::getWindowChildren
(const Window parent) const
{
    juce::Array<Window> children;
    unsigned int numChildren = 0;
    Window* childWindows = nullptr;
    Window unneededReturnVal;
    Status success = XQueryTree(display, parent,
            &unneededReturnVal, &unneededReturnVal,
            &childWindows, &numChildren);
    if (success && numChildren > 0)
    {
        for (int i = 0; i < numChildren; i++)
        {
            children.add(childWindows[i]);
        }
    }
    if (childWindows != nullptr)
    {
        XFree(childWindows);
        childWindows = nullptr;
    }
    return children;
}


// Finds all direct ancestors of a window and returns them in parent->child
// order.
juce::Array<Window> Windows::XInterface::getWindowAncestry
(const Window window) const
{
    juce::Array<Window> ancestry;
    const int screenCount = ScreenCount(display);
    for (int i = 0; i < screenCount; i++)
    {
        Window root = RootWindow(display, i);
        ancestry.add(root);
        if (root == window)
        {
            return ancestry;
        }
        ancestry = recursiveWindowSearch(ancestry, window);
        if (!ancestry.isEmpty())
        {
            break;
        }
    }
    return ancestry;
}


// Gets all siblings of a window and returns the list sorted from bottom to top.
juce::Array<Window> Windows::XInterface::getWindowSiblings
(const Window window) const
{
    Window parent = getWindowParent(window);
    return getWindowChildren(parent);
}


// Finds the parent of a window.
Window Windows::XInterface::getWindowParent(const Window window) const
{
    juce::Array<Window> parents = getWindowAncestry(window);
    if (parents.size() < 2)
    {
        return 0;
    }
    return parents[parents.size() - 2];
}


// Checks if a specific window is active.
bool Windows::XInterface::isActiveWindow(const Window window) const
{
    DBG(dbgPrefix << __func__ << ": Checking if window "
            << getWindowName(window) << " is focused:");
    using juce::Array;
    XWindowAttributes attr;
    int foundAttributes = XGetWindowAttributes(display, window, &attr);
    if (foundAttributes == 0)
    {
        DBG(dbgPrefix << __func__ << ": No, failed to get window attributes.");
        return false;
    }
    if (attr.map_state == IsUnmapped || attr.map_state == IsUnviewable)
    {
        DBG(dbgPrefix << __func__
                << ": No, window is not mapped or viewable.");
        return false;
    }
    if (attr.width == 0 || attr.height == 0)
    {
        DBG(dbgPrefix << __func__ << ": No, the window has zero area.");
        return false;
    }
    if (getDesktopIndex() != getWindowDesktop(window))
    {
        DBG(dbgPrefix << __func__
                << ": No, the window is on the wrong desktop.");
        return false;
    }
    jassert(xPropertySupported(activeWindowProperty));
    const Atom request = XInternAtom(display, activeWindowProperty, false);
    const Window root = XDefaultRootWindow(display);
    WindowProperty windowProp = getWindowProperty(root, request);
    if (windowProp.numItems == 0)
    {
        DBG(dbgPrefix << __func__ << ": No, there is no focused window.");
        return false;
    }
    Window activeWindow = windowProp.getDataValue<Window>();
    if (window != activeWindow)
    {
        DBG(dbgPrefix << __func__ << ": No, window "
                << getWindowName(activeWindow) << " is focused.");
        return false;
    }

    Array<Window> ancestors = getWindowAncestry(window);
    Window parentWindow = ancestors[ancestors.size() - 2];
    Array<Window> siblings = getWindowChildren(parentWindow);
    int windowIndex = siblings.indexOf(window);
    if (windowIndex == -1)
    {
        #ifdef JUCE_DEBUG
        DBG(dbgPrefix << __func__ << ": Error: window " << (int) window
                << " not found in child windows of " << (int) parentWindow);
        printWindowTree();
        jassertfalse;
        #endif
    }
    else
    {
        const int lastIndex = siblings.size() - 1;
        if (windowIndex != lastIndex)
        {
            DBG(dbgPrefix << __func__ << ": No, "  << (lastIndex - windowIndex)
                    << " window(s) are above this window");
            return false;
        }
    }
    int higherWindows = (siblings.size() - 1) - siblings.indexOf(window);
    if (higherWindows != 0)
    {
        DBG(dbgPrefix << __func__ << ": No, "  << higherWindows
                << " window(s) are above this window");
        return false;
    }
    DBG(dbgPrefix << __func__ << ": Yes, this window is focused");
    return true;
}


// Activates a window.
void Windows::XInterface::activateWindow(const Window window) const
{
    DBG(dbgPrefix << __func__ << ": activating window:");
    jassert(xPropertySupported(activeWindowProperty));
    // Switch to the window's desktop if necessary:
    if (xPropertySupported(currentDesktopProperty)
        && xPropertySupported(windowDesktopProperty))
    {
        setDesktopIndex(getWindowDesktop(window));
    }

    // Raise the window(and its parents) above other windows on the stack:
    juce::Array<Window> ancestors = getWindowAncestry(window);
    jassert(!ancestors.isEmpty() && ancestors.getLast() == window);
    for (const Window& window : ancestors)
    {
        juce::Array<Window> siblings = getWindowSiblings(window);
        if (siblings.size() < 2)
        {
            continue;
        }
        // Ensure override_redirect is enabled, or the window manager will
        // prevent us from moving windows.
        XWindowAttributes winAttr;
        XGetWindowAttributes(display, window, &winAttr);
        if (!winAttr.override_redirect)
        {
            XSetWindowAttributes newAttrs;
            newAttrs.override_redirect = true;
            // Get the mask for only changing the override_redirect attribute:
            long changeMask = 1L<<9; //defined on XSetWindowAttributes manpage
            int result = XChangeWindowAttributes(display, window, changeMask,
                    &newAttrs);
        }
        XRaiseWindow(display, window);
        XSync(display, false);
        XFlush(display);
        if (!winAttr.override_redirect)
        {
            // Reset override_redirect:
            XSetWindowAttributes newAttrs;
            newAttrs.override_redirect = false;
            long changeMask = 1L<<9;
            int result = XChangeWindowAttributes(display, window, changeMask,
                    &newAttrs);
        }
    }
    // Request input focus for the newly raised window:
    XEvent xEvent;
    memset(&xEvent, 0, sizeof(xEvent));
    xEvent.type = ClientMessage;
    xEvent.xclient.display = display;
    xEvent.xclient.window = window;
    xEvent.xclient.message_type
            = XInternAtom(display, activeWindowProperty, false);
    xEvent.xclient.format = 32;
    xEvent.xclient.data.l[0] = 2L; // 2 == Message from a window pager
    xEvent.xclient.data.l[1] = CurrentTime;

    XWindowAttributes winAttr;
    XGetWindowAttributes(display, window, &winAttr);
    int result = XSendEvent(display, winAttr.screen->root, false,
            SubstructureNotifyMask | SubstructureRedirectMask,
            &xEvent);

    if (result == BadWindow)
    {
        DBG(dbgPrefix << __func__
                << ": Got bad window error when requesting input focus.");
    }
    else if (result == BadValue)
    {
        DBG(dbgPrefix << __func__
                << ": Got bad value error when requesting input focus.");
    }
    XSync(display, false);
    XFlush(display);
}


// Finds the current selected desktop index.
int Windows::XInterface::getDesktopIndex() const
{
    if (!xPropertySupported(currentDesktopProperty))
    {
        return -1;
    }
    Atom request = XInternAtom(display, currentDesktopProperty, false);
    Window rootWindow = XDefaultRootWindow(display);
    WindowProperty desktopProp = getWindowProperty(rootWindow, request);
    if (desktopProp.numItems == 0 || desktopProp.size == 0
            || desktopProp.data == nullptr)
    {
        return -1;
    }
    return (int) desktopProp.getDataValue<long>();
}


// Sets the current active desktop index.
void Windows::XInterface::setDesktopIndex(const int desktopIndex) const
{
    if (!xPropertySupported(currentDesktopProperty))
    {
        return;
    }

    Window rootWindow = RootWindow(display, 0);
    XEvent xEvent;
    memset(&xEvent, 0, sizeof(xEvent));
    xEvent.type = ClientMessage;
    xEvent.xclient.display = display;
    xEvent.xclient.window = rootWindow;
    xEvent.xclient.message_type
            = XInternAtom(display, currentDesktopProperty, false);
    XSendEvent(display, rootWindow, false,
            SubstructureNotifyMask | SubstructureRedirectMask,
            &xEvent);
}


// Gets the index of the desktop that contains a specific window.
int Windows::XInterface::getWindowDesktop(const Window window) const
{
    if (!xPropertySupported(windowDesktopProperty))
    {
        return -1;
    }
    Atom request = XInternAtom(display, windowDesktopProperty, false);
    WindowProperty desktopProp = getWindowProperty(window, request);
    if (desktopProp.numItems == 0 || desktopProp.size == 0
            || desktopProp.data == nullptr)
    {
        return -1;
    }
    return (int) desktopProp.getDataValue<long>();
}

#if JUCE_DEBUG
// Prints comprehensive debug information about a window.
void Windows::XInterface::printWindowInfo(const Window window) const
{
    using juce::String;
    String id((unsigned long) window);
    String name = getWindowName(window);
    String claz = getWindowClass(window);
    String className = getWindowClassName(window);
    int desktop = getWindowDesktop(window);
    int pid = getWindowPID(window);
    juce::Array<Window> parents = getWindowAncestry(window);

    std::cout << "Window " << id << ": " << name << "[class " << claz
            << ", className " << className << "]\n";
    std::cout << "\tDepth: " << parents.size() << ", Process: " << pid
            << ", Desktop: " << desktop << "\n";

    XWindowAttributes attr;
    int foundAttributes = XGetWindowAttributes(display, window, &attr);
    if (foundAttributes == 0)
    {
        std::cout << "\tUnable to read window attributes\n";
        return;
    }
    std::cout << "\tPosition: (" << attr.x << "," << attr.y << "), size: ("
            << attr.width << "," << attr.height << ")\n";
}


// Recursively prints the entire window tree under some root window, from back
// to front.
void Windows::XInterface::printWindowTree(Window root, const int depth) const
{
    using juce::String;
    if (root == 0 && depth == 0)
    {
        root = XDefaultRootWindow(display);
        std::cout << "\nApplication window ID: " << (int) getMainAppWindow()
                << "\nFull window tree:\n";
    }
    String output;
    for (int i = 0; i < depth; i++)
    {
        output += "\t";
    }
    output += String((unsigned long) root);

    String title = getWindowName(root);
    if (title.isNotEmpty())
    {
        output += ":";
        output += title;
    }
    String winClass = getWindowClass(root);
    String className = getWindowClassName(root);
    if (winClass.isNotEmpty() || className.isNotEmpty())
    {
        output += "[";
        output += winClass;
        if (winClass.isNotEmpty() && className.isNotEmpty())
        {
            output += ", ";
        }
        output += className;
        output += "]";
    }
    std::cout << output << std::endl;
    juce::Array<Window> children = getWindowChildren(root);
    for (int i = 0; i < children.size(); i++)
    {
        printWindowTree(children[i], depth + 1);
    }
}
#endif

// Recursively searches for a specific window's ancestry.
juce::Array<Window> Windows::XInterface::recursiveWindowSearch
(const juce::Array<Window>& parents, const Window searchWin) const
{
    const Window lastParent = parents[parents.size() - 1];
    juce::Array<Window> expandedParents;
    juce::Array<Window> children = getWindowChildren(lastParent);
    for (const Window& child : children)
    {
        expandedParents = parents;
        expandedParents.add(child);
        if (child == searchWin)
        {
            break;
        }
        expandedParents = recursiveWindowSearch(expandedParents, searchWin);
        if (!expandedParents.isEmpty())
        {
            break;
        }
    }
    return expandedParents;
}


// Gets an arbitrary window property.
Windows::XInterface::WindowProperty Windows::XInterface::getWindowProperty
(const Window window, const Atom property) const
{
    unsigned long bytesAfter; // needed for XGetWindowProperty, result unused.
    Windows::XInterface::WindowProperty propertyData;
    int status = XGetWindowProperty(display, window, property, 0, (~0L), false,
            AnyPropertyType, &propertyData.type, &propertyData.size,
            &propertyData.numItems, &bytesAfter, &propertyData.data);
    if (status != Success)
    {
        if (status == BadWindow)
        {
            DBG(dbgPrefix << __func__ << ": Window "
                    << juce::String((unsigned long) window)
                    << "does not exist!");
        }
        DBG(dbgPrefix << __func__ << ": XGetWindowProperty failed "
                << "to find property " << XGetAtomName(display, property));
        propertyData = WindowProperty();
    }
    return propertyData;
}


// Checks if a particular property is supported by the window manager.
bool Windows::XInterface::xPropertySupported(const char* property) const
{
    Window rootWindow = XDefaultRootWindow(display);
    Atom featureList = XInternAtom(display, supportedFeatureProperty, false);
    WindowProperty supportedPropertyList
            = getWindowProperty(rootWindow, featureList);
    if (supportedPropertyList.data == nullptr
        || supportedPropertyList.numItems == 0)
    {
        return false;
    }
    Atom* propertyList = reinterpret_cast<Atom*>(supportedPropertyList.data);
    Atom neededProp = XInternAtom(display, property, false);
    for (long i = 0; i < supportedPropertyList.numItems; i++)
    {
        if (propertyList[i] == neededProp)
        {
            return true;
        }
    }
    return false;
}
