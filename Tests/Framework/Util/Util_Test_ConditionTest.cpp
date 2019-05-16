#include "Util_ConditionChecker.h"

#include "Testing_DelayUtils.h"
#include "JuceHeader.h"

namespace Util
{
    namespace Test
    {
        class ConditionTest;
    }
}


// Extra milliseconds to wait for an event that should occur at a specific time:
static const constexpr int bufferPeriod = 500;

// Interval used in in checks that should never run more than once:
static const constexpr int neverRunInterval = 9999999;

// Default condition check interval:
static const constexpr int defaultInterval = 100;

// Default amount of time used for delayed condition tests:
static const constexpr int defaultDelay = 1000;

// An interval multiplier that shouldn't be used:
static const constexpr float invalidMult = -0.1;
// An interval multiplier that is valid, but unreasonably low:
static const constexpr float tooLowMult = 0.00001;
// An interval multiplier that is valid, but unreasonably high:
static const constexpr float tooHighMult = 500;
// A reasonable multiplier for checks that are increasingly frequent:
static const constexpr float lowMult = 0.95;
// A reasonable multiplier for checks that are increasingly delayed:
static const constexpr float highMult = 1.25;

typedef std::function<bool()> TestFunction;
typedef std::function<void()> VoidFunction;

// Test conditions to reuse:
TestFunction alwaysPass = [](){ return true; };
TestFunction alwaysFail = [](){ return false; };

// Test callbacks to reuse:
VoidFunction noAction = [](){};

// Stores the result of a condition test:
enum class Result
{
    pass, fail, none
};

/**
 * @brief  Tests the ConditionChecker class.
 */
class Util::Test::ConditionTest : public juce::UnitTest
{
private:
    // Result of the last test:
    Result lastResult;

    // Reusable callbacks for setting the last result:
    VoidFunction markAsPassed = [this]()
    {
        lastResult = Result::pass;
    };

    VoidFunction markAsFailed = [this]()
    {
        lastResult = Result::fail;
    };

    /**
     * @brief  Creates an error message describing a specific failed condition
     *         check result.
     *
     * @param actual    The result of the condition check.
     *
     * @param expected  The expected result of the condition check.
     *
     * @return          A string describing the error.
     */
    juce::String errorMessage(const Result expected, const Result actual);

    /**
     * @brief  Tests if the last condition check resulted in the expected
     *         outcome, using an error message from the errorMessage function
     *         if the result was unexpected.
     *
     * @param expected  The expected result of the last condition check.
     */
    void testResult(const Result expected);

    /**
     * @brief  Creates a single use test function that will log every time it
     *         runs its test.
     *
     * @param test  The condition test function to track.
     *
     * @return      A tracked version of the test function. The returned
     *              function is valid for a single test or until trackedTest is
     *              called again, whichever comes first.
     */
    TestFunction trackedTest(const TestFunction test);

    /**
     * @brief  Gets a callback function that should never run. If it does run,
     *         it fails a test and prints an error message.
     *
     * @param error  The error message to print if the test fails.
     *
     * @return       The customized callback function.
     */
    VoidFunction badCallback(const juce::String error);
    /**
     * @brief  Ensures the object is not running a check, sets its test
     *         frequency and multiplier to either new or default values, and
     *         resets the last saved test result to Result::none.
     *
     * This will test that the object is not currently running a check.
     *
     * @param toReset         The ConditionChecker to reset.
     *
     * @param testFrequency   Optional frequency in milliseconds of condition
     *                        check actions.
     *
     * @param testMultiplier  Optional multiplier to apply to the test frequency
     *                        each time the ConditionChecker waits to check its
     *                        condition again.
     */
    void resetConditionChecker(ConditionChecker& toReset,
            const int testFrequency = defaultInterval,
            const float testMultiplier = 1);

    /**
     * @brief  Starts a condition check, testing if the check successfully
     *         starts.
     *
     * @param toTest     The ConditionChecker to use for the check.
     *
     * @param condition  The condition that toTest will be checking for.
     *
     * @param onPassing  An optional callback function to run if the condition
     *                   is met.
     *
     * @param timeout    An optional duration in milliseconds, after which the
     *                   test will time out.
     *
     * @param onTimeout  An optional callback function to run if toTest times
     *                   out.
     */
    void startTestedCheck(ConditionChecker& toTest, TestFunction condition,
            const VoidFunction onPassing = noAction,
            int timeout = -1,
            const VoidFunction onTimeout = VoidFunction());

    /**
     * @brief  Tests that a ConditionChecker correctly runs a check that should
     *         immediately succeed.
     *
     * @param toTest  The checker to use to run the test.
     */
    void immediateSuccessTest(ConditionChecker& toTest);

