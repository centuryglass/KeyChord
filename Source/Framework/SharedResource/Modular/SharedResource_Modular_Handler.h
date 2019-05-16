#pragma once
/**
 * @file  SharedResource_Modular_Handler.h
 *
 * @brief  Provides access to a single resource Module, and ensures the modular
 *         resource exists.
 */

#include "SharedResource_Handler.h"
#include "SharedResource_Modular_LockedPtr.h"

namespace SharedResource
{
    namespace Modular
    {
        template<class ResourceType> class Module;
        template<class ResourceType, class ModuleType> class Handler;
        template<class ResourceType, class ModuleType> class LockedPtr;
    }
}

/**
 * @brief  A SharedResource::Handler subclass that only accesses one Module
 *         held by a modular resource.
 *
 * @tparam ResourceType  The Modular::Resource class that holds this Handler's
 *                       module.
 *
 * @tparam ModuleType    The Module subclass the Handler accesses within the
 *                       resource.
 */
template <class ResourceType, class ModuleType>
class SharedResource::Modular::Handler :
        public SharedResource::Handler<ResourceType>
{
public:
    virtual ~Handler() { }

protected:
    using SharedResource::Handler<ResourceType>::Handler;

    /**
     * @brief  Gets a pointer to the Handler's resource Module, locking the
     *         resource for reading.
     *
     * @return  A scoped lock object that provides access to the Module.
     */
    LockedPtr<ResourceType, const ModuleType> getReadLockedResource() const
    {
        return LockedPtr<ResourceType, const ModuleType>
                (Handler::getResourceKey(), LockType::read);
    }

    /**
     * @brief  Gets a pointer to the Handler's resource Module, locking the
     *         resource for writing.
     *
     * @return  A scoped lock object that provides access to the Module.
     */
    LockedPtr<ResourceType, ModuleType> getWriteLockedResource() const
    {
        return LockedPtr<ResourceType, ModuleType>
                (Handler::getResourceKey(), LockType::write);
    }
};
