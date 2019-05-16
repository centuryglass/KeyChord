#pragma once
/**
 * @file  Config_ValueListener.h
 *
 * @brief  A listener that tracks a single value in a Config::FileResource.
 */

#include "JuceHeader.h"

namespace Config { template <class ListenerType, typename ValueType>
        class ValueListener; }

/**
 * @brief  Tracks a single value in a single JSON file, receiving updates
 *         whenever that value changes.
 *
 * @tparam ListenerType  A Config::Listener subclass that tracks this
 *                       ValueListener's JSON file.
 *
 * @tparam ValueType     The type of value tracked by the listener.
 */
template <class ListenerType, typename ValueType>
class Config::ValueListener
{
public:
    /**
     * @brief  Sets the ValueListener's tracked value on construction.
     *
     * @param valueKey  The key to the tracked value.
     */
    ValueListener(const juce::Identifier& valueKey) : listener(*this)
    {
        listener.addTrackedKey(valueKey);
    }

    ~ValueListener() { }

private:
    /**
     * @brief  Notifies the ValueListener that its tracked value has changed.
     *
     *  By default, this takes no action. Override this to handle the value
     * change event.
     *
     * @param newValue  The new value assigned to the valueKey in the JSON file.
     */
    virtual void configValueChanged(ValueType newValue) override { }

private:
    /**
     * @brief  The internal Listener used to connect to the FileResource.
     */
    class InternalListener : public ListenerType
    {
    public:
        /**
         * @brief  Links the InternalListener to its ValueListener on
         *         construction.
         *
         * @param valueListener  The valueListener holding this
         *                       InternalListener.
         */
        InternalListener(ValueListener& valueListener) :
            valueListener(valueListener) { }

    private:
        /**
         * @brief  Passes the updated value to the ValueListener whenever the
         *         FileResource sends an update signal.
         *
         * @param valueKey  The key to the single value tracked by the
         *                  ValueListener.
         */
        virtual void configValueChanged(const juce::Identifier& valueKey)
            override
        {
            valueListener.configValueChanged
                (ListenerType::template getConfigValue<ValueType>(valueKey));
        }
        ValueListener& valueListener;
    };
    InternalListener listener;
};
