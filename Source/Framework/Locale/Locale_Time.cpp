#include "Locale_Time.h"
#include "JuceHeader.h"

// Class localized text key:
static const juce::Identifier textUserKey = "Locale::Time";

// Individual string keys:
namespace TextKey
{
    static const juce::Identifier never          = "never";
    static const juce::Identifier now            = "now";
    static const juce::Identifier momentsAgo     = "momentsAgo";

    static const juce::Array<juce::Identifier> timeUnits =
    {
        "secondsPrefix",
        "secondsPostfix",

        "minutesPrefix",
        "minutesPostfix",

        "hoursPrefix",
        "hoursPostfix",

        "daysPrefix",
        "daysPostfix",

        "weeksPrefix",
        "weeksPostfix",

        "yearsPrefix",
        "yearsPostfix",
    };
}


// Longest duration in milliseconds to describe using TextKey::now:
static const constexpr int maxNowTextDuration = 500;
// Longest duration in milliseconds to describe using TextKey::momentsAgo:
static const constexpr int maxMomentTextDuration = 2000;

// Constant time conversion values:
static const juce::Array<int> unitConversions =
{
    1000, // Milliseconds per second
        60, // Seconds per minute
        60, // Minutes per hour
        24, // Hours per day
        7, // Days per week
        52, // Weeks per year
};

Locale::Time::Time(juce::Time time) : TextUser(textUserKey),
time(time) { }


// Gets a localized string describing the amount of time between the present and
// this object's stored time.
juce::String Locale::Time::approxTimePassed() const
{
    using juce::int64;
    using juce::String;
    int64 timeMS = time.toMilliseconds();
    if (timeMS == 0)
    {
        return localeText(TextKey::never);
    }
    int64 timePassed = juce::Time::currentTimeMillis() - time.toMilliseconds();
    if (timePassed < maxNowTextDuration)
    {
        return localeText(TextKey::now);
    }

    if (timePassed < maxMomentTextDuration)
    {
        return localeText(TextKey::momentsAgo);
    }

    for (int i = 0; i < unitConversions.size(); i++)
    {
        timePassed /= unitConversions[i];
        int nextUnit = i + 1;
        if (nextUnit == unitConversions.size()
                || (timePassed < (unitConversions[nextUnit] * 2)))
        {
            return localeText(TextKey::timeUnits[i * 2]) + String(timePassed)
                + localeText(TextKey::timeUnits[i * 2 + 1]);
        }
    }
    jassertfalse; // the for loop should always return before it can finish.
    return String();
}
