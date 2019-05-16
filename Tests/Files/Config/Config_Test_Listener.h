#pragma once
/**
 * @file  Config_Test_Listener.h
 *
 * @brief  A listener that tracks updates to the configTest.json file.
 */

#include "Config_Listener.h"

namespace Config
{
    namespace Test
    {
        class Resource;
        class Listener;
    }
}

/**
 * @brief  Receives updates when relevant values change within the test
 *         configuration file.
 */
class Config::Test::Listener : public Config::Listener<Resource>
{
public:
    Listener();
    virtual ~Listener() { }

    /**
     * @brief  Gets the last updated key this listener received.
     *
     * @return  The string value of the last updated key value the listener
     *          received.
     */
    juce::String getLastUpdated() const;

private:
    /**
     * @brief  Saves the updated key when the configuration file is updated.
     *
     * @param propertyKey  The key to the updated property value.
     */
    void configValueChanged(const juce::Identifier& propertyKey) override;

    const juce::Identifier* lastUpdated = nullptr;
};
