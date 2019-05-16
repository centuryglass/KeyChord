#pragma once
/**
 * @file  SharedResource_Instance.h
 *
 * @brief  Manages the lifespan of a resource object by tracking each reference
 *         object tied to the resource.
 */

#include "JuceHeader.h"

namespace SharedResource { class Instance; }
namespace SharedResource { class ReferenceInterface; }

/**
 * @brief  The parent class of all Resource objects, used to manage the
 *         connection between a resource and all of its associated Handler
 *         objects.
 *
 *  All handler objects implement the SharedResource::Reference class. The
 * instance class tracks all Reference objects associated with a resource,
 * destroys the resource when its last Reference is removed, and allows the
 * resource to act on all of its Reference objects.
 */
class SharedResource::Instance
{
private:
    /**
     * @brief  Creates the unique resource object instance.
     *
     * @param resourceKey  The resource's unique key identifier.
     */
    Instance(const juce::Identifier& resourceKey);

public:
    // References need to add themselves to the handlers list on
    // construction, and remove themselves on destruction.
    friend class Reference;

    // Instance may only be created as the parent class of Resource objects.
    friend class Resource;

    /**
     * @brief  Destroys the resource instance once all of its references have
     *         been destroyed.
     */
    virtual ~Instance();

    /**
     * @brief  Gets the unique key used to identify this resource Instance.
     *
     * @return  The resource's unique key identifier.
     */
    const juce::Identifier& getResourceKey() const;

    /**
     * @brief  Gets the number of Reference objects connected to this resource.
     *
     * @return  The Instance object's reference count.
     */
    int getReferenceCount() const;

private:
    /**
     * @brief  Runs an arbitrary function on each Reference object connected to
     *         the resource.
     *
     * @param handlerAction  Some action that should run for every reference
     *                       connected to this resource instance, passing in a
     *                       pointer to the Reference as a parameter.
     */
    void foreachReference
    (std::function<void(ReferenceInterface*)> referenceAction);

    // The resource's unique key identifier.
    const juce::Identifier& resourceKey;

    // Tracks all reference object associated with the resource.
    juce::Array<ReferenceInterface*> references;
};
