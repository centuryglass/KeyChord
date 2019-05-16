#pragma once
/**
 * @file  Windows_FocusInterface.h
 *
 * @brief  The interface used by Windows::FocusTracker to send updates to
 *         Windows::FocusListener objects.
 */

namespace Windows { class FocusInterface; }

/**
 * @brief  An interface for sending updates signaling that the main application
 *         window gained or lost focus.
 */
class Windows::FocusInterface
{
protected:
    FocusInterface() { }

public:
    // Only the FocusTracker resource may use this interface.
    friend class FocusTracker;

    virtual ~FocusInterface() { }

private:

    /**
     * @brief  Signals that the main application window just gained focus.
     */
    virtual void windowFocusGained() = 0;

    /**
     * @brief  Signals that the main application window just lost focus.
     */
    virtual void windowFocusLost() = 0;
};
