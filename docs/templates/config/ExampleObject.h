#pragma once
/**
 * @file  ExampleObject.h
 *
 * @brief  An example of a custom object data structure to load from a
 *         configuration file.
 */

class ExampleObject
{
public:
    /**
     * @brief  Constructs the example object, permanently setting its stored
     *         data. Custom config objects don't need to be immutable, but it
     *         simplifies things if they are.
     *
     * @param exampleNum   An arbitrary stored int value used as an example.
     *
     * @param exampleBool  An arbitrary stored boolean value used as an example.
     */
    ExampleObject(const int exampleNum, const bool exampleBool);

    virtual ~ExampleObject() { }
    
    /**
     * @brief  Gets the custom object's stored integer value.
     *
     * @return  The object's integer value.
     */
    int getExampleNum() const;

    /**
     * @brief  Gets the custom object's stored boolean value.
     *
     * @return  The object's boolean value.
     */
    bool getExampleBool() const;

private:
    const int exampleNum;
    const bool exampleBool;
};
