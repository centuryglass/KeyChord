#pragma once
#include "Input_Key_Alphabet.h"

namespace Input
{
    namespace Key
    {
        namespace AlphabetFactory
        {
            Alphabet createLowerCase();

            Alphabet createUpperCase();

            Alphabet createNumeric();

            Alphabet createSymbolic();
        }
    }
}
