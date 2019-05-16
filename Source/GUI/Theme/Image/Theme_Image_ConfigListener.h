#pragma once
/**
 * @file  Theme_Image_ConfigListener.h
 *
 * @brief  Receives updates when basic image path values set through the
 *         Image::JSONResource change.
 */

#include "Config_Listener.h"

namespace Theme
{
    namespace Image
    {
        class ConfigListener;
        class JSONResource;
    }
}

/**
 * @brief  Listens for relevant updated image paths shared by the configuration
 *         FileResource that controls image resource paths.
 */
class Theme::Image::ConfigListener : public Config::Listener<JSONResource>
{
public:
    ConfigListener();
    virtual ~ConfigListener() { }
};
