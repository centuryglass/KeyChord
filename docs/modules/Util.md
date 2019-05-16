# Util Module Documentation
The Util module provides miscellaneous support classes and functions used to build the application.

#### [Util\::Commands](../../Source/Framework/Util/Util_Commands.h)
The Commands class loads and runs system commands. System commands are defined in a read-only JSON file, so they can be updated if necessary without recompiling. Due to the limitations of the juce\::ChildProcess class, commands are divided into commands returning text output, commands that return an integer exit code, and commands that act without returning any value.

#### [Util\::CommandTypes](../../Source/Framework/Util/Util_CommandTypes.h)
CommandTypes defines all system command types that Commands objects can execute.

#### [Util\::Nullable](../../Source/Framework/Util/Util_Nullable.h)
Nullable is an abstract container class, defining objects that hold a single value that may equal null.

#### [Util\::Math](../../Source/Framework/Util/Util_Math.h)
The Util\::Math namespace is meant to provide miscellaneous mathematical functions. This currently only contains a template function for finding the median of three values.

#### [Util\::SafeCall](../../Source/Framework/Util/Util_SafeCall.h)
Util\::SafeCall provides a template function for safely running asynchronous calls on juce::Component objects within the JUCE message thread.

#### [Util\::TempTimer](../../Source/Framework/Util/Util_TempTimer.h)
TempTimer creates single-use timer objects that execute a function after a specific number of milliseconds.

#### [Util\::ShutdownListener](../../Source/Framework/Util/Util_ShutdownListener.h)
ShutdownListener is an abstract basis for classes that need to perform an action before the application shuts down.

#### [Util\::ConditionChecker](../../Source/Framework/Util/Util_ConditionChecker.h)
 ConditionChecker objects handle tasks that require waiting for an event that will occur after an indeterminate delay or not at all. They periodically check for a condition to be true, then run a callback function once it is.
