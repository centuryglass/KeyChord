#include "JuceHeader.h"
#include "HomeApplication.h"

// //The macro START_JUCE_APPLICATION(HomeApplication)
// //expands to this code when building for Linux:

// juce::JUCEApplicationBase* juce_CreateApplication();
// juce::JUCEApplicationBase* juce_CreateApplication() {
// return new HomeApplication();
// }


// extern "C" int main(int argc, char* argv[])
// {
// juce::JUCEApplicationBase::createInstance = &juce_CreateApplication;
// return juce::JUCEApplicationBase::main( argc, (const char**) argv );
// }

START_JUCE_APPLICATION(HomeApplication)
