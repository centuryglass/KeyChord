#include "Util_ShutdownListener.h"

juce::Array<Util::ShutdownListener*, juce::CriticalSection> listeners;

// Registers the listener to receive shutdown messages.
Util::ShutdownListener::ShutdownListener()
{
    const juce::ScopedLock listenerLock(listeners.getLock());
    listeners.add(this);
}


// Unregisters the listener from shutdown messages.
Util::ShutdownListener::~ShutdownListener()
{
    const juce::ScopedLock listLock(listeners.getLock());
    const juce::ScopedLock notifyLock(notifyGuard);
    listeners.removeAllInstancesOf(this);
}


// Sends the shutdown message to all shutdown listeners.
void Util::ShutdownBroadcaster::broadcastShutdown()
{
    DBG("ShutdownBroadcaster::" << __func__
            << ": Broadcasting to all ShutdownListener objects");
    const juce::ScopedLock listLock(listeners.getLock());

    // Ensure all shutdown listeners are notified exactly once, even if they
    // create new shutdown listeners within the onShutdown function.
    juce::Array<ShutdownListener*> gotNotification;
    int shutdownCalls;
    do
    {
        shutdownCalls = 0;
        for (int i = 0; i < listeners.size(); i++)
        {
            ShutdownListener* listener = listeners[i];
            if (listener != nullptr && !gotNotification.contains(listener))
            {
                const juce::ScopedLock listenerLock(listener->notifyGuard);
                const juce::ScopedUnlock listUnlock(listeners.getLock());
                listener->onShutdown();
                gotNotification.add(listener);
                shutdownCalls++;
            }
        }
    }
    while (shutdownCalls > 0);
    DBG("ShutdownBroadcaster::" << __func__ << ": Finished broadcasting.");
}
