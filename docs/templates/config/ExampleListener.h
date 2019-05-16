#pragma once
/**
 * @file  ExampleListener.h
 *
 * @brief  A listener that tracks updates to the example.json file.
 */

#include "Config_Listener.h"

class ExampleResource;

/**
 * @brief  Receives updates when relevant values change within the example
 *         configuration file.
 */
class ExampleListener : public Config::Listener<ExampleResource>
{
public:
    ExampleListener();
    virtual ~ExampleListener() { }
};
