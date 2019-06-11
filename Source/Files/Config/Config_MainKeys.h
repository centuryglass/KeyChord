#pragma once
/**
 * @file  Config_MainKeys.h
 *
 * @brief  Defines the keys used to load data from the main configuration file.
 */

#include "Config_DataKey.h"

namespace Config
{
    namespace MainKeys
    {
        // Whether the application should start in minimized mode:
        static const DataKey minimized("minimized",
                DataKey::DataType::boolType);
        // Whether the application should snap to the bottom edge of the screen
        // instead of the top:
        static const DataKey snapToBottom("snapToBottom",
                DataKey::DataType::boolType);
        // Whether the application should start in immediate text entry mode:
        static const DataKey immediateMode("immediateMode",
                DataKey::DataType::boolType);
    }
}
