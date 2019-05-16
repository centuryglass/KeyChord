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

//######################### UI Component Data ##############################

//Defines all component types managed in the config file
static const juce::Identifier popupMenu
        ("popup editor menu");
static const juce::Identifier pageLeft
        ("left arrow button");
static const juce::Identifier pageRight
        ("right arrow button");
static const juce::Identifier pageUp
        ("up arrow button");
static const juce::Identifier pageDown
        ("down arrow button");
static const juce::Identifier spinner
        ("loading spinner");

static const std::vector<Config::DataKey>& basic =
{
    smallText,
    mediumText,
    largeText
};

static const juce::Array<juce::Identifier> components =
{
    popupMenu,
    pageLeft,
    pageRight,
    pageUp,
    pageDown,
    spinner
};

}}}
