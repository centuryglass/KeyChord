#pragma once
#include "Alphabet.h"

namespace AlphabetFactory
{
    Alphabet createLowerCase();
    Alphabet createUpperCase();
    Alphabet createNumeric();
    Alphabet createSymbolic();
}
