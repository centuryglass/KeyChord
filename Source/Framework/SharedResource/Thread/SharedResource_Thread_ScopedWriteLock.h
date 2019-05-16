#pragma once
/**
 * @file  SharedResource_Thread_ScopedWriteLock.h
 *
 * @brief  Locks a SharedResource::Thread::Lock for writing while it remains in
 *         scope.
 */

#include "JuceHeader.h"

namespace SharedResource
{
    namespace Thread
    {
        class ScopedWriteLock;
        class Lock;
    }
}

/**
 * @brief  A juce::ScopedWriteLock created from a Thread::Lock instead of a
 *         juce::ReadWriteLock.
 */
class SharedResource::Thread::ScopedWriteLock
{
public:
    /**
     * @brief  Locks a resource lock for writing for as long as this object
     *         exists.
     *
     * @param threadLock  The resource lock object to lock.
     */
    ScopedWriteLock(Lock& threadLock);

    virtual ~ScopedWriteLock() { }

private:
    // Private lock object that provides all lock functionality:
    juce::ScopedWriteLock juceLock;
};
