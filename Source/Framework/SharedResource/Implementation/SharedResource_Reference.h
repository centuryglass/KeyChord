#pragma once
/**
 * @file  SharedResource_Reference.h
 *
 * @brief  Provides access to a Resource object's Instance and lock, and
 *         ensures the resource is destroyed if and only if all of its
 *         References have been destroyed.
 */

#include "JuceHeader.h"
#include "SharedResource_ReferenceInterface.h"

namespace SharedResource
{
    class Reference;
    class Instance;
}

/**
 * @brief  An object that serves as a reference to a specific resource Instance.
 */
class SharedResource::Reference : public ReferenceInterface
{
private:
    /**
     * @brief  Connects this new Reference to its resource Instance, creating
     *         the Instance object if necessary.
     *
     * @param resourceKey     The key identifying this object's resource.
     *
     * @param createResource  A function that can be used to create the
     *                        Instance if necessary.
     */
    Reference(const juce::Identifier& resourceKey,
            const std::function<Instance*()> createResource);
public:
    /**
     * @brief  Removes this Reference from its resource Instance object,
     *         destroying the resource if no references to it remain.
     */
    virtual ~Reference();

    // Reference objects may only be created as the parent class of a
    // Handler<ResourceType> object.
    template<class ResourceType> friend class Handler;

protected:
    /**
     * @brief  Gets the lock used to control access to the referenced resource.
     *
     * @return  The resource's shared lock.
     */
    const juce::ReadWriteLock& getResourceLock() const;

private:
    /**
     * @brief  Gets a pointer to this reference's resource object Instance.
     *
     * @return  The resource's unique object instance.
     */
    Instance* getResourceInstance() const;

    // The resource's unique key identifier.
    const juce::Identifier& resourceKey;
};
