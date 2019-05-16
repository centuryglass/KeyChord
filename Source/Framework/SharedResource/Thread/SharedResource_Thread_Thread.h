#pragma once
/**
 * @file  SharedResource_Thread_Thread.h
 *
 * @brief  An abstract basis for SharedResource Thread Resource and Module
 *         objects.
 */

#include "SharedResource_Thread_Lock.h"
#include "Util_ShutdownListener.h"
#include <condition_variable>
#include <mutex>

namespace SharedResource { namespace Thread { class Thread; } }

/**
 * @brief  Provides an abstract basis for Resource or Module classes that
 *         create and manage a thread.
 *
 *  It provides a single thread interface shared by Thread::Resource and
 * Thread::Module objects. Thread objects selectively lock themselves to
 * prevent concurrent data access, and stop themselves when the application
 * starts to shut down.
 */
class SharedResource::Thread::Thread : public Util::ShutdownListener,
        private juce::Thread
{
public:
    /**
     * @brief  Creates the Thread object without starting the thread.
     *
     * @param name  The name used to identify the thread.
     */
    Thread(const juce::String name);

    /**
     * @brief  Checks that the thread has successfully stopped before
     *         destruction.
     */
    virtual ~Thread();

    /**
     * @brief  Starts the Thread object if it isn't already running.
     */
    virtual void startResourceThread();

    /**
     * @brief  Performs all necessary steps to stop the thread.
     */
    virtual void stopResourceThread();

    /**
     * @brief  Stops the Thread object, waiting until the thread completely
     *         stops running and starts to delete its ThreadLock.
     */
    void stopThreadAndWait();

    // Checks if the resource's thread is currently running.
    using juce::Thread::isThreadRunning;

    // Allow handlers to notify the thread normally to wake it when waiting.
    using juce::Thread::notify;

protected:
    // Allow thread subclasses limited access to the juce::Thread interface:
    using juce::Thread::signalThreadShouldExit;
    using juce::Thread::threadShouldExit;
    using juce::Thread::getThreadId;

    /**
     * @brief  Gets the Thread::Resource's resource key.
     *
     * @return  The unique SharedResource instance key.
     */
    virtual const juce::Identifier& getThreadResourceKey() const = 0;

    /**
     * @brief  Gets the number of references connected to this Thread.
     *
     * @return   The thread Resource or Module reference count.
     */
    virtual int getThreadReferenceCount() const = 0;

private:
    /**
     * @brief  Runs once each time the thread starts running.
     *
     *  Override this function to define custom initialization routines for a
     * Thread subclass.
     *
     * @param lock  The Thread's resource lock object.
     */
    virtual void init(Lock& lock) { }

    /**
     * @brief  The main thread action loop. When the thread is running, this
     *         will continually run until something stops the thread.
     *
     * @param lock  Grants access to the thread's SharedResource lock within
     *              the loop. This should not be saved or shared outside the
     *              thread.
     */
    virtual void runLoop(Lock& lock) = 0;

    /**
     * @brief  Runs once each time the thread stops running.
     *
     * Override this to define custom cleanup routines for a Thread subclass.
     *
     * @param lock  The Thread's resource lock object.
     */
    virtual void cleanup(Lock& lock) { }

    /**
     * @brief  Sets if the thread should wait until the next notification to
     *         resume, running the cleanup function and removing the Lock until
     *         the Thread is notified.
     *
     * @return  Whether the thread should wait for a notification before
     *          resuming.
     */
    virtual bool threadShouldWait() { return false; }

    /**
     * @brief  Initializes the thread, runs the action loop, then runs cleanup
     *         routines before the thread exits.
     */
    virtual void run() final override;

    /**
     * @brief  Ensures the thread is stopped when the application starts to
     *         shut down.
     */
    virtual void onShutdown() override;

    // Stores the thread's name:
    const juce::String threadName;

    // Used when waiting for the thread to start:
    std::mutex startMutex;
    std::condition_variable startCondition;

    // Used when waiting for the thread to stop:
    std::mutex stopMutex;
    std::condition_variable stopCondition;

    // Preserves the thread object and controls access while the thread runs:
    std::unique_ptr<Lock> threadLock = nullptr;
};
