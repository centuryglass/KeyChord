#include "Theme_Image_ConfigFile.h"
#include "Theme_Image_JSONResource.h"
#include "Theme_Image_JSONKeys.h"
#include "Theme_Image_AssetList.h"

Theme::Image::ConfigFile::ConfigFile() { }


// Gets a configured image asset list from shared JSON file data.
Theme::Image::AssetList Theme::Image::ConfigFile::getAssetList
(const juce::Identifier& componentKey) const
{
    SharedResource::LockedPtr<const JSONResource> config
            = getReadLockedResource();
    return config->getAssetList(componentKey);
}


// Gets the path to the HomePage background image.
juce::String Theme::Image::ConfigFile::getHomeBackgroundPath() const
{
    SharedResource::LockedPtr<const JSONResource> config
            = getReadLockedResource();
    return config->getConfigValue<juce::String>(JSONKeys::homeBackground);
}


// Sets the path to the HomePage background image.
void Theme::Image::ConfigFile::setHomeBackgroundPath
(const juce::String newPath)
{
    SharedResource::LockedPtr<JSONResource> config = getWriteLockedResource();
    config->setConfigValue(JSONKeys::homeBackground, newPath);
}
