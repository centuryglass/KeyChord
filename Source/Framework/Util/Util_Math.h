#pragma once
/**
 * @file  Util_Math.h
 *
 * @brief  Provides miscellaneous mathematical functions.
 */

namespace Util
{
    namespace Math
    {
        /**
         * @brief  Finds the median of three values.
         *
         * @param a   The first of the values to compare.
         *
         * @param b   The second of the values to compare.
         *
         * @param c   The third of the values to compare.
         *
         * @tparam T  The compared value type.
         *
         * @return    The median of a, b, and c.
         */
        template<typename T>
        T median(const T a, const T b, const T c)
        {
            if (a > b)
            {
                if (b > c)
                {
                    return b;
                }
                if (a < c)
                {
                    return a;
                }
                return c;
            }
            if (a > c)
            {
                return a;
            }
            if (b < c)
            {
                return b;
            }
            return c;
        }
    }
}
