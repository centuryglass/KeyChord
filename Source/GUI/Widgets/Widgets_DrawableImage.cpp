#include "Widgets_DrawableImage.h"
#include "Assets.h"
#include "Theme_Colour_ConfigFile.h"
#include <map>

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Widgets::DrawableImage::";
#endif

// Default colour values to replace with custom image colours:
static juce::Array<juce::Colour> defaultColours;

// Creates a DrawableImage using an image file path.
Widgets::DrawableImage::DrawableImage
(const juce::String assetFilename, const juce::RectanglePlacement placement) :
DrawableImage(placement)
{
    setImage(assetFilename);
}


// Creates a DrawableImage using an image file.
Widgets::DrawableImage::DrawableImage
(const juce::File imageFile, const juce::RectanglePlacement placement) :
DrawableImage(placement)
{
    setImage(imageFile);
}


// Creates a DrawableImage using an image object.
Widgets::DrawableImage::DrawableImage
(const juce::Image image, const juce::RectanglePlacement placement) :
DrawableImage(placement)
{
    setImage(image);
}


// Creates DrawableImage using a Drawable object.
Widgets::DrawableImage::DrawableImage
(juce::Drawable* drawable, const juce::RectanglePlacement placement) :
DrawableImage(placement)
{
    setImage(drawable);
}


// Creates a DrawableImage without an initial image.
Widgets::DrawableImage::DrawableImage
(const juce::RectanglePlacement placement) :
placement(placement)
{
#    if JUCE_DEBUG
    setName("Widgets::DrawableImage");
#    endif
    setInterceptsMouseClicks(false, false);
    if (defaultColours.isEmpty())
    {
        Theme::Colour::ConfigFile colourConfig;
        defaultColours.add(colourConfig.getColour((int) imageColour0Id));
        defaultColours.add(colourConfig.getColour((int) imageColour1Id));
        defaultColours.add(colourConfig.getColour((int) imageColour2Id));
        defaultColours.add(colourConfig.getColour((int) imageColour3Id));
        defaultColours.add(colourConfig.getColour((int) imageColour4Id));
    }
}


// Changes the image drawn by this component.
void Widgets::DrawableImage::setImage(const juce::String assetFilename)
{
    setImage(Assets::findAssetFile(assetFilename));
}


// Changes the image drawn by this component.
void Widgets::DrawableImage::setImage(const juce::File imageFile)
{
    const juce::MessageManagerLock mmLock;
    if (imageFile.existsAsFile())
    {
        if (imageDrawable != nullptr)
        {
            removeChildComponent(imageDrawable.get());
        }
        imageDrawable.reset(juce::Drawable::createFromImageFile(imageFile));
        if (imageDrawable != nullptr)
        {
            imageSource = imageFile;
            initImage();
        }
        else
        {
            DBG(dbgPrefix << __func__ << ": Failed to load "
                    << imageFile.getFullPathName());
        }
    }
    else
    {
        imageDrawable.reset(nullptr);
    }
}


// Changes the image drawn by this component.
void Widgets::DrawableImage::setImage(const juce::Image image)
{
    juce::DrawableImage * drawable = new juce::DrawableImage();
    drawable->setImage(image);
    imageDrawable.reset(drawable);
    initImage();
}


// Changes the image drawn by this component.
void Widgets::DrawableImage::setImage(juce::Drawable* drawable)
{
    imageDrawable.reset(drawable);
    initImage();
}


// Checks if an image is set for this component.
bool Widgets::DrawableImage::hasImage()
{
    return imageDrawable != nullptr && !imageDrawable->getBounds().isEmpty();
}


// Checks if an image is set for this component.
bool Widgets::DrawableImage::isEmpty()
{
    return !hasImage();
}


// Reloads the image and applies the new colour values.
void Widgets::DrawableImage::colourChanged()
{
    if (imageSource.existsAsFile())
    {
        setImage(imageSource);
    }
}


// Adjusts image size and placement whenever component size changes.
void Widgets::DrawableImage::resized()
{
    if (imageDrawable != nullptr)
    {
        imageDrawable->setTransformToFit(getLocalBounds().toFloat(),
                placement);
    }
}


// Sets the initial image colours and scale.
void Widgets::DrawableImage::initImage()
{
    if (imageDrawable == nullptr)
    {
        return;
    }
    addAndMakeVisible(imageDrawable.get());
    imageDrawable->setTransformToFit(getLocalBounds().toFloat(), placement);

    // Load image colours that will replace the defaults:
    using juce::Colour;
    juce::Array<Colour> imageColours;
    bool replacementFound = false;
    for (int i = 0; i < defaultColours.size(); i++)
    {
        juce::Colour newColour = findColour(imageColour0Id + i);
        if (!replacementFound && newColour != defaultColours[i])
        {
            replacementFound = true;
        }
        imageColours.add(newColour);
    }
    if (!replacementFound)
    {
        return;  // No colours changed, no other actions are needed.
    }

    // Map temporary replacement colours to the values they're replacing:
    using juce::uint32;
    std::map<uint32, uint32> tempColours;
    for (int i = 0; i < defaultColours.size(); i++)
    {
        const int colourId = imageColour0Id + i;
        //  Let i and j be colour indices where j > i. If colour[i] is changing
        // from a to b, and colour[j] is changing from b to c, set colour[i] to
        // some temporary value so that in the end we get colour[i] = b,
        // colour[j] = c and not colour[i] = c,  colour[j] = c
        // (unless b equals c).

        // TLDR: Prevent conflicts if one of the new colours is also a default
        //       colour.
        const int existingIndex = defaultColours.indexOf(imageColours[i]);
        if (existingIndex < i)
        {
            //  Colour conflict doesn't exist, or involves a colour that has
            // already been changed, so direct replacement is possible.
            imageDrawable->replaceColour(defaultColours[i], imageColours[i]);
        }
        else if (existingIndex > i)
        {
            //  Colour conflict exists, replace the colour with a temporary
            //  colour that's not already in use.
            Colour tempColour = juce::Colour(0x0);
            while (defaultColours.contains(tempColour)
                    || imageColours.contains(tempColour)
                    || tempColours.count(tempColour.getARGB()) != 0)
            {
                tempColour = Colour(tempColour.getARGB() + 1);
            }
            tempColours[tempColour.getARGB()] = imageColours[i].getARGB();
            imageDrawable->replaceColour(defaultColours[i], tempColour);
        }
        // If existingIndex == i, the colour doesn't change, so no action is
        // needed.
    }
    // Temporary colours can now be safely replaced with their actual values.
    for (auto it = tempColours.begin(); it != tempColours.end(); it++)
    {
        imageDrawable->replaceColour(Colour(it->first), Colour(it->second));
    }
}
