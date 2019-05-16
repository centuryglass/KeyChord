#include "SharedResource_ReferenceInterface.h"

// Gets the lock used to control this reference.
juce::CriticalSection& SharedResource::ReferenceInterface::getLock()
{
    return lock;
}
