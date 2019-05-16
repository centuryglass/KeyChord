#include "JuceHeader.h"
/**
 * @file  TestTemplate.cpp
 *
 * @brief  Use this file as a template when creating new test classes.
 */
#include "ExampleClass.h"

namespace ModuleName { namespace Test { class ExampleClassTest; } }

class ModuleName::Test::ExampleClassTest : public juce::UnitTest
{
public:
    ExampleClassTest() : juce::UnitTest("ExampleClass Testing",
            "ModuleName") {}

    virtual void initialise() override
    {
        // Add optional initialization here
    }

    virtual void shutdown() override
    {
        // Add optional cleanup code here
    }

    void runTest() override
    {
        beginTest("First test name:");

        bool testCondition = true;
        expect(testCondition, "Test condition was not true");

        int testValue1 = 1;
        int testValue2 = 1;
        expectEquals(testValue1, testValue2,
                "Test values were not equal!");

        int testValue3 = 2;
        expectNotEquals(testValue1, testValue3,
                "Test values were equal!");


        beginTest("Second test name:");

        int low = 0;
        int high = 9;

        expectLessThan(low, high, "Low was not less than high!");
        expectLessOrEqual(low, high, "Low was not less than or equal to high!");
        expectGreaterThan(high, low, "High was not greater than low!");
        expectGreaterOrEqual(high, low,
                "High was not greater than or equal to low!");

        logMessage("Some test message to print.");

        beginTest("Third test name:");

        juce::Random rng = getRandom();
        int random = rng.nextInt();
        expectWithinAbsoluteError(random, 0, 1000,
                "Random value not between -1000 and 1000!");

        // Passes, since this won't throw an exception
        expectDoesNotThrow(rng.getSystemRandom());
        // Writes an error, since this won't throw an exception.
        expectThrows(rng.getSystemRandom());


        // Writes an error, since this won't throw an out of range exception
        expectThrowsType(rng.getSystemRandom(), std::out_of_range);
    }
};

static ModuleName::Test::ExampleClassTest test;