    /**
     * @brief  Runs a delayed condition test that will succeed after a specific
     *         duration.
     *
     * @param toTest     The checker to use to run the test.
     *
     * @param delayMS    Milliseconds to wait before the condition is met.
     *
     * @param timeout    Milliseconds to wait before timeout, or -1 to disable
     *                   the timeout period.
     *
     * @param onSuccess  Callback function to run if the test passes before
     *                   timeout.
     *
     * @param onTimeout  Callback function to run if the test times out before
     *                   passing.
     */
    void delayTest(ConditionChecker& toTest, const int delayMS,
            const int timeout, VoidFunction onSuccess, VoidFunction onTimeout);

    /**
     * @brief  Tests that a Condition checker correctly runs a check that should
     *         succeed after a specific duration.
     *
     * @param toTest   The checker to use to run the test.
     *
     * @param delayMS  Milliseconds to wait before the condition is met.
     */
    void delayedSuccessTest(ConditionChecker& toTest, const int delayMS);

public:
    ConditionTest() : juce::UnitTest("ConditionChecker Testing", "Util") {}

    void runTest() override
    {
        ConditionChecker checker;

        beginTest("Condition check cancelling");
        resetConditionChecker(checker, neverRunInterval);
        startTestedCheck(checker, alwaysFail,
                badCallback(errorMessage(Result::none, Result::pass)), -1,
                badCallback(errorMessage(Result::none, Result::fail)));
        expect(checker.isChecking(),
                "Condition check should still be running");
        expect(!checker.startCheck(alwaysFail,
                badCallback(errorMessage(Result::none, Result::pass)), -1,
                badCallback(errorMessage(Result::none, Result::fail))));
        expect(checker.isChecking(), "Condition check should still be running");
        checker.cancelCheck(false, true);
        expect(!checker.isChecking(), "Condition check should have cancelled.");
        resetConditionChecker(checker);

        beginTest("Immediate condition success");
        resetConditionChecker(checker, neverRunInterval);
        immediateSuccessTest(checker);
        resetConditionChecker(checker, 1);
        immediateSuccessTest(checker);
        resetConditionChecker(checker);

        beginTest("Delayed condition success");
        resetConditionChecker(checker);
        delayedSuccessTest(checker, defaultDelay);
        logMessage("Testing invalid multiplier:");
        resetConditionChecker(checker, defaultInterval, invalidMult);
        delayedSuccessTest(checker, defaultDelay);
        logMessage("Testing overly low multiplier:");
        resetConditionChecker(checker, defaultInterval, tooLowMult);
        delayedSuccessTest(checker, defaultDelay);
        logMessage("Testing normal low multiplier:");
        resetConditionChecker(checker, defaultInterval, lowMult);
        delayedSuccessTest(checker, defaultDelay);
        logMessage("Testing normal high multiplier:");
        resetConditionChecker(checker, defaultInterval, highMult);
        delayedSuccessTest(checker, defaultDelay);
        resetConditionChecker(checker);

        beginTest("Condition timeout");
        resetConditionChecker(checker);
        startTestedCheck(checker, alwaysFail, markAsPassed, defaultDelay,
                markAsFailed);
        juce::MessageManager::getInstance()->runDispatchLoopUntil(defaultDelay
                + bufferPeriod);
        expect(!checker.isChecking(), "Checker should have timed out.");
        testResult(Result::fail);
        resetConditionChecker(checker);
        logMessage("Testing overly high multiplier:");
        resetConditionChecker(checker, defaultInterval, tooHighMult);
        delayTest(checker, defaultDelay, defaultDelay - bufferPeriod,
                markAsPassed, markAsFailed);
        testResult(Result::fail);

        beginTest("Single callback test");
        int passCounter = 0;
        int failCounter = 0;
        VoidFunction addPass = [&passCounter]()
        {
            passCounter++;
        };
        VoidFunction addFail = [&failCounter]()
        {
            failCounter++;
        };
        resetConditionChecker(checker);
        delayTest(checker, defaultDelay, -1, addPass, addFail);
        expectEquals(passCounter, 1,
                "Test should have incremented the pass counter exactly once.");
        expectEquals(failCounter, 0,
                "Test should not have changed the fail counter.");
        resetConditionChecker(checker);
        passCounter = 0;
        failCounter = 0;
        delayTest(checker, defaultDelay, defaultDelay / 2, addPass, addFail);
        expectEquals(passCounter, 0,
                "Test should not have changed the pass counter.");
        expectEquals(failCounter, 1,
                "Test should have incremented the fail counter exactly once.");
    }
};

static Util::Test::ConditionTest test;

