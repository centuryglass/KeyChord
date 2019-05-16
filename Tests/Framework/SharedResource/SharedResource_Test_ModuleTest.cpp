#include "SharedResource_Test_ModuleTestClasses.h"

/**
 * @brief  Tests that SharedResource::Modular::Resource manages resource modules
 *         and resource module handlers without problems.
 */
class ModuleTest : public juce::UnitTest
{
public:
    ModuleTest() : juce::UnitTest("SharedResource::Module Testing",
            "SharedResource") {}

    void runTest()
    {
        beginTest("Resource Module initialization");

        juce::OwnedArray<TestHandlerA> aHandlers;
        juce::OwnedArray<TestHandlerB> bHandlers;
        juce::OwnedArray<TestHandlerC> cHandlers;

        aHandlers.add(new TestHandlerA);
        expectEquals<juce::String>(defaultValueA, aHandlers[0]->getValue(),
                "Error initializing TestModuleA with default values.");
        bHandlers.add(new TestHandlerB);
        expectEquals<double>(defaultValueB, bHandlers[0]->getValue(),
                "Error initializing TestModuleB with default values.");
        cHandlers.add(new TestHandlerC);
        expectEquals<int>(defaultValueC, cHandlers[0]->getValue(),
                "Error initializing TestModuleC with default values.");

        beginTest("Module Handler notification");
        const int aCount = 3;
        const int bCount = 5;
        const int cCount = 9;

        while (aHandlers.size() < aCount)
        {
            aHandlers.add(new TestHandlerA);
        }
        while (bHandlers.size() < bCount)
        {
            bHandlers.add(new TestHandlerB);
        }
        while (cHandlers.size() < cCount)
        {
            cHandlers.add(new TestHandlerC);
        }

        expectEquals(aHandlers[0]->actOnHandlers(), aCount,
                "Unexpected TestHandlerA object count.");
        expectEquals(bHandlers[0]->actOnHandlers(), bCount,
                "Unexpected TestHandlerB object count.");
        expectEquals(cHandlers[0]->actOnHandlers(), cCount,
                "Unexpected TestHandlerC object count.");

        beginTest("Module value changes");
        juce::String newAValue = aHandlers[0]->getValue() + " altered";
        aHandlers[0]->setValue(newAValue);
        expectEquals(aHandlers[1]->getValue(), newAValue,
                "Failed to set and read new TestModuleA value.");

        double newBValue = bHandlers[0]->getValue() + 0.5;
        bHandlers[0]->setValue(newBValue);
        expectEquals(bHandlers[1]->getValue(), newBValue,
                "Failed to set and read new TestModuleB value.");

        int newCValue = cHandlers[0]->getValue() + 10;
        cHandlers[0]->setValue(newCValue);
        expectEquals(cHandlers[1]->getValue(), newCValue,
                "Failed to set and read new TestModuleC value.");

        beginTest("Sibling module access");
        expectEquals(cHandlers[0]->getAValue(), aHandlers[0]->getValue(),
                "Invalid value read from TestModule A through module C.");
        expectEquals(cHandlers[0]->getBValue(), bHandlers[0]->getValue(),
                "Invalid value read from TestModule B through module C.");
        expectEquals(aCount + bCount + cCount, cHandlers[0]->actOnAllHandlers(),
                "Failed to access all test handlers via sibling modules.");

        beginTest("Module reset");
        aHandlers.clear();
        bHandlers.clear();
        cHandlers.clear();

        TestHandlerA aHandler;
        TestHandlerB bHandler;
        TestHandlerC cHandler;
        expectEquals<juce::String>(defaultValueA, aHandler.getValue(),
                "Error restoring TestModuleA with default values.");
        expectEquals<double>(defaultValueB, bHandler.getValue(),
                "Error restoring TestModuleB with default values.");
        expectEquals<int>(defaultValueC, cHandler.getValue(),
                "Error restoring TestModuleC with default values.");

        expectEquals(aHandler.actOnHandlers(), 1,
                "Unexpected TestHandlerA object count after reset.");
        expectEquals(bHandler.actOnHandlers(), 1,
                "Unexpected TestHandlerB object count after reset.");
        expectEquals(cHandler.actOnHandlers(), 1,
                "Unexpected TestHandlerC object count after reset.");
    }
};

static ModuleTest test;
