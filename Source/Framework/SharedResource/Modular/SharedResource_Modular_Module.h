#pragma once
/**
 * @file  SharedResource_Modular_Module.h
 *
 * @brief  An abstract basis for a classes that handle part of the
 *         responsibilities of a SharedResource::Modular::Resource object.
 */

#include "SharedResource_Modular_Handler.h"
#include <functional>

namespace SharedResource { namespace Modular {
        template<class ResourceType> class Module; } }

/**
 * @brief  Handles a portion of the responsibilities of a Modular::Resource
 *         object.
 *
 * @tparam ResourceType  The Modular::Resource subclass that owns this Module.
 */
template<class ResourceType>
class SharedResource::Modular::Module
{
private:
    // The resource that owns this module:
    ResourceType& parentResource;

protected:
    /**
     * @brief  Connects the module to its Resource.
     *
     * @param parentResource  The Modular::Resource object instance.
     */
    Module(ResourceType& parentResource) : parentResource(parentResource) { }

    /**
     * @brief  Runs a callback function on each of this module's handler
     *         objects that share a specific class.
     *
     * @tparam HandlerType   The type of handler that should be acted on.
     *
     * @param handlerAction  A function to run on each valid module handler of
     *                       type HandlerType.
     */
    template<class HandlerType>
    void foreachHandler(const std::function<void(HandlerType*)> notifyAction)
    {
        parentResource.template foreachHandler<HandlerType>(notifyAction);
    }

    /**
     * @brief  Gets the Resource object that owns this module.
     *
     * @return  A reference to the module's resource.
     */
    ResourceType& getResource()
    {
        return parentResource;
    }

    /**
     * @brief  Gets the Resource object that owns this module.
     *
     * @return  A constant reference to the module's resource.
     */
    const ResourceType& getConstResource() const
    {
        return parentResource;
    }

    /**
     * @brief  Gets one of the other Module types shared by this Module
     *         object's resource.
     *
     * @tparam ModuleType  The type of Module to get from the resource.
     *
     * @return             A pointer to the Resource object's instance of the
     *                     requested module type, or nullptr if the Resource
     *                     doesn't have a module of type ModuleType.
     */
    template<class ModuleType>
    ModuleType* getSiblingModule()
    {
        return parentResource.template getModule<ModuleType>();
    }

    /**
     * @brief  Gets one of the other Module types shared by this Module
     *         object's resource.
     *
     * @tparam ModuleType  The type of Module to get from the resource.
     *
     * @return             A constant pointer to the Resource object's instance
     *                     of the requested module type, or nullptr if the
     *                     Resource doesn't have a module of type ModuleType.
     */
    template<class ModuleType>
    const ModuleType* getConstSiblingModule() const
    {
        return parentResource.template getModule<const ModuleType>();
    }
};
