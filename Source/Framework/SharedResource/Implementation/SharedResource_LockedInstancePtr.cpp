#define SHARED_RESOURCE_IMPLEMENTATION
#include "SharedResource_LockedInstancePtr.h"
#include "SharedResource_Holder.h"

// Initializes the resource pointer, locking the resource.
SharedResource::LockedInstancePtr::LockedInstancePtr
(const juce::Identifier& resourceKey, const LockType lockType) :
resourceKey(resourceKey),
lockType(lockType)
{
    const juce::ReadWriteLock& resourceLock
        = Holder::getHolderInstance()->getResourceLock(resourceKey);
    if (lockType == LockType::read)
    {
        resourceLock.enterRead();
    }
    else
    {
        resourceLock.enterWrite();
    }
    locked = true;
}


// Unlocks the resource when the LockedInstancePtr is destroyed.
SharedResource::LockedInstancePtr::~LockedInstancePtr()
{
    unlock();
}


// Unlocks the resource. Once the resource is unlocked, the LockedInstancePtr
// may no longer access the resource Instance or lock.
void SharedResource::LockedInstancePtr::unlock()
{
    if (locked)
    {
        const juce::ReadWriteLock& resourceLock
            = Holder::getHolderInstance()->getResourceLock(resourceKey);
        if (lockType == LockType::read)
        {
            resourceLock.exitRead();
        }
        else
        {
            resourceLock.exitWrite();
        }
        locked = false;
    }
}


// Checks if the resource is still locked by this pointer.
bool SharedResource::LockedInstancePtr::isLocked() const
{
    return locked;
}


// Accesses the resource Instance's functions or data.
SharedResource::Instance*
SharedResource::LockedInstancePtr::getInstance() const
{
    if (locked)
    {
        return Holder::getHolderInstance()->getResource(resourceKey);
    }
    else
    {
        return nullptr;
    }
}
