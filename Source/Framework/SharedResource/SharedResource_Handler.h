#pragma once
/**
 * @file  SharedResource_Handler.h
 *
 * @brief  Provides safe access to a specific SharedResource::Resource
 *         subclass, ensuring the single Resource object instance exists as
 *         long as any of its Handlers exists, destroying it when its last
 *         handler is destroyed.
 */

#include "SharedResource_LockedPtr.h"
#include "SharedResource_Reference.h"
#include "JuceHeader.h"

namespace SharedResource { template<class ResourceType> class Handler; }

/**
 * @brief  A reference object used to securely access a specific resource.
 *
 * @tparam ResourceType  The specific SharedResource::Resource subclass managed
 *                       by the ResourceHandler.
 */
template <class ResourceType>
class SharedResource::Handler : public SharedResource::Reference
{
protected:
    /**
     * @brief  Creates a new Handler, initializing its Resource if necessary.
     *
     *  This constructor determines the resource key directly from the
     * ResourceType, and should not be used to implement Handlers for abstract
     * Resource subclasses.
     *
     * @param createResource  An optional function that will create the
     *                        resource. If no function is given, the default
     *                        constructor for ResourceType will be used.
     */
    Handler(const std::function<ResourceType*()> createResource
            = [](){ return new ResourceType(); }) :
    Reference(ResourceType::resourceKey, [createResource]()->Instance*
    {
        return static_cast<Instance*>(createResource());
    }),
    resourceKey(ResourceType::resourceKey) { }

    /**
     * @brief  Creates a new Handler, initializing its Resource if necessary.
     *
     *  This constructor should be used when implementing Handlers for abstract
     * Resource subclasses, where the resource key cannot be determined
     * automatically.
     *
     * @param resourceKey     The unique key string identifying the
     *                        ResourceType Resource class.
     *
     * @param createResource  An optional function that will create the
     *                        resource. If no function is given, the default
     *                        constructor for ResourceType will be used.
     */
    Handler(const juce::Identifier& resourceKey,
            const std::function<ResourceType*()> createResource
                = [](){ return new ResourceType(); }) :
    Reference(resourceKey, [createResource]()->Instance*
    {
        return static_cast<Instance*>(createResource());
    }),
    resourceKey(resourceKey) { }

public:
    virtual ~Handler() { }

protected:
    /**
     * @brief  Gets a pointer to the class resource, locking it for reading.
     *
     *  Handlers should use this to access their Resource whenever they need to
     * read data from it without changing it.
     *
     * @tparam LockedType  Optionally specifies a different class pointer type
     *                     that the LockedPtr should use to represent the
     *                     resource. This type must be a valid class of the
     *                     resource object instance.
     *
     * @return             A read-locked pointer to the class Resource instance.
     */
    template <class LockedType = ResourceType>
    LockedPtr<const LockedType> getReadLockedResource() const
    {
        return LockedPtr<const LockedType> (resourceKey, LockType::read);
    }

    /**
     * @brief  Gets a pointer to the class resource, locking it for writing.
     *
     *  Handlers should use this to access their Resource whenever they need to
     * write to(or otherwise change) the resource.
     *
     * @tparam LockedType  Optionally specifies a different class pointer type
     *                     that the LockedPtr should use to represent the
     *                     resource. This type must be a valid class of the
     *                     resource object instance.
     *
     * @return             A write-locked pointer to the class Resource
     *                     instance.
     */
    template <class LockedType = ResourceType>
    LockedPtr<LockedType> getWriteLockedResource() const
    {
        return LockedPtr<LockedType>(resourceKey, LockType::write);
    }

    /**
     * @brief  Gets the key that identifies this Handler object's resource.
     *
     * @return  The unique key used to select which resource this Handler will
     *          access.
     */
    const juce::Identifier& getResourceKey() const
    {
        return resourceKey;
    }

private:
    // The unique key identifier used to find the connected Resource instance.
    const juce::Identifier& resourceKey;
};