// Creates an error message describing a specific failed condition check result.
juce::String Util::Test::ConditionTest::errorMessage
(const Result expected, const Result actual)
{
    if (expected == actual)
    {
        return "Using an error message, but the expected result occured.";
    }
    jassert(expected != actual);
    juce::String error = "Condition check ";
    switch(actual)
    {
        case Result::pass:
            error += "passed, ";
            break;
        case Result::fail:
            error += "failed, ";
            break;
        case Result::none:
            error += "never finished, ";
    }
    error += "but it should have ";
    switch(expected)
    {
        case Result::pass:
            error += "passed.";
            break;
        case Result::fail:
            error += "failed.";
            break;
        case Result::none:
            error += "never finished.";
    }
    return error;
}


// Tests if the last condition check resulted in the expected outcome, using an
// error message from the errorMessage function if the result was unexpected.
void Util::Test::ConditionTest::testResult(const Result expected)
{
    expect(lastResult == expected, errorMessage(expected, lastResult));
}


// Tracks ongoing condition check data
struct TrackingData
{
    int count = 0;
    juce::int64 startTime = 0;
    juce::int64 runTime = 0;
    juce::int64 lastPrint = -100;
};

// Creates a single use test function that will log every time it runs its test.
TestFunction Util::Test::ConditionTest::trackedTest(const TestFunction test)
{
    using juce::String;
    TrackingData* data = new TrackingData;
    // TODO: is there a reasonable RAII approach to this?
    data->startTime = juce::Time::currentTimeMillis();
    TestFunction trackedTest = [this, data, test]()
    {
        data->count++;
        juce::int64 timePassed = juce::Time::currentTimeMillis()
                - data->startTime - data->runTime;
        data->runTime += timePassed;
        if (data->runTime > (data->lastPrint + 100))
        {
            data->lastPrint = data->runTime;
            String testMessage = "Check ";
            testMessage += String(data->count);
            testMessage += ": waited ";
            testMessage += String(data->runTime);
            testMessage += " ms, ";
            testMessage += String(timePassed);
            testMessage += " since last test.";
            logMessage(testMessage);
        }
        if (test())
        {
            logMessage("Checked condition has been met");
            delete data;
            return true;
        }
        return false;
    };
    return trackedTest;
}


// Gets a callback function that should never run. If it does run, it fails a
// test and prints an error message.
VoidFunction Util::Test::ConditionTest::badCallback(const juce::String error)
{
    VoidFunction failure = [this, error]()
    {
        expect(false, error);
    };
    return failure;
}


// Resets a ConditionChecker object.
void Util::Test::ConditionTest::resetConditionChecker
(ConditionChecker& toReset, const int testFrequency, const float testMultiplier)
{
    toReset.cancelCheck();
    expect(!toReset.isChecking(), "Failed to cancel condition checking.");
    lastResult = Result::none;
    toReset.setCheckInterval(testFrequency, testMultiplier);
}


// Starts a condition check, testing if the check successfully starts.
void Util::Test::ConditionTest::startTestedCheck(
        ConditionChecker& toTest,
        TestFunction condition,
        const VoidFunction onPassing,
        int timeout,
        const VoidFunction onTimeout)
{
    expect(toTest.startCheck(condition, onPassing, timeout, onTimeout),
            "Failed to start condition checking.");
}


// Tests that a ConditionChecker correctly runs a check that should immediately
// succeed.
void Util::Test::ConditionTest::immediateSuccessTest(ConditionChecker& toTest)
{
    bool passed = false;
    juce::String failureError = errorMessage(Result::pass, Result::fail);
    startTestedCheck(toTest, alwaysPass, [&passed]() { passed = true; }, 1,
            badCallback(errorMessage(Result::pass, Result::fail)));
    expect(!toTest.isChecking(),
            "Condition checking should have immediately finished.");
    expect(passed, "Condition check failed to run passing callback.");
}


// Runs a delayed condition test that will succeed after a specific duration.
void Util::Test::ConditionTest::delayTest(ConditionChecker& toTest,
        const int delayMS,
        const int timeout,
        VoidFunction onSuccess,
        VoidFunction onTimeout)
{
    using juce::int64;
    int64 passTime = juce::Time::currentTimeMillis() + delayMS;
    TestFunction delayedPass = [passTime]()
    {
        return juce::Time::currentTimeMillis() > passTime;
    };
    delayedPass = trackedTest(delayedPass);
    lastResult = Result::none;
    startTestedCheck(toTest, delayedPass, onSuccess, timeout,
            onTimeout);
    juce::MessageManager::getInstance()->runDispatchLoopUntil(
            delayMS + bufferPeriod);
    expect(!toTest.isChecking(), "Condition check failed to finish");
}


// Tests that a Condition checker correctly runs a check that should succeed
// after a specific duration.
void Util::Test::ConditionTest::delayedSuccessTest
(ConditionChecker& toTest, const int delayMS)
{
    delayTest(toTest, delayMS, -1, markAsPassed, markAsFailed);
    testResult(Result::pass);
}
