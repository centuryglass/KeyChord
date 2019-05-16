#pragma once
/**
 * @file  Windows_FocusListener.h
 *
 * @brief  Checks if the main application window is focused, and receives
 *         notifications whenever it gains or loses focus.
 */

#include "Windows_FocusInterface.h"
#include "SharedResource_Handler.h"
#include "JuceHeader.h"

namespace Windows
{
    class FocusListener;
    class FocusTracker;
}

/**
 * @brief  Connects to the FocusTracker resource to check if the MainWindow is
 *         focused, and to receive updates when the window gains or loses focus.
 *
 *  All background tasks should either inherit FocusListener or own a
 * FocusListener object so they can ensure that they aren't wasting resources
 * when the application isn't being actively used.
 */
class Windows::FocusListener : public SharedResource::Handler<FocusTracker>,
        public FocusInterface
{
public:
    /**
     * @brief  Connects to the FocusTracker on construction, creating the
     *         tracker instance if necessary.
     */
    FocusListener();

    virtual ~FocusListener() { }

    /**
     * @brief  Checks if the main application window is currently focused.
     *
     * @return   Whether the MainWindow exists and is currently focused.
     */
    bool getFocusState() const;

private:
    /**
     * @brief  Signals that the main application window just gained focus.
     *
     *  This takes no action by default. FocusListener subclasses should
     * override this function if they need to act when the window gains focus.
     */
    virtual void windowFocusGained() override;

    /**
     * @brief  Signals that the main application window just lost focus.
     *
     *  This takes no action by default. FocusListener subclasses should
     * override this function if they need to act when the window loses focus.
     */
    virtual void windowFocusLost() override;
};
