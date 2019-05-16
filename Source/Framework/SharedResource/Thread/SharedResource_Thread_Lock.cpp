#include "SharedResource_Thread_Lock.h"
#include "SharedResource_Resource.h"

// Creates a Lock tied to a single thread resource.
SharedResource::Thread::Lock::Lock(const juce::Identifier& resourceKey) :
    Handler(resourceKey, []()->Resource* { return nullptr; }) { }


// Blocks the thread until it can be locked for reading.
void SharedResource::Thread::Lock::enterRead() const
{
    getResourceLock().enterRead();
}


// Blocks the thread until it can be locked for writing.
void SharedResource::Thread::Lock::enterWrite() const
{
    getResourceLock().enterWrite();
}


// Releases a read lock held by this thread. This must be called once for each
// call to takeReadLock.
void SharedResource::Thread::Lock::exitRead() const
{
    getResourceLock().exitRead();
}


// Releases a read lock held by this thread. This must be called once for each
// call to takeWriteLock.
void SharedResource::Thread::Lock::exitWrite() const
{
    getResourceLock().exitWrite();
}


// Attempts to lock the thread for reading, without blocking the calling thread
// if the lock can't be acquired.
bool SharedResource::Thread::Lock::tryEnterRead() const
{
    return getResourceLock().tryEnterRead();
}


// Attempts to lock the thread for writing, without blocking the calling thread
// if the lock can't be acquired.
bool SharedResource::Thread::Lock::tryEnterWrite() const
{
    return getResourceLock().tryEnterWrite();
}
