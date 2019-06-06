#pragma once
/**
 * @file  Theme_LookAndFeel.h
 *
 * @brief  Sets the default appearance and behavior of all UI component objects.
 */

#include "JuceHeader.h"
#include "Theme_Colour_ConfigFile.h"

namespace Theme { class LookAndFeel; }

/**
 * @brief  Loads configurable colour values, so that Component classes can apply
 *         them by ID using the setColour function.
 *
 *  Directly interacting with this class should not be necessary, except when
 * the application calls LookAndFeel::setDefaultLookAndFeel() to set a
 * Theme::LookAndFeel object as the default.
 */
class Theme::LookAndFeel : public juce::LookAndFeel_V4
{
public:
    LookAndFeel();

    virtual ~LookAndFeel() { }
};
