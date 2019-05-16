#pragma once

#include "SharedResource_Thread_Thread.h"
#include "SharedResource_Thread_Lock.h"

#pragma once
/**
 * @file  SharedResource_Thread_Module.h
 *
 * @brief  An abstract basis for SharedResource Thread Module objects.
 */

#include "SharedResource_Handler.h"
#include "SharedResource_Resource.h"
#include "SharedResource_Modular_Module.h"
#include "SharedResource_Thread_Thread.h"
#include "SharedResource_Thread_Lock.h"

namespace SharedResource { namespace Thread {
        template <class ResourceType> class Module; } }

/**
 * @brief  Manages a single child thread using the SharedResource Module
 *         control pattern.
 *
 *  Module threads may be accessed through any of their Handler objects or any
 * of their sibling modules, and will only be destroyed when all of their
 * resource's handlers are destroyed.
 *
 *  Thread::Module objects guarantee that they will not be deleted while their
 * thread still runs. While running within their own thread, Module objects are
 * able to access their own resource locks to prevent handlers from modifying
 * their data.
 *
 * @tparam ResourceType  The subclass of SharedResource::Modular::Resource that
 *                       will manage this Module
 */
template <class ResourceType>
class SharedResource::Thread::Module : public Modular::Module<ResourceType>,
        public Thread
{
public:
    /**
     * @brief  Creates a new Thread::Module.
     *
     * @param resource    The Module's parent Resource instance.
     *
     * @param threadName  The name used to identify the thread to the system.
     */
    Module(ResourceType& resource, const juce::String& threadName) :
    Modular::Module<ResourceType>(resource), Thread(threadName) { }

    virtual ~Module() { }

protected:
    /**
     * @brief  Gets the Thread::Resource's resource key.
     *
     * @return  The unique SharedResource instance key.
     */
    virtual const juce::Identifier& getThreadResourceKey() const override
    {
        return ResourceType::resourceKey;
    }

    /**
     * @brief  Gets the number of references connected to this Thread::Module
     *         object's Resource.
     *
     * @return   The Resource reference count.
     */
    virtual int getThreadReferenceCount() const override
    {
        return Module<ResourceType>::getResource().getReferenceCount();
    }
};
