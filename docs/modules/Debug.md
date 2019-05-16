# Debug Module Documentation
The Debug module provides tools for testing application performance and locating errors.

#### [Debug\::ScopeTimer](../../Source/Development/Debug/Debug_ScopeTimer.h)
ScopeTimer saves the time of its creation and the time it goes out of scope. This allows scope execution times to be recorded by declaring a ScopeTimer at the top of the scope.

#### [Debug\::ScopeTimerRecords](../../Source/Development/Debug/Debug_ScopeTimerRecords.h)
ScopeTimerRecords stores the records saved by all ScopeTimer objects, and prints them out when the application finishes executing.

#### [Debug\::Component](../../Source/Development/Debug/Debug_Component.h)
The Component namespace provides functions for inspecting and debugging juce\::Component objects. Currently, it includes a single function to print the tree of all visible components.

#### [Debug\::AddressLog](../../Source/Development/Debug/Debug_AddressLog.h)
The AddressLog keeps and prints logs of events associated with particular memory addresses.


