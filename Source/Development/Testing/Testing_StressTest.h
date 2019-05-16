#pragma once
/**
 * @file  Testing_StressTest.h
 *
 * @brief  Tests for thread safety by performing many simultaneous actions in
 *         many threads.
 */

#include <map>
#include "JuceHeader.h"

namespace Testing
{
    class Action;
    class StressTest;
}

/**
 * @brief  Defines a single action a StressTest may choose to perform.
 */
class Testing::Action
{
public:
    /**
     * @brief  Sets the name and testing behavior of an action on construction.
     *
     * @param testName     The name used to identify this action in test logs.
     *
     * @param testAction   The test function the object will run. This
     *                     function's return value indicates whether the test
     *                     passed.
     *
     * @param allowAction  An optional function to check if the test can run
     *                     correctly. When this returns false, the testAction
     *                     will not run.
     */
    Action(const juce::String testName, const std::function<bool()> testAction,
            std::function<bool()> allowAction = []() { return true; });

    Action() { }

    virtual ~Action() { }

    /**
     * @brief  Gets the name describing the test action.
     *
     * @return  A brief description of the test this object performs.
     */
    const juce::String& getTestName() const;

    /**
     * @brief  Checks if the test can run correctly.
     *
     * @return  True if the test may run, false if circumstances prevent the
     *          test from running.
     */
    bool canRunAction();

    /**
     * @brief  Runs the test action and saves the results, assuming
     *         canRunAction allows it.
     */
    void runTestAction();

    /**
     * @brief  Gets the number of times this object's test function ran.
     *
     * @return  The test action execution count.
     */
    int getActionCount() const;

    /**
     * @brief  Gets the number of times this object's test function succeeded.
     *
     * @return  The number of successful test actions executed.
     */
    int getSuccessfulActionCount() const;

private:
    // Test description:
    const juce::String testName;
    // Runs the test, returning the result:
    std::function<bool()> testAction;
    // Returns whether the test may run:
    std::function<bool()> allowAction;
    // Tracks how many times the test ran:
    juce::Atomic<int> testsRan;
    // Tracks how many times the test passed:
    juce::Atomic<int> testsPassed;
};

/**
 * @brief  A UnitTest class that can run a stress test, attempting many
 *         simultaneous actions on many threads.
 */
class Testing::StressTest : public juce::UnitTest
{
public:
    /**
     * @brief  Initializes a new StressTest.
     *
     * @param testName      A name identifying this StressTest.
     *
     * @param testCategory  The test's category name.
     *
     * @param minThreads    Minimum number of testing threads to run at once.
     *
     * @param maxThreads    Maximum number of testing threads to run at once.
     *
     * @param actionFreq    Longest amount of time(in milliseconds) that
     *                      threads should sleep between actions.
     *
     * @param testDuration  Duration to run tests, in seconds.
     */
    StressTest(
            const juce::String testName,
            const juce::String testCategory,
            const int minThreads,
            const int maxThreads,
            const int actionFreq,
            const int testDuration);

protected:
    /**
     * @brief  Adds a new action for threads to randomly perform while testing.
     *
     * @param testAction  A function that threads may randomly select to run
     *                    while testing.
     */
    void addAction(Action testAction);

    /**
     * @brief  Starts the threaded stress test.
     *
     * This creates the minimum number of threads, and allows them to perform
     * random actions for <testDuration> seconds. This should be called once in
     * the runTest() function of StressTest unit tests.
     */
    void runThreads();

private:
    /**
     * @brief  Runs a child thread that randomly selects and performs actions
     *         for the duration of a test.
     */
    class TestThread : public juce::Thread
    {
    public:
        /**
         * @brief  Initializes a new test thread
         *
         * @param testSource  The test object creating this thread.
         */
        TestThread(StressTest& testSource);

        virtual ~TestThread() { }

    private:
        /**
         * @brief  Runs a random test from the list of test actions, then
         *         sleeps for a random amount of time less than the maximum
         *         test delay period.
         */
        virtual void run() override;

        // The UnitTest that created the thread, used to access test actions
        // and frequency.
        StressTest& testSource;
    };

    // All actions that TestThreads may select while testing:
    juce::Array<Action> testActions;
    // Minimum number of TestThreads to run at once.
    const int minThreads;
    // Maximum number of TestThreads to run at once:
    const int maxThreads;
    // Maximum amount of time for threads to sleep between actions, in
    // milliseconds.
    const int actionFreq;
    // Amount of time to allow TestThreads to act, in seconds:
    const int testDuration;
    // The time an active test should end, in milliseconds since the Unix
    // epoch.
    juce::uint64 endTime;
    // Holds all active test threads:
    juce::OwnedArray<TestThread, juce::CriticalSection> threads;
};
