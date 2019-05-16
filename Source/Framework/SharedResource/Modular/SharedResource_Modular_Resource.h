#pragma once
/**
 * @file  SharedResource_Modular_Resource.h
 *
 * @brief  A SharedResource::Resource class that shares its responsibilities
 *         between multiple Module subclasses.
 */

#include "SharedResource_Resource.h"

namespace SharedResource { namespace Modular {
        template<class ResourceType> class Resource; } }
namespace SharedResource { namespace Modular {
        template<class ResourceType> class Module; } }

/**
 * @brief  A basis for shared resource classes that divide their
 *         responsibilities between several Modular::Module classes.
 *
 *  Each Modular::Resource holds a single object instance of each Module class
 * associated with it. These modules are accessed as if they each were separate
 * Resource types, but all of them are initialized and destroyed together, and
 * all of them share a single lock. This approach allows Resources that perform
 * a large number of interconnected tasks to be divided up without the risk of
 * deadlocks involved when multiple resources interact.
 *
 * @tparam ResourceType  The Modular::Resource's parent class. This will be
 *                       SharedResource::Resource by default. Only classes that
 *                       inherit from SharedResource::Resource are valid
 *                       options.
 *
 */
template <class ResourceType = SharedResource::Resource>
class SharedResource::Modular::Resource : public ResourceType
{
protected:
    /**
     * @brief  Creates the single instance of a Resource subclass.
     *
     * @param resourceKey  The Resource subclass instance's unique key.
     */
    Resource(const juce::Identifier& resourceKey) :
            ResourceType(resourceKey) { }

public:
    /**
     * @brief  Gets a pointer to one of this Resource object's modules.
     *
     *  Resource class implementations must provide a specialization of this
     * function for each module type that they contain.
     *
     * @tparam ModuleType  The class of the requested module instance.
     *
     * @return             A pointer to the single Module of that type held by
     *                     the Resource.
     */
    template<class ModuleType> ModuleType* getModule()
    {
        DBG("SharedResource::Modular::Resource::" << __func__
                << ": Module class is invalid or missing getModule() "
                << "specialization.");
        jassertfalse;
        return nullptr;
    }

private:
    // Allows modules to act on their Handler objects:
    template<class ModularResourceType> friend class Module;

    /**
     * @brief  Performs an action on all of a module's Handlers that share a
     *         specific type.
     *
     * @tparam HandlerType   The Handler subclass that will be notified.
     *
     * @param notifyAction   The action to perform on each compatible handler.
     */
    template<class HandlerType>
    void foreachHandler(const std::function<void(HandlerType*)> notifyAction)
    {
        SharedResource::Resource::foreachHandler<HandlerType>(notifyAction);
    }
};
