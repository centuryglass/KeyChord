#define SHARED_RESOURCE_IMPLEMENTATION
#include "SharedResource_Holder.h"
#include "SharedResource_Instance.h"
#include "SharedResource_ReferenceInterface.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "SharedResource::Instance::";
#endif

// Creates the unique resource object instance.
SharedResource::Instance::Instance
(const juce::Identifier& resourceKey) : resourceKey(resourceKey)
{
    DBG(dbgPrefix << __func__ << ": Creating resource \""
            << resourceKey.toString() << "\"");

    // The reference creating this resource can't add itself to the resource's
    // reference list until all of the resource instance's constructors finish
    // running. This means that if one of those constructors creates and then
    // destroys a Reference, it could inadvertently destroy itself. To prevent
    // this, the reference list is created with an initial null reference,
    // which the creating Reference will replace.
    references.add(nullptr);
    Holder::getHolderInstance()->setResource(resourceKey, this);
}


// Destroys the resource instance once all of its references have been
// destroyed.
SharedResource::Instance::~Instance()
{
    if (references.size() > 0)
    {
        DBG(dbgPrefix << __func__ << ": Resource " << resourceKey.toString()
                << " destroyed while " << references.size()
                << " references still exist!");
        jassertfalse;
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Destroying resource \""
                << resourceKey.toString() << "\"");
    }
}


// Gets the unique key used to identify this resource Instance.
const juce::Identifier& SharedResource::Instance::getResourceKey() const
{
    return resourceKey;
}


// Gets the number of Reference objects connected to this resource.
int SharedResource::Instance::getReferenceCount() const
{
    return references.size();
}


// Runs an arbitrary function on each Reference object connected to the
// resource.
void SharedResource::Instance::foreachReference
(std::function<void(ReferenceInterface*)> referenceAction)
{
    Holder* resourceHolder = Holder::getHolderInstance();
    const juce::ReadWriteLock& resourceLock = resourceHolder->getResourceLock
        (resourceKey);
    const juce::ScopedReadLock referenceListLock(resourceLock);

    juce::Array<ReferenceInterface*> handledReferences;
    int referencesHandled;
    do
    {
        referencesHandled = 0;
        for (int i = 0; i < references.size(); i++)
        {
            ReferenceInterface* reference = references[i];
            if (reference != nullptr && !handledReferences.contains(reference))
            {
                const juce::ScopedLock referenceLock(reference->getLock());
                resourceLock.exitRead();
                referenceAction(reference);
                resourceLock.enterRead();
                handledReferences.add(reference);
                referencesHandled++;
            }
        }
    }
    while (referencesHandled > 0);
}

