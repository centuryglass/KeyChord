#include "SharedResource_Thread_Thread.h"

// Number of milliseconds to wait before forcibly terminating the thread:
static const constexpr int timeoutMilliseconds = 5000;

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "SharedResource::Thread::Thread::";
#endif

namespace ThreadResource = SharedResource::Thread;

// Creates the Thread object without starting the thread.
ThreadResource::Thread::Thread(const juce::String name) :
        juce::Thread(name) { }


// Checks that the thread has successfully stopped before destruction.
ThreadResource::Thread::~Thread()
{
    // The Thread object should only be destroyed with its associated resource,
    // and its resource should never be destroyed while the thread is running.

    // If the thread is running at this step, that means that the Thread object
    // was destroyed early, the Resource was destroyed before all of its
    // Reference objects were destroyed, or the thread is restarting when it
    // should have been stopped for the last time.
    jassert(!isThreadRunning());
}


// Starts the Thread object's thread if it isn't already running.
void ThreadResource::Thread::startResourceThread()
{
    std::unique_lock<std::mutex> startLock(startMutex);
    if (!isThreadRunning())
    {
        // Make sure a previous Lock instance doesn't still exist:
        {
            std::unique_lock<std::mutex> stopLock(stopMutex);
            if (threadLock != nullptr)
            {
                stopCondition.wait(stopLock);
            }
        }
        // Create the thread lock before starting the thread:
        threadLock.reset(new Lock(getThreadResourceKey()));
        startThread();
        startCondition.wait(startLock);
    }
}


// Performs all necessary steps to stop the thread, and waits for the thread to
// exit.
void ThreadResource::Thread::stopResourceThread()
{
    // Don't try to stop the thread while running on the thread:
    jassert(Thread::getCurrentThreadId() != getThreadId());
    if (isThreadRunning())
    {
        signalThreadShouldExit();
        notify();
    }
}


// Stops the thread resource, waiting until the thread completely stops running
// and deletes its ThreadLock.
void ThreadResource::Thread::stopThreadAndWait()
{
    std::unique_lock<std::mutex> stopLock(stopMutex);
    if (isThreadRunning() || threadLock != nullptr)
    {
        stopResourceThread();
        stopCondition.wait(stopLock);
    }
}


// Initializes the thread, runs the action loop, then runs cleanup routines
// before the thread exits.
void ThreadResource::Thread::run()
{
    jassert(threadLock != nullptr);
    // Notify any callers waiting for the thread to start:
    {
        std::unique_lock<std::mutex> startLock(startMutex);
        startCondition.notify_all();
    }

    DBG(dbgPrefix << __func__ << ": Initializing thread \"" << getThreadName()
            << "\".");
    init(*threadLock);

    DBG(dbgPrefix<< __func__ << ": Thread \"" << getThreadName()
            << "\" running main action loop.");
    while (!threadShouldExit())
    {
        runLoop(*threadLock);

        // If the threadLock is the thread's only reference, start shutting it
        // down:
        if (getThreadReferenceCount() == 1)
        {
            signalThreadShouldExit();
        }

        if (threadShouldWait() && !threadShouldExit())
        {
            DBG(dbgPrefix << __func__ << ": Thread \"" << getThreadName()
                    << "\" running cleanup before waiting.");
            cleanup(*threadLock);
            // If the thread was notified while it was not waiting, calling
            // wait will immediately return and clear the saved notification.
            // There's no way to tell if the thread is in this state, so notify
            // and wait are called now so that we know the next wait call will
            // function correctly.
            notify();
            bool resetThread = wait(-1);
            jassert(resetThread);
            wait(-1);
            DBG(dbgPrefix << __func__ << ": Thread \"" << getThreadName()
                    << "\" waking and re-initializing.");
            init(*threadLock);
        }
    }

    DBG(dbgPrefix << __func__ << ": Running cleanup for thread \""
            << getThreadName() << "\".");
    cleanup(*threadLock);

    DBG(dbgPrefix << __func__ << ": Thread \"" << getThreadName()
            << "\" finished running.");

    // Delete the lock outside of the thread, just in case deleting the lock
    // also deletes the thread.
    std::function<void()> lockDelete = [this]()
    {
        // Make sure the thread has finished running before doing anything
        // else:
        if (threadShouldExit())
        {
            DBG(dbgPrefix << __func__ << ": Thread \"" << getThreadName()
                    << "\" hasn't finished stopping yet, wait for it.");
            notify();
            waitForThreadToExit(-1);
        }

        // Transfer the threadLock to a local unique_ptr while the stopLock is
        // held, just in case the another thread is trying to replace it:
        std::unique_ptr<Lock> tempLockHolder(nullptr);
        {
            std::unique_lock<std::mutex> stopLock(stopMutex);
            tempLockHolder.swap(threadLock);

            // Before deleting the ThreadLock, threads waiting on the
            // stopCondition must be notified, and this function must release
            // its lock on the Thread object's stopMutex. Otherwise, the thread
            // will freeze if deleting the ThreadLock causes the Thread object
            // to be deleted.
            stopCondition.notify_all();
        }

        DBG(dbgPrefix << __func__ << ": Asynchronously deleting ThreadLock on"
                << " stopped thread \"" << getThreadName() << "\"");

        // The threadLock will now be deleted automatically when tempLockHolder
        // goes out of scope and is destroyed.
    };

    // In most circumstances, the juce::MessageManager thread is used to handle
    // asynchronous function calls. In this case, it shouldn't be used, as it
    // might be stopping or waiting for the lock to be deleted.
    juce::Thread::launch(lockDelete);
}


// Ensures the thread is stopped when the application starts to shut down.
void ThreadResource::Thread::onShutdown()
{
    stopThreadAndWait();
}
