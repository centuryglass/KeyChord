#pragma once
/**
 * @file  SharedResource_Thread_Lock.h
 *
 * @brief  The interface used by object that allow thread resources and modules
 *         to lock themselves.
 */

#include "SharedResource_Handler.h"

namespace SharedResource { namespace Thread { class Lock; } }
namespace SharedResource { class Resource; }

/**
 * @brief  Accesses the resource's lock, and keeps the resource alive while the
 *         thread is running.
 */
class SharedResource::Thread::Lock : public Handler<Resource>
{
protected:
    // The Thread::Lock may only be created by the resource Thread object.
    friend class Thread;

    /**
     * @brief  Creates a Lock tied to a single thread resource.
     *
     * @param resourceKey  The thread resource's unique SharedResource object
     *                     key.
     */
    Lock(const juce::Identifier& resourceKey);

public:
    virtual ~Lock() { }

    /**
     * @brief  Blocks the thread until it can be locked for reading.
     *
     *  Once locked for reading, the thread will block any attempts to acquire a
     * write lock until all readers have released their locks. No changes may be
     * made to the ThreadResource's data members while this is locked for
     * reading.
     */
    void enterRead() const;

    /**
     * @brief  Blocks the thread until it can be locked for writing.
     *
     *  Once locked for writing, no other thread will be able to access the
     * ThreadResource's data members until the write lock has been released.
     */
    void enterWrite() const;

    /**
     * @brief  Releases a read lock held by this thread. This must be called
     *         once for each call to takeReadLock.
     */
    void exitRead() const;

    /**
     * @brief  Releases a read lock held by this thread. This must be called
     *         once for each call to takeWriteLock.
     */
    void exitWrite() const;

    /**
     * @brief  Attempts to lock the thread for reading, without blocking the
     *         calling thread if the lock can't be acquired.
     *
     * @return  Whether the lock was successfully acquired.
     */
    bool tryEnterRead() const;

    /**
     * @brief  Attempts to lock the thread for writing, without blocking the
     *         calling thread if the lock can't be acquired.
     *
     * @return  Whether the lock was successfully acquired.
     */
    bool tryEnterWrite() const;

private:
    // Allow the SharedResource::Thread scoped lock classes to access the
    // resource's ReadWriteLock.
    friend class ScopedReadLock;
    friend class ScopedWriteLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Lock);
};
