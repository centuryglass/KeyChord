#pragma once
/**
 * @file  Config_Test_ObjectData.h
 *
 * @brief  An example of a custom object data structure to load from a
 *         configuration file.
 */

namespace Config { namespace Test { class ObjectData; } }

class Config::Test::ObjectData
{
public:
    /**
     * @brief  Constructs the test object, permanently setting its stored
     *         data.
     *
     * @param testNum   An arbitrary stored int value used for testing.
     *
     * @param testBool  An arbitrary stored boolean value used for testing.
     */
    ObjectData(const int testNum, const bool testBool);

    virtual ~ObjectData() { }

    /**
     * @brief  Gets the custom object's stored integer value.
     *
     * @return  The object's integer value.
     */
    int getTestNum() const;

    /**
     * @brief  Gets the custom object's stored boolean value.
     *
     * @return  The object's boolean value.
     */
    bool getTestBool() const;

    /**
     * @brief  Compares this object with another.
     *
     * @param rhs  The compared object.
     *
     * @return     Whether both objects have the same stored number and bool.
     */
    bool operator== (const ObjectData& rhs) const;

private:
    const int testNum;
    const bool testBool;
};
