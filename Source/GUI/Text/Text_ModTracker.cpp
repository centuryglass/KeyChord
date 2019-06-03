#include "Text_ModTracker.h"
#include "SharedResource_Resource.h"

static const juce::Identifier modResourceKey("ModResource");

/**
 * @brief  Private modifier resource class:
 */
class Text::ModResource : public SharedResource::Resource
{
public:
    ModResource() : SharedResource::Resource(modResourceKey) { }

    /**
     * @brief  Gets the key flag value used to store which keys are held down.
     *
     * @return  A byte where each of the lowest four bits is set to one only if
     *          the corresponding ModKey is held.
     */
    unsigned char getKeyFlags() const
    {
        return modMap;
    }

    /**
     * @brief  Sets the key flag value used to store which keys are held down.
     *
     * @param flags  All ModKey values that should be held down, combined with
     *               the binary OR('|') operator.
     */
    void setKeyFlags(unsigned char flags)
    {
        modMap = flags;
    }

private:
    // Stores modifier status in its lower four bits.
    unsigned char modMap = 0;
};


Text::ModTracker::ModTracker() :
SharedResource::Handler<ModResource>(modResourceKey) { }


// Sets whether a specific modifier is held.
void Text::ModTracker::setKey(const ModKey modKey, const bool holdKey)
{
    SharedResource::LockedPtr<ModResource> modResource
            = getWriteLockedResource();
    unsigned char modMap = modResource->getKeyFlags();
    if (holdKey)
    {
        modMap |= (unsigned char) modKey;
    }
    else
    {
        modMap &= ~(unsigned char) modKey;
    }
    modResource->setKeyFlags(modMap);
}


// Toggles the state of a specific key, holding it if it wasn't held, and
// releasing it if it was.
void Text::ModTracker::toggleKey(const ModKey modKey)
{
    SharedResource::LockedPtr<ModResource> modResource
            = getWriteLockedResource();
    unsigned char modMap = modResource->getKeyFlags();
    modMap ^= (unsigned char) modKey;
    modResource->setKeyFlags(modMap);
}


// Releases all held modifier keys.
void Text::ModTracker::clearAll()
{
    SharedResource::LockedPtr<ModResource> modResource
            = getWriteLockedResource();
    modResource->setKeyFlags(0);
}


// Checks if a specific modifier key is held down.
bool Text::ModTracker::isKeyHeld(const ModKey modKey) const
{
    SharedResource::LockedPtr<const ModResource> modResource
            = getReadLockedResource();
    return modResource->getKeyFlags() & (unsigned char) modKey;
}
