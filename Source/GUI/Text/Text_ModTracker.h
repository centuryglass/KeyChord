#pragma once
/**
 * @file  Text_ModTracker.h
 *
 * @brief  Gets and sets which modifier keys are currently held down.
 */

#include "SharedResource_Handler.h"

namespace Text 
{ 
    class ModTracker; 
    class ModResource; 
}

/**
 * @brief  Gets and sets which modifier keys are currently held down.
 *
 *  Because modifier keys represent a global shared state, they are controlled
 * using a SharedResource. All ModTracker objects may safely get or set the
 * current modifier key status. The resource will only remain as long as at
 * least one ModTracker object exists.
 */
class Text::ModTracker : public SharedResource::Handler<ModResource>
{
public:
    // Modifier type flags:
    enum class ModKey
    {
        control = 1,
        shift   = 2,
        alt     = 4,
        command = 8
    };

    ModTracker();

    virtual ~ModTracker() { }

    /**
     * @brief  Sets whether a specific modifier is held.
     *
     * @param modKey   The modifier key to set.
     *
     * @param holdKey  True to mark the key as held down, false to mark it as
     *                 not held down.
     */
    void setKey(const ModKey modKey, const bool holdKey);

    /**
     * @brief  Toggles the state of a specific key, holding it if it wasn't
     *         held, and releasing it if it was.
     *
     * @param modKey  The modifier key to toggle.
     */
    void toggleKey(const ModKey modKey);

    /**
     * @brief  Releases all held modifier keys.
     */
    void clearAll();

    /**
     * @brief  Checks if a specific modifier key is held down.
     *
     * @param modKey  The modifier key to check.
     *
     * @return        Whether the key is currently being held.
     */
    bool isKeyHeld(const ModKey modKey) const;

};
