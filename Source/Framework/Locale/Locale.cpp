#include "Locale.h"
#include "JuceHeader.h"

// Default locale to use if no other is found:
static const constexpr char* defaultLocale = "en_US";

// Get the name of the system locale.
juce::String Locale::getLocaleName()
{
    std::locale l("");
    return juce::String(l.name()).initialSectionNotContaining(".");
}


// Gets a default locale to use when the system locale is undefined or
// unsupported.
juce::String Locale::getDefaultLocale()
{
    return defaultLocale;
}
