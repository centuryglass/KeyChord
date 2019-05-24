#pragma once
/**
 * @file  Layout_Component_JSONKeys.h
 *
 * @brief  Provides the keys for all parameters in the layout.json
 *         configuration file.
 */

#include "Config_DataKey.h"

namespace Layout { namespace Component { namespace JSONKeys {

//######################### Text Size Keys #################################

static const Config::DataKey smallText
    ("small text", Config::DataKey::doubleType);
static const Config::DataKey mediumText
    ("medium text", Config::DataKey::doubleType);
static const Config::DataKey largeText
    ("large text", Config::DataKey::doubleType);

static const std::vector<Config::DataKey>& basic =
{
    smallText,
    mediumText,
    largeText
};
}}}
