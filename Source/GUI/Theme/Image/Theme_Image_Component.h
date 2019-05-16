#pragma once
/**
 * @file  Theme_Image_Component.h
 *
 * @brief  A Component template class with image assets and image colours set
 *         in the image assets JSON file.
 */

#include "Theme_Image_AssetList.h"
#include "Theme_Image_ConfigFile.h"
#include "Widgets_DrawableImage.h"

namespace Theme { namespace Image { template <class ComponentType>
    class Component; } }

/**
 * @brief  A subclass of Widgets::DrawableImage with image assets and colours
 *         defined by an AssetList object.
 *
 * @tparam DrawableImageType  The Component's parent class. This should be
 *                            either Widgets::DrawableImageComponent or one of
 *                            its subclasses.
 */
template <class DrawableImageType = Widgets::DrawableImage>
class Theme::Image::Component : public DrawableImageType
{
public:
    /**
     * @brief  Loads the image asset list on creation.
     *
     * @param assetKey    One of the component asset keys defined in
     *                    Theme::Image::JSONKeys.
     *
     * @param assetIndex  Selects which component image asset file should be
     *                    loaded on initialization.
     *
     * @param placement   Defines how the image will be scaled.
     */
    Component(const juce::Identifier& assetKey, const int assetIndex = 0,
            const juce::RectanglePlacement placement
                    = juce::RectanglePlacement::centred) :
    DrawableImageType(placement),
    assetIndex(assetIndex)
    {
        assetList = assetConfig.getAssetList(assetKey);
        applyImageAssets();
    }


    virtual ~Component() { }

    /**
     * @brief  Loads a new image from a different asset file.
     *
     * @param index  The index of an asset file defined for this component in
     *               its AssetList. If index is out of bounds or it equals the
     *               current loaded index, nothing will happen.
     */
    void setImageAssetIndex(const int index)
    {
        if (index >= 0 && assetIndex != index)
        {
            assetIndex = index;
        }
        applyImageAssets();
    }

    /**
     * @brief  Gets the number of image assets available to the Component.
     *
     * @return  The image asset file count.
     */
    int getImageAssetCount() const
    {
        return assetList.getImageFiles().size();
    }

protected:
    /**
     * @brief  Reloads the image file and sets the image colours.
     */
    virtual void applyImageAssets()
    {
        const juce::StringArray& filenames = assetList.getImageFiles();
        if (assetIndex >= 0 && assetIndex < filenames.size())
        {
            DrawableImageType::setImage(filenames[assetIndex]);
        }
        else
        {
            DBG("Theme::Image::Component::" << __func__
                    << ": couldn't load asset index " << assetIndex
                    << ", asset count is " << filenames.size());
            assetIndex = -1;
        }

        const juce::Array<juce::Colour>& colours = assetList.getImageColours();
        for (int i = 0; i < colours.size(); i++)
        {
            int colourId = Widgets::DrawableImage::imageColour0Id + i;
            DrawableImageType::setColour(colourId, colours[i]);
        }
    }

private:
    // Current selected image asset index:
    int assetIndex = 0;

    // The list of available image assets and custom image colours:
    AssetList assetList;

    // Gets image assets and keeps the image asset list loaded:
    ConfigFile assetConfig;
};
