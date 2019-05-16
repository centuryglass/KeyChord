#pragma once
/**
 * @file  SharedResource_Thread_Resource.h
 *
 * @brief  An abstract basis for SharedResource Thread Resource objects.
 */

#include "SharedResource_Handler.h"
#include "SharedResource_Resource.h"
#include "SharedResource_Thread_Thread.h"

namespace SharedResource { namespace Thread { class Resource; } }
namespace SharedResource { namespace Thread { class Lock; } }

/**
 * @brief  Manages a single child thread using the SharedResource control
 *         pattern.
 *
 *  ThreadResource threads may be accessed through any of their Handler objects,
 * and will only be destroyed when all of their handlers are destroyed.
 *
 *  ThreadResource objects guarantee that they will not be deleted while their
 * thread still runs. While running within their own thread, ThreadResource
 * objects are able to access their own resource locks to prevent handlers from
 * modifying their data.
 */
class SharedResource::Thread::Resource : public SharedResource::Resource,
        public Thread
{
public:
    /**
     * @brief  Creates a new Thread::Resource.
     *
     * @param resourceKey  The thread's unique SharedResource object key.
     *
     * @param threadName   The name used to identify the thread to the system.
     */
    Resource(const juce::Identifier& resourceKey,
            const juce::String& threadName);

    virtual ~Resource() { }

protected:
    /**
     * @brief  Gets the Thread::Resource's resource key.
     *
     * @return  The unique SharedResource instance key.
     */
    virtual const juce::Identifier& getThreadResourceKey() const override;

    /**
     * @brief  Gets the number of references connected to this Thread::Resource.
     *
     * @return   The Resource reference count.
     */
    virtual int getThreadReferenceCount() const override;
};
