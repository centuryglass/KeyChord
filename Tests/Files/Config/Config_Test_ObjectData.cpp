#include "Config_Test_ObjectData.h"


// Constructs the test object, permanently setting its stored data.
Config::Test::ObjectData::ObjectData(const int testNum, const bool testBool) :
testNum(testNum), testBool(testBool) { }


// Gets the custom object's stored integer value.
int Config::Test::ObjectData::getTestNum() const
{
    return testNum;
}


// Gets the custom object's stored boolean value.
bool Config::Test::ObjectData::getTestBool() const
{
    return testBool;
}


// Compares this object with another.
bool Config::Test::ObjectData::operator== (const ObjectData& rhs) const
{
    return (testNum == rhs.testNum) && (testBool == rhs.testBool);
}
