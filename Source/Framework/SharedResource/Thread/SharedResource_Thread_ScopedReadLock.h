#pragma once
/**
 * @file  SharedResource_Thread_ScopedReadLock.h
 *
 * @brief  Locks a SharedResource::Thread::Lock for reading while it remains in
 *         scope.
 */

#include "JuceHeader.h"

namespace SharedResource
{
    namespace Thread
    {
        class ScopedReadLock;
        class Lock;
    }
}

/**
 * @brief  A juce::ScopedReadLock created from a Thread::Lock instead of a
 *         juce::ReadWriteLock.
 */
class SharedResource::Thread::ScopedReadLock
{
public:
    /**
     * @brief  Locks a resource lock for reading for as long as this object
     *         exists.
     *
     * @param threadLock  The resource lock object to lock.
     */
    ScopedReadLock(Lock& threadLock);

    virtual ~ScopedReadLock() { }

private:
    // Private lock object that provides all lock functionality:
    juce::ScopedReadLock juceLock;
};
