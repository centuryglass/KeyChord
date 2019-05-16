# Windows Module Documentation
The Windows module creates, finds, tracks, and controls open windows.

#### [Windows::XInterface](../../Source/System/Windows/Windows_XInterface.h)
XInterface objects interact with the X Window System to find and manipulate windows.

#### [Windows::MainWindow](../../Source/System/Windows/Windows_MainWindow.h)

#### [Windows::FocusTracker](../../Source/System/Windows/Windows_FocusTracker.h)
FocusTracker is a [SharedResource](./SharedResource.md_ used to track the window focus state and signal to all FocusListener objects when the window gains or loses focus.

#### [Windows::FocusListener](../../Source/System/Windows/Windows_FocusListener.h)
FocusListener objects connect to the FocusTracker resource to check if the MainWindow is focused, and to receive updates when the window gains or loses focus.

#### [Windows::FocusInterface](../../Source/System/Windows/Windows_FocusInterface.h)
FocusInterface is an abstract interface inherited by FocusListener, used by the FocusTracker to send out window focus updates.

#### [Windows::FocusedTimer](../../Source/System/Windows/Windows_FocusedTimer.h)
 FocusedTimer is a juce::Timer class that only executes its timer callback function when the main application window is focused.

#### [Windows::Info](../../Source/System/Windows/Windows_Info.h)
The Windows\::Info namespace provides functions for getting information about the main application window.

#### [Windows::Alert](../../Source/System/Windows/Windows_Alert.h)
The Windows\::Alert namespace provides convenience functions for creating simple popup alert windows.

