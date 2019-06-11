#include "Text_Values.h"

#ifdef JUCE_DEBUG
// Print the full namespace name before all debug output:
static const constexpr char* dbgPrefix = "Text::Values::";
#endif

namespace Text { namespace Values {
    // Maps JUCE character description strings to their assigned index value.
    static const std::map<juce::String, CharValue> specialCharMap =
    {
        { "outline",   outline   },
        { "fill",      fill      },
        { "backspace", backspace },
        { "enter",     enter     },
        { "tab",       tab       },
        { "left",      left      },
        { "up",        up        },
        { "down",      down      },
        { "left",      left      },
        { "right",     right     },
        { "escape",    escape    },
        { "delete",    del       },
        { "f1",        f1        },
        { "f2",        f2        },
        { "f3",        f3        },
        { "f4",        f4        },
        { "f5",        f5        },
        { "f6",        f6        },
        { "f7",        f7        },
        { "f8",        f8        },
        { "f9",        f9        },
        { "f10",       f10       },
        { "f11",       f11       },
        { "f12",       f12       },
    };
} }

// Gets the character value stored in a string loaded from a configuration file.
unsigned int Text::Values::getCharValue
(const juce::String charString)
{
    // Immediately recognize empty strings as invalid
    if (charString.length() == 0)
    {
        return 0;
    }
    // Convert single characters
    if (charString.length() == 1)
    {
        return charString[0];
    }
    // Search for mapped special characters
    try
    {
        return specialCharMap.at(charString.toLowerCase());
    }
    catch (const std::out_of_range& e)
    {
        // Convert decimal strings
        if (charString.containsOnly("0123456789"))
        {
            return std::max(charString.getIntValue(), 0);
        }
        // Convert hex strings
        if (charString.containsOnly("xX0123456789abcdefABCDEF"))
        {
            return std::max(charString.getHexValue32(), 0);
        }
        DBG(dbgPrefix << __func__ << ": Invalid special character string "
                << charString << " (length " << charString.length() << ")");
        return 0;
    }
}


// Gets a character string representing a character index value.
juce::String Text::Values::getXString(const CharValue charValue)
{
    using juce::String;
    // Anything over 0xff isn't a valid CharValue, return the empty string:
    if (charValue > extraPrintMax)
    {
        return String();
    }
    else if ((charValue >= normalPrintMin && charValue <= normalPrintMax)
            || (charValue >= extraPrintMin && charValue < extraPrintMax))
    {
        // Hex character codes are the most reliable option for standard
        // printable characters:
        return String("0x") + String::toHexString(charValue);
    }
    // Use a map to convert special CharValues to X11 keySym strings:
    static const std::map<CharValue, String> keySymMap =
    {
        { backspace, "BackSpace" },
        { enter,     "Return"    },
        { tab,       "Tab"       },
        { up,        "Up"        },
        { down,      "Down"      },
        { left,      "Left"      },
        { right,     "Right"     },
        { f1,        "F1"        },
        { f2,        "F2"        },
        { f3,        "F3"        },
        { f4,        "F4"        },
        { f5,        "F5"        },
        { f6,        "F6"        },
        { f7,        "F7"        },
        { f8,        "F8"        },
        { f9,        "F9"        },
        { f10,       "F10"       },
        { f11,       "F11"       },
        { f12,       "F12"       },
        { escape,    "Escape"    },
        { del,       "Delete"    }
    };
    try
    {
        return keySymMap.at(charValue);
    }
    catch (const std::out_of_range& e)
    {
        // If it's not a typical special character, it's not out of bounds, and
        // it's not in the BinaryFont, something's gone wrong.
        DBG(dbgPrefix << __func__ << ": invalid CharValue "
                << String(charValue));
        return String();
    }
}


// Checks if a value needs to be drawn twice as wide as normal.
bool Text::Values::isWideValue(const unsigned int& value)
{
    return value >= ctrl && value <= wideOutline;
}


// Checks if a value represents a modifier, rather than a typical printed
// character.
bool Text::Values::isModifier(const unsigned int& value)
{
    return value >= ctrl && value <= super;
}
