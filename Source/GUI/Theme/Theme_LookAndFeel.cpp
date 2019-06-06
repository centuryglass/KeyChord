#include "Theme_LookAndFeel.h"
#include "Theme_Colour_JSONKeys.h"
#include "Theme_Colour_ConfigFile.h"

// TODO: Get rid of all the inline magic numbers used for calculating sizes,
//       make them into proper constexpr values defined up here.

Theme::LookAndFeel::LookAndFeel()
{
    using juce::Array;
    using juce::Identifier;
    Colour::ConfigFile colourConfig;
    const Array<int>& colourIds = Colour::JSONKeys::getColourIds();
    for (const int& id : colourIds)
    {
        setColour(id, colourConfig.getColour(id));
    }
    DBG("Theme::LookAndFeel::LookAndFeel: Loaded " << colourIds.size()
            << " colour id values.");
}
