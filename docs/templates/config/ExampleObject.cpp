#include "ExampleObject.h"


// Constructs the example object, permanently setting its stored data.
ExampleObject::ExampleObject(const int exampleNum, const bool exampleBool) :
exampleNum(exampleNum), exampleBool(exampleBool) { }


// Gets the custom object's stored integer value.
int ExampleObject::getExampleNum() const
{
    return exampleNum;
}


// Gets the custom object's stored boolean value.
bool ExampleObject::getExampleBool() const
{
    return exampleBool;
}
