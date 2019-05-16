#include "Theme_Image_AssetList.h"

namespace ThemeImages = Theme::Image;

// The key used to store the list of image asset files:
static const constexpr char* imageFileKey = "image files";

// The key used to store the list of image asset colours:
static const constexpr char* imageColourKey = "colours";

// Creates an empty asset list object.
ThemeImages::AssetList::AssetList() { }


// Initializes the image asset list from JSON data.
ThemeImages::AssetList::AssetList(juce::DynamicObject* jsonObj)
{
    using juce::var;
    using juce::Colour;
    if (jsonObj == nullptr)
    {
        DBG("Theme::Image::AssetList::AssetList: Invalid JSON object!");
        jassertfalse;
        return;
    }
    var assetList = jsonObj->getProperty(imageFileKey);
    if (assetList.isArray())
    {
        for (var asset : *assetList.getArray())
        {
            imageFiles.add(asset);
        }
    }
    var colourList = jsonObj->getProperty(imageColourKey);
    if (colourList.isArray())
    {
        for (var colour : *colourList.getArray())
        {
            if (colour.isString())
            {
                colours.add(Colour::fromString(colour.toString()));
            }
        }
    }
}


// Packages the asset list into a juce::DynamicObject.
juce::DynamicObject* ThemeImages::AssetList::getDynamicObject() const
{
    using juce::DynamicObject;
    using juce::Array;
    using juce::var;
    DynamicObject * componentObject = new DynamicObject();
    if (imageFiles.size() > 0)
    {
        Array<var> imageFilesListed;
        for (int i = 0; i < imageFiles.size(); i++)
        {
            imageFilesListed.add(var(imageFiles[i]));
        }
        componentObject->setProperty(imageFileKey, imageFilesListed);
    }
    if (colours.size() > 0)
    {
        Array<var> coloursListed;
        for (int i = 0; i < colours.size(); i++)
        {
            coloursListed.add(var(colours[i].toString()));
        }
        componentObject->setProperty(imageColourKey, coloursListed);
    }
    return componentObject;
}


// Gets the configurable list of image files.
const juce::StringArray& ThemeImages::AssetList::getImageFiles() const
{
    return imageFiles;
}


// Gets the list of configurable colour values.
const juce::Array<juce::Colour>&
ThemeImages::AssetList::getImageColours() const
{
    return colours;
}
