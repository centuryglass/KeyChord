#pragma once
/**
 * @file  SharedResource_LockedPtr.h
 *
 * @brief  Provides access to a specific SharedResource::Resource, keeping it
 *         locked for as long as the LockedPtr exists.
 */

#include "SharedResource_LockedInstancePtr.h"
#include "SharedResource_LockType.h"

namespace SharedResource { template<class ResourceType> class LockedPtr; }

/**
 * @brief  A scoped lock object that can be used to access the resource object
 *         it locks.
 *
 * @tparam ResourceType  The SharedResource::Resource subclass this LockedPtr
 *                       accesses.
 */
template<class ResourceType>
class SharedResource::LockedPtr : public LockedInstancePtr
{
private:
    // LockedPtr objects should only be created and used internally by
    // Handler objects.
    template <class LockType> friend class Handler;

    /**
     * @brief  Locks the ResourceType resource for as long as the LockedPtr
     *         exists.
     *
     * @param resourceKey  The unique key string identifying the ResourceType
     *                     Resource class.
     *
     * @param lockType     The type of lock used to secure the resource.
     */
    LockedPtr(const juce::Identifier& resourceKey, const LockType lockType) :
        LockedInstancePtr(resourceKey, lockType) { }

public:
    virtual ~LockedPtr() { }

    /**
     * @brief  Accesses the locked resource's functions or data.
     *
     * @return  The address of the locked resource object instance.
     */
    ResourceType* operator->() const
    {
        return static_cast<ResourceType*>(getInstance());
    }
};

