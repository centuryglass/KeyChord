#define SHARED_RESOURCE_IMPLEMENTATION
#include "SharedResource_Holder.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "SharedResource::Holder::";
#endif

// Holds the unique SharedResource::Holder instance
static std::unique_ptr<SharedResource::Holder> holderInstance;

// Prevents concurrent access to the containers holding resource data.
juce::CriticalSection SharedResource::Holder::holderLock;

SharedResource::Holder::Holder()
{
    DBG(dbgPrefix << __func__ << ": Creating SharedResource instance Holder ");
}


// Clears all resource locks.
SharedResource::Holder::~Holder()
{
    const juce::ScopedLock cleanupLock(holderLock);
    DBG(dbgPrefix << __func__ << ": Destroying resource Instance Holder ");
#ifdef JUCE_DEBUG
    // Test that all resources were properly destroyed. Claim all resource
    // locks for writing in case a resource is still being deleted.
    for (int i = 0; i < resourceList.size(); i++)
    {
        const juce::ScopedWriteLock resourceLock(*resourceLocks[i]);
        if (resourceList[i] != nullptr)
        {
            juce::String resourceName = "[Missing ID!]";
            for (auto iter : resourceIndices)
            {
                if (iter.second == i)
                {
                    resourceName = iter.first.toString();
                    break;
                }
            }
            DBG(dbgPrefix << __func__ << ": Resource " << resourceName
                    << " was not destroyed!");
            jassertfalse;
        }
    }
    jassert(resourceIndices.size() == resourceList.size()
            && resourceList.size() == resourceLocks.size());
#endif
    resourceIndices.clear();
    resourceList.clear();
    resourceLocks.clear();
}


// Gets the program's sole SharedResource::Holder instance.
SharedResource::Holder* SharedResource::Holder::getHolderInstance()
{
    const juce::ScopedLock initLock(holderLock);
    if (holderInstance == nullptr)
    {
        holderInstance.reset(new Holder());
    }
    return holderInstance.get();
}


// Deletes the holder instance if it is holding no valid Instance pointers.
void SharedResource::Holder::clearIfEmpty()
{
    if (holderInstance != nullptr)
    {
        for (Instance* resourceInstance : holderInstance->resourceList)
        {
            if (resourceInstance != nullptr)
            {
                return;
            }
        }
        holderInstance.reset(nullptr);
    }
}


// Finds and gets an Instance pointer using its resource key.
SharedResource::Instance* SharedResource::Holder::getResource
(const juce::Identifier& resourceKey)
{
    const juce::ScopedLock resourceLock(holderLock);
    const int resourceIndex = getResourceIndex(resourceKey);
    return holderInstance->resourceList.getUnchecked(resourceIndex);
}


// Saves the address of a new resource instance.
void SharedResource::Holder::setResource(const juce::Identifier& resourceKey,
        Instance* resource)
{
    const juce::ScopedLock resourceLock(holderLock);
    const int resourceIndex = getResourceIndex(resourceKey);
    Holder* const holder = getHolderInstance();

#ifdef JUCE_DEBUG
    // Make sure either a null resource is becoming non-null, or a
    // non-null resource is becoming null.
    if (resource == nullptr
            && holder->resourceList.getUnchecked(resourceIndex) == nullptr)
    {
        DBG(dbgPrefix << __func__ << ": Error, setting resource "
                << resourceKey.toString()
                << " to null when it is already null!");
        jassertfalse;
    }
    else if (resource != nullptr
            && holder->resourceList.getUnchecked(resourceIndex) != nullptr)
    {
        DBG(dbgPrefix << __func__ << ": Error, setting new resource "
                << resourceKey.toString() << " when it already exists!");
        jassertfalse;
    }
#endif
    holder->resourceList.set(resourceIndex, resource);
}


// Gets the lock used to control access to a resource instance.
const juce::ReadWriteLock&
SharedResource::Holder::getResourceLock(const juce::Identifier& resourceKey)
{
    const juce::ScopedLock resourceLock(holderLock);
    const int resourceIndex = getResourceIndex(resourceKey);
    return *getHolderInstance()->resourceLocks[resourceIndex];
}


// Gets the index where a resource and its lock are stored in the resourceList
// and resourceLocks arrays.
int SharedResource::Holder::getResourceIndex
(const juce::Identifier& resourceKey)
{
    if (getHolderInstance()->resourceIndices.count(resourceKey) == 0)
    {
        initResource(resourceKey);
    }
    return getHolderInstance()->resourceIndices.at(resourceKey);
}


// Initializes a resource container, assigning it an index, creating its lock,
// and setting the initial resource instance address as nullptr.
void SharedResource::Holder::initResource(const juce::Identifier& resourceKey)
{
    Holder* const holder = getHolderInstance();
    jassert(holder->resourceList.size() == holder->resourceLocks.size());
    holder->resourceIndices[resourceKey] = holder->resourceList.size();
    holder->resourceList.add(nullptr);
    holder->resourceLocks.add(new juce::ReadWriteLock());
}
