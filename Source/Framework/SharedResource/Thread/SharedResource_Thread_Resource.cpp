#include "SharedResource_Thread_Resource.h"

// Creates a new Thread::Resource.
SharedResource::Thread::Resource::Resource
(const juce::Identifier& resourceKey, const juce::String& threadName) :
SharedResource::Resource(resourceKey), Thread(threadName) { }


// Gets the Thread::Resource's resource key.
const juce::Identifier&
SharedResource::Thread::Resource::getThreadResourceKey() const
{
    return getResourceKey();
}


// Gets the number of references connected to this Thread::Resource.
int SharedResource::Thread::Resource::getThreadReferenceCount() const
{
    return getReferenceCount();
}
