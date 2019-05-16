#include "Widgets_DrawableImageButton.h"

// Creates a DrawableImageButton using an image file path.
Widgets::DrawableImageButton::DrawableImageButton
(const juce::File imageFile, const juce::RectanglePlacement placement) :
DrawableImageButton(placement)
{
    setImage(imageFile);
}


// Creates a DrawableImageButton using an image file.
Widgets::DrawableImageButton::DrawableImageButton
(const juce::String assetName, const juce::RectanglePlacement placement) :
DrawableImageButton(placement)
{
    setImage(assetName);
}


// Creates a DrawableImageButton using an image object.
Widgets::DrawableImageButton::DrawableImageButton
(const juce::Image imageObject, const juce::RectanglePlacement placement) :
DrawableImageButton(placement)
{
    setImage(imageObject);
}


// Creates a DrawableImageButton using a Drawable object.
Widgets::DrawableImageButton::DrawableImageButton
(juce::Drawable* drawable, const juce::RectanglePlacement placement) :
DrawableImageButton(placement)
{
    setImage(drawable);
}


// Creates the button with no initial image.
Widgets::DrawableImageButton::DrawableImageButton
(const juce::RectanglePlacement placement) :
Button("DrawableImageButton"),
imageComponent(placement)
{
    // Transfer colour values to the DrawableImage before any image is set:
    colourChanged();
    addAndMakeVisible(imageComponent);
}


// Changes the image drawn by this component.
void Widgets::DrawableImageButton::setImage(const juce::String assetFilename)
{
    imageComponent.setImage(assetFilename);
}


// Changes the image drawn by this component.
void Widgets::DrawableImageButton::setImage(const juce::File imageFile)
{
    imageComponent.setImage(imageFile);
}


// Changes the image drawn by this component.
void Widgets::DrawableImageButton::setImage(const juce::Image imageObject)
{
    imageComponent.setImage(imageObject);
}


// Changes the image drawn by this component.
void Widgets::DrawableImageButton::setImage(juce::Drawable* drawable)
{
    imageComponent.setImage(drawable);
}


// Changes the image drawn by this component.
void setImage(juce::Drawable* imageObject);

// Transfers colour changes to the internal image component.
void Widgets::DrawableImageButton::colourChanged()
{
    using juce::Colour;
    for (int colourId = DrawableImage::imageColour0Id;
        colourId <= DrawableImage::imageColour4Id;
        colourId++)
    {
        Colour imageColour = imageComponent.findColour(colourId, false);
        Colour buttonColour = findColour(colourId, false);
        if (imageColour != buttonColour)
        {
            imageComponent.setColour(colourId, buttonColour);
        }
    }
}


// Resizes the image to fit the button.
void Widgets::DrawableImageButton::resized()
{
    imageComponent.setBounds(getLocalBounds());
}


// Changes the button opacity when it is held down.
void Widgets::DrawableImageButton::paintButton
(juce::Graphics& graphics, bool isMouseOverButton, bool isButtonDown)
{
    if (isMouseOverButton && isButtonDown)
    {
        imageComponent.setAlpha(0.3f);
    }
    else
    {
        imageComponent.setAlpha(1.0f);
    }
}
