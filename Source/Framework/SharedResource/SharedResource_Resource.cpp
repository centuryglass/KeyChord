#define SHARED_RESOURCE_IMPLEMENTATION
#include "SharedResource_Resource.h"
#include "SharedResource_Holder.h"

// Creates the single resource object instance.
SharedResource::Resource::Resource(const juce::Identifier& resourceKey) :
Instance(resourceKey) { }


// Packages an asynchronous action so that it will check if the SharedResource
// instance that created it still valid, and if so, ensure it remains valid
// while the action is executed.
std::function<void()> SharedResource::Resource::buildAsyncFunction(
        SharedResource::LockType lockType,
        std::function<void()> action,
        std::function<void()> ifDestroyed)
{
    const juce::Identifier& resKey = getResourceKey();
    return [this, lockType, resKey, action, ifDestroyed]()
    {
        Holder* resourceHolder = Holder::getHolderInstance();
        if (lockType == LockType::read)
        {
            juce::ScopedReadLock(resourceHolder->getResourceLock(resKey));
            if (this == resourceHolder->getResource(resKey))
            {
                action();
                return;
            }
        }
        else
        {
            juce::ScopedWriteLock(resourceHolder->getResourceLock(resKey));
            if (this == resourceHolder->getResource(resKey))
            {
                action();
                return;
            }
        }
        DBG("SharedResource::Resource: Cancelled async function, resource "
                << resKey.toString() << " was destroyed.");
        ifDestroyed();
    };
}
