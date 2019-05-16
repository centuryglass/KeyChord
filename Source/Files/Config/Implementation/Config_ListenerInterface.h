#pragma once
/**
 * @file  Config_ListenerInterface.h
 *
 * @brief  The interface used by Config::FileResource objects to send
 *         notifications to Config::Listener objects when one of their tracked
 *         values are updated.
 */

#include "JuceHeader.h"

namespace Config { class ListenerInterface; }

/**
 * @brief  The interface used to update Listeners that track any number of
 *         values in a JSON configuration file.
 */
class Config::ListenerInterface
{
public:
    // Only FileResource may send updates to Listeners.
    friend class FileResource;

    ListenerInterface() { }

    virtual ~ListenerInterface() { }

protected:
    /**
     * @brief  Checks if a particular value is tracked by this Listener.
     *
     * @param key  The key to a value stored in the Listener's file resource.
     *
     * @return     Whether the value with the given key is tracked by the
     *             Listener.
     */
    virtual bool isKeyTracked(const juce::Identifier& key) const = 0;

    /**
     * @brief  Notifies the Listener that a value it tracks has been updated.
     *
     * @param updatedValue  The key to the updated value.
     */
    virtual void configValueChanged(const juce::Identifier& updatedValue) = 0;
};

