#pragma once
/**
 * @file  Theme_Image_JSONKeys.h
 *
 * @brief  Provides the keys for all configurable component image resources
 *         defined in the imageResource.json configuration file.
 */

#include "Config_DataKey.h"

namespace Theme { namespace Image { namespace JSONKeys {

// Background image used on the HomePage:
static const Config::DataKey homeBackground
    ("home page background", Config::DataKey::stringType);

static const std::vector<Config::DataKey> imageKeys
{
    homeBackground
};

//######################### UI Component Data ##############################
//Defines all component types managed in the config file
static const juce::Identifier popupMenu(
        "popup menu");
static const juce::Identifier navButton(
        "navigation buttons");
static const juce::Identifier spinner(
        "loading spinner");
static const juce::Identifier cancelButton(
        "cancel button");
static const juce::Identifier confirmButton(
        "confirm button");

static const juce::Array<juce::Identifier> components =
{
    popupMenu,
    navButton,
    spinner,
    cancelButton,
    confirmButton
};
}}}
