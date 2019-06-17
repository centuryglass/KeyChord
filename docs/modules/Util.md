# Util Module Documentation
The Util module provides miscellaneous support classes and functions used to build the application.

#### [Util\::Math](../../Source/Framework/Util/Util_Math.h)
The Util\::Math namespace is meant to provide miscellaneous mathematical functions. This currently only contains a template function for finding the median of three values.

#### [Util\::ConditionChecker](../../Source/Framework/Util/Util_ConditionChecker.h)
 ConditionChecker objects handle tasks that require waiting for an event that will occur after an indeterminate delay or not at all. They periodically check for a condition to be true, then run a callback function once it is.
