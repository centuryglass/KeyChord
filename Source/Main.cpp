#include "JuceHeader.h"
#include "Application.h"

// //The macro START_JUCE_APPLICATION(Application)
// //expands to this code when building for Linux:

// juce::JUCEApplicationBase* juce_CreateApplication();
// juce::JUCEApplicationBase* juce_CreateApplication() {
// return new Application();
// }


// extern "C" int main(int argc, char* argv[])
// {
// juce::JUCEApplicationBase::createInstance = &juce_CreateApplication;
// return juce::JUCEApplicationBase::main( argc, (const char**) argv );
// }

START_JUCE_APPLICATION(Application)
