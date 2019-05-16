#define SHARED_RESOURCE_IMPLEMENTATION
#include "SharedResource_Reference.h"
#include "SharedResource_Holder.h"
#include "SharedResource_Instance.h"

// Removes this Reference from its resource Instance object, destroying the
// resource if no references to it remain.
SharedResource::Reference::~Reference()
{
    {
        const juce::ScopedWriteLock resourceLock(getResourceLock());
        const juce::ScopedLock referenceLock(getLock());
        Instance* resourceInstance = getResourceInstance();
        jassert(resourceInstance != nullptr);
        resourceInstance->references.removeFirstMatchingValue(this);
        if (resourceInstance->references.isEmpty())
        {
            Holder::setResource(resourceKey, nullptr);
            delete resourceInstance;
            resourceInstance = nullptr;
        }
    }
    Holder::clearIfEmpty();
}


// Connects this new Reference to its resource Instance, creating the Instance
// object if necessary.
SharedResource::Reference::Reference(const juce::Identifier& resourceKey,
        const std::function<Instance*()> createResource) :
resourceKey(resourceKey)
{
    const juce::ScopedWriteLock initLock(getResourceLock());
    Instance* resourceInstance = getResourceInstance();
    if (resourceInstance == nullptr)
    {
        resourceInstance = createResource();
        jassert(getResourceInstance() != nullptr);
        jassert(resourceInstance->references[0] == nullptr);
        resourceInstance->references.set(0, this);
    }
    else
    {
        resourceInstance->references.add(this);
    }
}


// Gets the lock used to control access to the referenced resource.
const juce::ReadWriteLock& SharedResource::Reference::getResourceLock() const
{
    return Holder::getResourceLock(resourceKey);
}


// Gets a pointer to this reference's resource object Instance.
SharedResource::Instance*
SharedResource::Reference::getResourceInstance() const
{
    return Holder::getResource(resourceKey);
}
