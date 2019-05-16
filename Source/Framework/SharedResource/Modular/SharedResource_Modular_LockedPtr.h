#pragma once
/**
 * @file  SharedResource_Modular_LockedPtr.h
 *
 * @brief  Provides controlled access to a SharedResource::Modular::Module
 *         object.
 */

#include "SharedResource_LockedInstancePtr.h"

namespace SharedResource { namespace Modular {
        template<class ResourceType, class ModuleType> class LockedPtr; } }

/**
 * @brief  Accesses a SharedResource::Modular::Module object, keeping that
 *         Module's Resource locked while the LockedPtr exists.
 *
 *  Modular::LockedPtr functions almost identically to a
 * SharedResource::LockedPtr, the only difference being that its -> operator
 * accesses a Module object held by the locked Resource object, instead of
 * directly accessing the resource itself.
 *
 * @tparam ResourceType  The SharedResource::Modular::Resource subclass
 *                       accessed by the LockedPtr object.
 *
 * @tparam ModuleType    The SharedResource::Modular::Module subclass the
 *                       LockedPtr should request from the ResourceType object.
 */
template <class ResourceType, class ModuleType>
class SharedResource::Modular::LockedPtr : public LockedInstancePtr
{
private:
    // LockedPtr objects should only be created and used internally by
    // Handler objects.
    template <class LockedResource, class LockedModule> friend class Handler;

    /**
     * @brief  Locks the ResourceType resource for as long as the LockedPtr
     *         exists.
     *
     * @param resourceKey  The unique key string identifying the ResourceType
     *                     Resource class.
     *
     * @param lockType     The type of lock used to secure the resource.
     */
    LockedPtr(const juce::Identifier& resourceKey, const LockType lockType) :
        LockedInstancePtr(resourceKey, lockType) { }

public:
    virtual ~LockedPtr() { }

    /**
     * @brief  Accesses the locked resource module's functions or data.
     *
     * @return  The address of the locked resource object instance.
     */
    ModuleType* operator->() const
    {
        return static_cast<ResourceType*>(getInstance())->
                template getModule<ModuleType>();
    }
};
