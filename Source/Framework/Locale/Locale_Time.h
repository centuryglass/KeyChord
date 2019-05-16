#pragma once
/**
 * @file  Locale_Time.h
 *
 * @brief  Generates localized time strings.
 */

#include "Locale_TextUser.h"

namespace Locale { class Time; }
namespace juce
{
    class Time;
    class String;
}

/**
 * @brief  Generates localized strings describing the amount of time that has
 *         passed between a given time value and the current time.
 */
class Locale::Time : public Locale::TextUser
{
public:
    /**
     * @brief  Creates an object to generate text describing a single time.
     *
     * @param time  The time value referred to by this object.
     */
    Time(const juce::Time time);

    virtual ~Time() { }

    /**
     * @brief  Gets a localized string describing the amount of time between
     *         the present and this object's stored time.
     *
     * This is not intended to be used for time values in the future.
     *
     * @return  The approximate difference between the present and this
     *          object's time, the localized equivalent of "now" if the time is
     *          not in the past, or the localized equivalent of "never" if the
     *          time is set to the Unix epoch.
     */
    juce::String approxTimePassed() const;

private:
    // Stores the time value described by this object.
    juce::Time time;
};
