#define EXAMPLE_FILE_IMPLEMENTATION

#include "ExampleFile.h"
#include "ExampleResource.h"
#include "ExampleKeys.h"

ExampleFile::ExampleFile() { }


// Gets the file's example integer value.
int ExampleFile::getExampleInt() const
{
    return getConfigValue<int>(ExampleKeys::exampleInt);
}


// Gets the file's example string value.
juce::String ExampleFile::getExampleString() const
{
    return getConfigValue<juce::String>(ExampleKeys::exampleString);
}


// Gets the file's example bool value.
bool ExampleFile::getExampleBool() const
{
    return getConfigValue<bool>(ExampleKeys::exampleBool);
}


// Gets the file's example double value.
double ExampleFile::getExampleDouble() const
{
    return getConfigValue<double>(ExampleKeys::exampleDouble);
}


// Gets the file's example array value.
juce::Array<int> ExampleFile::getExampleArray() const
{
    SharedResource::LockedPtr<const ExampleResource> resource
            = getReadLockedResource();
    return resource->getExampleArray();
}


// Gets the file's example object value.
ExampleObject ExampleFile::getExampleObject() const
{
    SharedResource::LockedPtr<const ExampleResource> resource
            = getReadLockedResource();
    return resource->getExampleObject();
}


// Sets the file's example integer value.
void ExampleFile::setExampleInt(const int newInt)
{
    setConfigValue<int>(ExampleKeys::exampleInt, newInt);
}


// Sets the file's example string value.
void ExampleFile::setExampleString(const juce::String newString)
{
    setConfigValue<juce::String>(ExampleKeys::exampleString, newString);
}


// Sets the file's example bool value.
void ExampleFile::setExampleBool(const bool newBool)
{
    setConfigValue<bool>(ExampleKeys::exampleBool, newBool);
}


// Sets the file's example double value.
void ExampleFile::setExampleDouble(const double newDouble)
{
    setConfigValue<double>(ExampleKeys::exampleDouble, newDouble);
}


// Sets the file's example object value.
void ExampleFile::setExampleObject(const ExampleObject newObject)
{
    SharedResource::LockedPtr<ExampleResource> resource
            = getWriteLockedResource();
    resource->setExampleObject(newObject);
}
