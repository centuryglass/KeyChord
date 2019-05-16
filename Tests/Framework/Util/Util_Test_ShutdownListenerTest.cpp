#include "Util_ShutdownListener.h"
#include "JuceHeader.h"

namespace Util
{
    namespace Test
    {
        class ShutdownListenerTest;
        class TestListener;
    }
}
/**
 * @brief  A fake application class to trigger the shutdown notification.
 */
class HomeApplication
{
public:
    /**
     * @brief  Sends a fake shutdown broadcast message.
     */
    static void broadcastShutdown()
    {
        Util::ShutdownBroadcaster::broadcastShutdown();
    }
};


/**
 * @brief  A basic implementation of ShutdownListener for testing.
 */
class Util::Test::TestListener : public ShutdownListener
{
private:
    // Tracks the number of times that any ShutdownListener ran onShutdown()
    // since the test began:
    int& notifyCount;

    /**
     * @brief  Updates the notification count when the shutdown message is
     *         received.
     */
    virtual void onShutdown() override
    {
        notifyCount++;
    }

public:
    /**
     * @brief  Saves a reference to the shared notification count.
     *
     * @param notifyCount  The variable tracking notification count for all
     *                     TestListener objects.
     */
    TestListener(int& notifyCount) : notifyCount(notifyCount) { }
};

/**
 * @brief  Tests that application shutdown messages are communicated
 *         appropriately to all ShutdownListener objects.
 */
class Util::Test::ShutdownListenerTest : public juce::UnitTest
{
public:
    ShutdownListenerTest() : juce::UnitTest("ShutdownListener Testing",
            "Util") {}

    void runTest() override
    {
        juce::OwnedArray<TestListener> testListeners;
        int notifyCount = 0;
        beginTest("Shutdown notification testing");

        // Make sure the shutdown notification runs safely if no listeners
        // exist:
        HomeApplication::broadcastShutdown();

        int listenerCount = 5;
        for (int i = 0; i < listenerCount; i++)
        {
            testListeners.add(new TestListener(notifyCount));
        }
        HomeApplication::broadcastShutdown();
        expectEquals(notifyCount, listenerCount,
                "Notification count does not match listener count.");

        notifyCount = 0;
        listenerCount --;
        testListeners.removeLast();
        HomeApplication::broadcastShutdown();
        expectEquals(notifyCount, listenerCount,
                juce::String("After removing listener, notification count does")
                + juce::String("not match listener count."));

        notifyCount = 0;
        listenerCount += 2;
        testListeners.add(new TestListener(notifyCount));
        testListeners.add(new TestListener(notifyCount));
        HomeApplication::broadcastShutdown();
        expectEquals(notifyCount, listenerCount,
                juce::String("After adding listeners, notification count does")
                + juce::String("not match listener count."));

        notifyCount = 0;
        listenerCount = 0;
        testListeners.clear();
        HomeApplication::broadcastShutdown();
        expectEquals(notifyCount, listenerCount,
                juce::String("After clearing listeners, notification count ")
                + juce::String("does not match listener count."));

    }
};

static Util::Test::ShutdownListenerTest test;
