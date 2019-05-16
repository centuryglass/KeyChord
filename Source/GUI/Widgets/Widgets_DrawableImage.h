#pragma once
/**
 * @file  Widgets_DrawableImage.h
 *
 * @brief  Draws images that are automatically scaled when the window is
 *         resized.
 */

#include "JuceHeader.h"

namespace Widgets { class DrawableImage; }

/**
 * @brief  A juce::Component that draws a scaled image.
 *
 *  Unlike juce::DrawableImage objects, this component's image will resize
 * itself whenever the component's bounds change.
 */
class Widgets::DrawableImage: public juce::Component
{
public:
    /**
     * @brief  ColourId values corresponding to the colours set in
     *         loadDefaultColours.
     *
     *  By using setColour(imageColourNId, newColour), any part of the image
     * with the same colour as defaultColours[N] will have its colour changed to
     * newColour.
     *
     *  When changing colours, potential colour conflicts are checked for and
     * averted, so you can safely do things like set imageColour0Id to
     * defaultColours[3], and then set imageColour3Id to some new colour
     * without also changing the areas set by imageColour0Id.
     */
    enum ColourIds {
        imageColour0Id = 0x1900000,
        imageColour1Id = 0x1900001,
        imageColour2Id = 0x1900002,
        imageColour3Id = 0x1900003,
        imageColour4Id = 0x1900004
    };

    /**
     * @brief  Creates a DrawableImage using an image file path.
     *
     * @param assetFilename  The filename of an image in the asset folder, or a
     *                       full image path. If not a valid image file, the
     *                       constructed object will have no initial image.
     *
     * @param placement      The way the image will be scaled to fit the
     *                       component.
     */
    DrawableImage(const juce::String assetFilename,
            const juce::RectanglePlacement placement
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImage using an image file.
     *
     * @param imageFile  The image file to use. If not a valid image file, the
     *                   constructed object will have no initial image.
     *
     * @param placement  The way the image will be scaled to fit the component.
     */
    DrawableImage(const juce::File imageFile,
            const juce::RectanglePlacement placement
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImage using an image object.
     *
     * @param image      Any image object. If invalid, the constructed object
     *                   will have no initial image.
     *
     * @param placement  The way the image will be scaled to fit the component.
     */
    DrawableImage(const juce::Image image,
            const juce::RectanglePlacement placement
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImage using a Drawable object.
     *
     * @param drawable  A dynamically allocated Drawable object that this
     *                  Component will claim. If null, the constructed object
     *                  will have no initial image.
     *
     * @param placement  The way the image will be scaled to fit the component.
     */
    DrawableImage(juce::Drawable* drawable,
            const juce::RectanglePlacement placement
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImage object without an initial image.
     *
     * @param placement  The way images will be scaled to fit the component.
     */
    DrawableImage(const juce::RectanglePlacement placement
            = juce::RectanglePlacement::centred);

    virtual ~DrawableImage() { }

    /**
     * @brief  Changes the image drawn by this component.
     *
     * @param assetFilename  The filename of an image in the asset folder, or a
     *                       full image path. If invalid, the DrawableImage will
     *                       have no image.
     */
    void setImage(const juce::String assetFilename);

    /**
     * @brief  Changes the image drawn by this component.
     *
     * @param imageFile  Any image file. If invalid, the DrawableImage will
     *                   have no image.
     */
    void setImage(const juce::File imageFile);

    /**
     * @brief  Changes the image drawn by this component.
     *
     * @param image  Any image object. If invalid, the DrawableImage will have
     *               no image.
     */
    void setImage(const juce::Image image);

    /**
     * @brief  Changes the image drawn by this component.
     *
     * @param drawable  A dynamically allocated Drawable object that this
     *                  Component will claim. If null, the DrawableImage will
     *                  have no image.
     */
    void setImage(juce::Drawable* drawable);

    /**
     * @brief  Checks if an image is set for this component.
     *
     * @return  Whether the component's image is valid.
     */
    bool hasImage();

    /**
     * @brief  Checks if an image is not set for this component.
     *
     * @return  Whether the component's image is null or has an area of zero
     *          pixels.
     */
    bool isEmpty();

    /**
     * @brief  Reloads the image and applies the new colour values.
     */
    virtual void colourChanged() override;

protected:
    /**
     * @brief  Adjusts image size and placement whenever component bounds
     *         change.
     */
    void resized() override;

private:
    /**
     * @brief  Sets the initial image colours and scale.
     */
    void initImage();

    // The image source file, if one is provided:
    juce::File imageSource;

    // The internal image component:
    std::unique_ptr<juce::Drawable> imageDrawable;

    // The image placement setting:
    juce::RectanglePlacement placement;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableImage)
};
