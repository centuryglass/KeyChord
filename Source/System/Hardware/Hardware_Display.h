#pragma once
/**
 * @file  Hardware_Display.h
 *
 * @brief  Contains functions related to display management and properties.
 */

#include "JuceHeader.h"

namespace Hardware
{
    namespace Display
    {
        /**
         * @brief  Gets the current display brightness level.
         *
         * @return  Display brightness, as an integer between 1 and 10.
         */
        int getBrightness();

        /**
         * @brief  Sets the display brightness level.
         *
         * @param brightness  This should be an integer between 1 and 10.
         *                    Values outside of this range will be rounded to
         *                    the closest valid integer.
         */
        void setBrightness(const int brightness);

        /**
         * @brief  Gets the size of the main display.
         *
         * @return  The display's width and height in pixels.
         */
        juce::Rectangle<int> getSize();

        /**
         * @brief  Gets the width of the main display.
         *
         * @return  The display's width in pixels.
         */
        int getWidth();

        /**
         * @brief  Gets the height of the main display.
         *
         * @return  The display's height in pixels.
         */
        int getHeight();
    }
}
