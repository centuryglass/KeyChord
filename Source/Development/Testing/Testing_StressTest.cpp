#include "Testing_StressTest.h"

// Sets the name and testing behavior of an action on construction.
Testing::Action::Action( const juce::String testName,
        const std::function<bool()> testAction,
        const std::function<bool()> allowAction) :
testName(testName),
testAction(testAction),
allowAction(allowAction),
testsPassed(0),
testsRan(0) { }


// Gets the name describing the test action.
const juce::String& Testing::Action::getTestName() const
{
    return testName;
}


// Checks if the test can run correctly.
bool Testing::Action::canRunAction()
{
    return allowAction();
}


// Runs the test action and saves the results, assuming canRunAction allows it.
void Testing::Action::runTestAction()
{
    jassert(allowAction);
    jassert(testAction);
    if (canRunAction())
    {
        const bool testPassed = testAction();
        jassert(testPassed);
        if (testPassed)
        {
            testsPassed.operator++();
        }
        else
        {
            DBG("Failed " << testName);
        }
        testsRan.operator++();
    }
    else
    {
        DBG("Couldn't run " << testName);
    }
}


// Gets the number of times this object's test function ran.
int Testing::Action::getActionCount() const
{
    return testsRan.get();
}


// Gets the number of times this object's test function succeeded.
int Testing::Action::getSuccessfulActionCount() const
{
    return testsPassed.get();
}


// Initialize a new StressTest.
Testing::StressTest::StressTest(
        const juce::String testName,
        const juce::String testCategory,
        const int minThreads,
        const int maxThreads,
        const int actionFreq,
        const int testDuration) :
juce::UnitTest(testName, testCategory),
minThreads(minThreads),
maxThreads(maxThreads),
actionFreq(actionFreq),
testDuration(testDuration)
{
    using juce::ScopedLock;

    Action addThread("Create new test thread", [this]()
    {
        const ScopedLock threadLock(threads.getLock());
        if (threads.size() < this->maxThreads)
        {
            threads.add(new TestThread(*this));
            threads.getLast()->startThread();
        }
        return true;
    }, [this]()
    {
        const ScopedLock threadLock(threads.getLock());
        return threads.size() < this->maxThreads;
    });

    Action removeThread("Destroy test thread", [this]()
    {
        const ScopedLock threadLock(threads.getLock());
        if (threads.size() > this->minThreads)
        {
            int threadNum = abs(getRandom().nextInt()) % threads.size();
            threads[threadNum]->signalThreadShouldExit();
        }
        return true;
    }, [this]()
    {
        const ScopedLock threadLock(threads.getLock());
        return threads.size() > this->minThreads;
    });
    addAction(addThread);
    addAction(removeThread);
}


// Adds a new action for threads to randomly perform while testing.
void Testing::StressTest::addAction(Action testAction)
{
    testActions.add(testAction);
}


// Starts the threaded stress test.
void Testing::StressTest::runThreads()
{
    using namespace juce;
    const ScopedLock threadLock(threads.getLock());
    endTime = Time::currentTimeMillis() + (100 * testDuration);
    while (threads.size() < minThreads)
    {
        threads.add(new TestThread(*this));
        threads.getLast()->startThread();
    }
    const ScopedUnlock threadUnlock(threads.getLock());
    while (!threads.isEmpty())
    {
        Thread::sleep(testDuration * 1000);
        for (int i = 0; i < threads.size(); i++)
        {
            if (threads[i] == nullptr || !threads[i]->isThreadRunning())
            {
                threads.remove(i);
                i--;
            }
        }
    }
    for (int i = 0; i < testActions.size(); i++)
    {
        String log;
        expectEquals(testActions[i].getSuccessfulActionCount(),
                testActions[i].getActionCount(),
                testActions[i].getTestName() + ": not all tests passed.");

        log << "Test \"" << testActions[i].getTestName() << "\": "
            << juce::String(testActions[i].getSuccessfulActionCount()) << "/"
            << juce::String(testActions[i].getActionCount())
            << " tests passed.";
        logMessage(log);
    }
}


// Initializes a new test thread
Testing::StressTest::TestThread::TestThread(StressTest& testSource) :
juce::Thread(testSource.getName() + " Thread"), testSource(testSource) { }


// Runs a random test from the list of test actions, then sleeps for a random
// amount of time less than the maximum test delay period.
void Testing::StressTest::TestThread::run()
{
    using namespace juce;
    while (!threadShouldExit() && Time::currentTimeMillis()
            < testSource.endTime)
    {
        juce::Array<int> validTestIndices;
        for (int i = 0; i < testSource.testActions.size(); i++)
        {
            validTestIndices.add(i);
        }
        while (!validTestIndices.isEmpty())
        {
            int metaIndex = abs(Random::getSystemRandom().nextInt())
                % validTestIndices.size();
            int index = validTestIndices[metaIndex];
            validTestIndices.remove(metaIndex);
            if (testSource.testActions[index].canRunAction())
            {
                Action& testAction
                        = testSource.testActions.getReference(index);
                testAction.runTestAction();
                break;
            }
        }
        sleep(testSource.getRandom().nextInt() % testSource.actionFreq);
    }
}
