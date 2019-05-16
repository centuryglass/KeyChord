#include "JuceHeader.h"
/**
 * @file  Config_Test_FileTest.cpp
 *
 * @brief  Tests JSON Configuration file resources.
 */
#include "Config_Test_FileHandler.h"
#include "Config_Test_Listener.h"
#include "Config_Test_JSONKeys.h"

namespace Config { namespace Test { class FileTest; } }

class Config::Test::FileTest : public juce::UnitTest
{
public:
    FileTest() : juce::UnitTest("Config File Testing",
            "Config") {}

    void runTest() override
    {
        using juce::File;
        using juce::String;
        using juce::int64;


        const String defaultPath
                = "/usr/share/pocket-home/configuration/configTest.json";
        const String filePath
                = "~/.config/pocket-home/configTest.json";

        beginTest("Config file initialization");
        File defaultFile(defaultPath);
        expect(defaultFile.existsAsFile(),
                "Default configuration file not found.");
        expectGreaterThan(defaultFile.getSize(), int64(0),
                "Default file should not be empty.");

        File testFile(filePath);
        expect(!testFile.exists(), "Test file should not exist yet.");

        std::unique_ptr<FileHandler> handler = std::make_unique<FileHandler>();
        Listener testListener;

        expectGreaterThan(testFile.getSize(), int64(0),
                "Failed to restore and save default config values.");
        const int64 defaultSize = testFile.getSize();

        beginTest("Reading default values");

        const int defaultInt                = handler->getTestInt();
        const String defaultString          = handler->getTestString();
        const bool defaultBool              = handler->getTestBool();
        const double defaultDouble          = handler->getTestDouble();
        const juce::Array<int> defaultArray = handler->getTestArray();
        const ObjectData defaultObject      = handler->getTestObject();

        expectNotEquals(defaultInt, 0,
                "Default int value should be nonzero.");
        expect(defaultString.isNotEmpty(),
                "Default string should not be empty.");
        expect(defaultBool,
                "Default boolean should be true.");
        expectNotEquals(defaultDouble, 0.0,
                "Default double value should be nonzero.");
        expect(! defaultArray.isEmpty(),
                "Default array should not be empty.");
        expectNotEquals(defaultObject.getTestNum(), 0,
                "Default object num should be nonzero.");
        expect(! defaultObject.getTestBool(),
                "Default object bool should be false.");

        beginTest("Writing new values");
        testListener.addTrackedKey(JSONKeys::testBool);
        const int newInt       = defaultInt * 10;
        const String newString = defaultString + ",:EDITED";


        const bool newBool              = !defaultBool;
        const double newDouble          = defaultDouble * 10;
        const juce::Array<int> newArray = {1, 3, 5, 9, 11};
        const ObjectData newObject(
                defaultObject.getTestNum() * 10,
                ! defaultObject.getTestBool());

        handler->setTestInt(newInt);
        handler->setTestString(newString);
        expectEquals(testListener.getLastUpdated(), String("nullptr"),
                "TestListener should not have been updated.");
        handler->setTestBool(newBool);
        handler->setTestDouble(newDouble);
        handler->setTestArray(newArray);
        handler->setTestObject(newObject);

        expectGreaterThan(testFile.getSize(), defaultSize,
                "Changes were not written to config file.");
        expectEquals(handler->getTestInt(), newInt,
                "Failed to update test integer value.");
        expectEquals(handler->getTestString(), newString,
                "Failed to update test string value.");
        expect((handler->getTestBool() == newBool),
                "Failed to update test bool value.");
        expectEquals(handler->getTestDouble(), newDouble,
                "Failed to update test double value.");
        expect(handler->getTestArray() == newArray,
                "Failed to update test array value.");
        expect(handler->getTestObject() == newObject,
                "Failed to update test object value.");
        expectEquals(testListener.getLastUpdated(),
                ((juce::Identifier&) JSONKeys::testBool).toString(),
                "TestListener should have only registered the boolean update.");

        beginTest("Restoring default values");
        testListener.removeTrackedKey(JSONKeys::testBool);
        testListener.addTrackedKey(JSONKeys::testString);
        handler->restoreToDefault();
        expectEquals(testListener.getLastUpdated(),
                ((juce::Identifier&) JSONKeys::testString).toString(),
                "TestListener should have only registered the string update.");
        expectEquals(testFile.getSize(), defaultSize,
                "Config file should have returned to default size.");
        expectEquals(handler->getTestInt(), defaultInt,
                "Failed to restore default test integer value.");
        expectEquals(handler->getTestString(), defaultString,
                "Failed to restore default test string value.");
        expect((handler->getTestBool() == defaultBool),
                "Failed to restore default test bool value.");
        expectEquals(handler->getTestDouble(), defaultDouble,
                "Failed to restore default test double value.");
        expect(handler->getTestArray() == defaultArray,
                "Failed to restore default test array value.");
        expect(handler->getTestObject() == defaultObject,
                "Failed to restore default test object value.");

        testFile.deleteFile();
    }
};

static Config::Test::FileTest test;
