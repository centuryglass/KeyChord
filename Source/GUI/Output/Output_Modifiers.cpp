#include "Output_Modifiers.h"
#include <map>


// Gets the string representation of any combination of modifier types.
juce::String Output::Modifiers::getModString(const int modifierFlags)
{
    using juce::String;
    const std::map<int, String> modNames =
    {
        {(int) control, "control+" },
        {(int) alt,     "alt+"     },
        {(int) shift,   "shift+"   },
        {(int) super,   "super+"   },
    };
    juce::String modString;
    for (const auto& iter : modNames)
    {
        if ((modifierFlags & iter.first) != 0)
        {
            modString += iter.second;
        }
    }
    return modString;
}
