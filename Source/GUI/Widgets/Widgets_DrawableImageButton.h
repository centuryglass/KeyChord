#pragma once
/**
 * @file  Widgets_DrawableImageButton.h
 *
 * @brief  Provides a button component drawn using a Widgets::DrawableImage.
 */

#include "Widgets_DrawableImage.h"

namespace Widgets { class DrawableImageButton; }


/**
 * @brief  A button component drawn using a Widgets::DrawableImage.
 */
class Widgets::DrawableImageButton : public juce::Button {
public:

    /**
     * @brief  Creates a DrawableImageButton using an image file path.
     *
     * @param assetFilename  The filename of an image in the asset folder, or a
     *                       full image path. If not a valid image file, the
     *                       constructed object will have no initial image.
     *
     * @param placement      The way the image will be scaled to fit the
     *                       component.
     */
    DrawableImageButton(const juce::String assetFileName,
            const juce::RectanglePlacement placement
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImageButton using an image file.
     *
     * @param imageFile  The image file to use. If not a valid image file, the
     *                   constructed object will have no initial image.
     *
     * @param placement  The way the image will be scaled to fit the component.
     */
    DrawableImageButton(const juce::File imageFile,
            const juce::RectanglePlacement placement
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImageButton using an image object.
     *
     * @param image      Any image object. If invalid, the constructed object
     *                   will have no initial image.
     *
     * @param placement  The way the image will be scaled to fit the component.
     */
    DrawableImageButton(const juce::Image imageObject,
            const juce::RectanglePlacement placement
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImageButton using a Drawable object.
     *
     * @param drawable  A dynamically allocated Drawable object that this
     *                  Component will claim. If null, the constructed object
     *                  will have no initial image.
     *
     * @param placement  The way the image will be scaled to fit the component.
     */
    DrawableImageButton(juce::Drawable* drawable,
            const juce::RectanglePlacement placement
                = juce::RectanglePlacement::centred);

    /**
     * @brief  Creates a DrawableImageButton object without an initial image.
     *
     * @param placement  The way images will be scaled to fit the component.
     */
    DrawableImageButton(const juce::RectanglePlacement placement
            = juce::RectanglePlacement::centred);

    virtual ~DrawableImageButton() { }

    /**
     * @brief  Changes the image drawn by this component.
     *
     * @param assetFilename  The filename of an image in the asset folder, or a
     *                       full image path. If invalid, the
     *                       DrawableImageButton will have no image.
     */
    void setImage(const juce::String assetFilename);

    /**
     * @brief  Changes the image drawn by this component.
     *
     * @param imageFile  Any image file. If invalid, the DrawableImageButton
     *                   will have no image.
     */
    void setImage(const juce::File imageFile);

    /**
     * @brief  Changes the image drawn by this component.
     *
     * @param image  Any image object. If invalid, the DrawableImageButton will
     *               have no image.
     */
    void setImage(const juce::Image imageObject);

    /**
     * @brief  Changes the image drawn by this component.
     *
     * @param drawable  A dynamically allocated Drawable object that this
     *                  Component will claim. If null, the DrawableImageButton
     *                  will have no image.
     */
    void setImage(juce::Drawable* imageObject);

protected:
    /**
     * @brief  Adjusts the image size and placement whenever component size
     *         changes.
     */
    void resized() override;

private:
    /**
     * @brief  Transfers colour changes to the internal image component.
     */
    virtual void colourChanged() override;

    /**
     * @brief  Changes the button opacity when it is held down.
     *
     * @param graphics           The JUCE graphics context used to draw the
     *                           button.
     *
     * @param isMouseOverButton  Whether the mouse cursor is currently over the
     *                           button.
     *
     * @param isButtonDown       Whether the button is currently being clicked.
     */
    virtual void paintButton(juce::Graphics& graphics, bool isMouseOverButton,
            bool isButtonDown) override;

    // The internal button image component:
    DrawableImage imageComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrawableImageButton)
};
