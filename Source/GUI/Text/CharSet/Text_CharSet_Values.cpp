#include "Text_CharSet_Values.h"

namespace Text { namespace CharSet { namespace Values {
    // Maps special character strings to their assigned index.
    static const std::map<juce::String, unsigned int> specialCharMap =
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
} } }

// Gets the character value stored in a string loaded from a configuration file.
unsigned int Text::CharSet::Values::getCharValue
(const juce::String charString)
{
    if (charString.length() == 0)
    {
        return 0;
    }
    if (charString.length() == 1)
    {
        return charString[0];
    }
    try
    {
        return specialCharMap.at(charString.toLowerCase());
    }
    catch (const std::out_of_range& e)
    {
        return 0;
    }
}


// Gets a character string representing a character index value.
juce::String Text::CharSet::Values::getCharString
(const unsigned int charValue)
{
   const int normalPrintMin = 0x21; 
   const int normalPrintMax = 0x7f; 
   const int extraPrintMin = 0xa1;
   const int extraPrintMax = 0xff;

   if (charValue > extraPrintMax)
   {
       return juce::String();
   }
   else if ((charValue >= normalPrintMin && charValue <= normalPrintMax)
           || (charValue >= extraPrintMin && charValue < extraPrintMax))
   {
       return juce::String::charToString((juce::juce_wchar) charValue);
   }
   else
   {
       for (auto iter = specialCharMap.begin();
               iter != specialCharMap.end(); iter++)
       {
           if (iter->second == charValue)
           {
               return iter->first;
           }
       }
   }
   return juce::String();
}


// Checks if a value needs to be drawn twice as wide as normal.
bool Text::CharSet::Values::isWideValue(const unsigned int& value)
{
    return value >= ctrl && value <= wideOutline;
}
