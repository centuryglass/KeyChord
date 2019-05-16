#ifndef SHARED_RESOURCE_IMPLEMENTATION
    #error File included directly outside of SharedResource implementation.
#endif
#pragma once
/**
 * @file  SharedResource_Holder.h
 *
 * @brief  Holds all resource Instance objects and resource locks.
 */

#include <map>
#include "JuceHeader.h"

namespace SharedResource { class Holder; }
namespace SharedResource { class Instance; }

/**
 * @brief  Stores all resource Instance objects, creates their dedicated lock
 *         objects, and ensures resource initialization and destruction is
 *         threadsafe.
 */
class SharedResource::Holder
{
private:
    Holder();

public:
    /**
     * @brief  Clears all resource locks.
     *
     *  This should never run before all resources are cleared. In debug
     * builds, this will verify that each Instance pointer is null.
     */
    ~Holder();

    /**
     * @brief  Gets the program's sole SharedResource::Holder instance.
     *
     * @return  The holder managing all Instance objects.
     */
    static Holder* getHolderInstance();

    /**
     * @brief  Deletes the holder instance if it is holding no valid Instance
     *         pointers. This should be called after deleting each resource
     *         Instance.
     */
    static void clearIfEmpty();

    /**
     * @brief  Finds and gets an Instance pointer using its resource key.
     *
     * @param resourceKey  A unique key identifying a Resource subclass.
     *
     * @return             A pointer to the requested resource's Instance
     *                     object. This value will be null if the Resource has
     *                     not yet been initialized.
     */
    static Instance* getResource(const juce::Identifier& resourceKey);

    /**
     * @brief  Saves the address of a new resource Instance.
     *
     *  This should only be called when the existing resource Instance at the
     * given ID is null, or to set the pointer to null while destroying the
     * resource.
     *
     * @param resourceKey  A unique key identifying a Resource subclass.
     *
     * @param resource     A pointer to the new resource instance, or nullptr if
     *                     destroying the existing resource.
     */
    static void setResource(const juce::Identifier& resourceKey,
            Instance* resource);

    /**
     * @brief  Gets the lock used to control access to a resource Instance.
     *
     * @param resourceKey  A unique key identifying a Resource subclass.
     *
     * @return             The lock used to control access to that resource.
     */
    static const juce::ReadWriteLock& getResourceLock
    (const juce::Identifier& resourceKey);

private:
    /**
     * @brief  Gets the index where a resource and its lock are stored in the
     *         resourceList and resourceLocks arrays.
     *
     *  If necessary, this will initialize the resource container, creating a
     * new lock and assigning an index.
     *
     * @param resourceKey  A unique key identifying a Resource subclass.
     *
     * @return             The index needed to find the resource index and its
     *                     lock.
     */
    static int getResourceIndex(const juce::Identifier& resourceKey);

    /**
     * @brief  Initializes a resource container, assigning it an index,
     *         creating its lock, and setting the initial resource instance
     *         address as nullptr.
     *
     * @param resourceKey  A unique key identifying a Resource subclass.
     */
    static void initResource(const juce::Identifier& resourceKey);

    // Prevents concurrent access to the containers holding resource data.
    static juce::CriticalSection holderLock;

    // Maps each resource ID to the index where its Instance and lock
    // are stored.
    std::map<juce::Identifier, int> resourceIndices;

    // Holds all resource instances.
    juce::Array<Instance*> resourceList;

    // Holds all resource locks.
    juce::OwnedArray<juce::ReadWriteLock> resourceLocks;
};
