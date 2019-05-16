#pragma once
/**
 * @file  Theme_Colour_ConfigListener.h
 *
 * @brief  Receives updates when colour values set through the
 *         Colour::JSONResource change.
 */

#include "Config_Listener.h"
#include "Theme_Colour_ListenerInterface.h"

namespace Theme
{
    namespace Colour
    {
        class ConfigListener;
        class JSONResource;
    }
}

/**
 * @brief  Listens for updates to configurable JUCE ColourId values.
 */
class Theme::Colour::ConfigListener : public ListenerInterface,
    public Config::Listener<JSONResource>
{
friend class JSONResource;
protected:
    ConfigListener();

    virtual ~ConfigListener() { }

    /**
     * @brief  Sets this listener to receive updates when a specific JUCE
     *         ColourId value changes.
     *
     * @param colourId  A colour identifier for this listener to track.
     */
    void addTrackedColourId(const int colourId);

    /**
     * @brief  Stops this listener from receiving updates when a specific JUCE
     *         ColourId value changes.
     *
     * @param colourId  A colour identifier this listener will no longer track.
     */
    void removeTrackedColourId(const int colourId);

    /**
     * @brief  Checks if a specific ID is tracked by this Listener.
     *
     * @param colourId  The ColourID value to search for in the Listener's
     *                  tracked IDs.
     *
     * @return          Whether the Listener tracks the given ID.
     */
    virtual bool isTrackedId(const int colourId) const override;

    /**
     * @brief  Gets the list of ColourId values tracked by this Listener.
     *
     * @return  All tracked ColourIds.
     */
    virtual const juce::Array<int, juce::CriticalSection>& getTrackedIds()
            const override;

    /**
     * @brief  Calls configValueChanged for each tracked key, and calls
     *         colourChanged for each tracked colourId.
     */
    virtual void loadAllConfigProperties() override;

private:
    /**
     * @brief  Notifies the Listener when a colour value it tracks is updated.
     *
     *  When generic UICategory colours are updated, this function will run for
     * each ColourId in that category that doesn't have an explicit colour
     * definition.
     *
     * @param colourId    The JUCE ColourId value being updated.
     *
     * @param updatedKey  The key string of the updated config value.
     *
     * @param newColour   The updated Colour value.
     */
    virtual void colourChanged(
            const int colourId,
            const juce::Identifier& updatedKey,
            const juce::Colour newColour) override;

    // All tracked ColourId values
    juce::Array<int, juce::CriticalSection> trackedColourIds;
};
