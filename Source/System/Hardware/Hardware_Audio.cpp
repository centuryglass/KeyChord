#include "Hardware_Audio.h"
#include "Util_Commands.h"
#include "JuceHeader.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Hardware::Audio::";
#endif


// Gets the system's volume level.
int Hardware::Audio::getVolumePercent()
{
    Util::Commands systemCommands;
    juce::String volume = systemCommands.runTextCommand(
            Util::CommandTypes::Text::getVolume);
    DBG(dbgPrefix << __func__ << ": System volume=" << volume);
    return volume.getIntValue();
}


// Changes the system audio volume level.
void Hardware::Audio::setVolume(int volumePercent)
{
    juce::String volumeArg(volumePercent);
    volumeArg += "%";
    Util::Commands systemCommands;
    systemCommands.runActionCommand(Util::CommandTypes::Action::setVolume,
            volumeArg);
}
