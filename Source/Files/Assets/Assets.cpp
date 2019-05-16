#include "Assets.h"
#include "Assets_XPMLoader.h"

#ifdef JUCE_DEBUG
// Print namespace before all debug output:
static const constexpr char* dbgPrefix = "Assets::";
#endif

// TODO: Select this using XDGDirectories:
static const constexpr char* assetFolder
        = "/usr/share/" JUCE_TARGET_APP "/";


/**
 * @brief  Locates a file from an absolute or local path.
 *
 * @param path  A local or absolute file path string.
 *
 * @return      A file with the given path, which may or may not be a valid
 *              file.
 */
static juce::File absoluteFileFromPath(const juce::String& path)
{
    using juce::File;
    return File::isAbsolutePath(path) ? File(path)
            : File::getCurrentWorkingDirectory().getChildFile(path);
}


// Loads an asset file using its asset name.
juce::File Assets::findAssetFile
(const juce::String& assetName, bool lookOutsideAssets)
{
    juce::File assetFile
            = absoluteFileFromPath(juce::String(assetFolder) + assetName);
    if (!assetFile.existsAsFile() && lookOutsideAssets)
    {
        assetFile = absoluteFileFromPath(assetName);
    }
    #ifdef JUCE_DEBUG
    if (!assetFile.exists())
    {
        DBG(dbgPrefix << __func__ << ": Failed to find asset file \""
                << assetName << "\"");
    }
    #endif
    return assetFile;
}


// Creates an Image object from an asset file.
juce::Image Assets::loadImageAsset
(const juce::String& assetName, bool lookOutsideAssets)
{
    using juce::Image;
    if (assetName.endsWith(".svg"))
    {
        Image image;
        std::unique_ptr<juce::Drawable> svgDrawable
                (loadSVGDrawable(assetName, lookOutsideAssets));
        if (svgDrawable != nullptr)
        {
            image = Image(Image::ARGB, 128, 128, true);
            juce::Graphics g(image);
            juce::Rectangle<float> imgBounds(0, 0, image.getWidth(),
                    image.getHeight());
            juce::Rectangle<float> svgBounds = ((juce::DrawableComposite*)
                    svgDrawable.get())->getContentArea();
            juce::RectanglePlacement svgPlacement(
                    juce::RectanglePlacement::centred);
            juce::AffineTransform svgTransform = svgPlacement.getTransformToFit(
                    svgBounds, imgBounds);
            svgDrawable->draw(g, 1.f, svgTransform);
        }
        return image;

    }
    juce::File imageFile = findAssetFile(assetName, lookOutsideAssets);
    if (assetName.endsWith(".xpm"))
    {
        DBG("Loading xpm " << assetName);
        return XPMLoader::loadXPMImage(imageFile);
    }
    return juce::ImageFileFormat::loadFrom(imageFile);
}


// Creates a Drawable object from an svg asset file.
juce::Drawable * Assets::loadSVGDrawable
(const juce::String& assetName, bool lookOutsideAssets)
{
    juce::File svgFile = findAssetFile(assetName, lookOutsideAssets);
    if (!svgFile.existsAsFile() || svgFile.getFileExtension() != ".svg")
    {
        DBG(dbgPrefix << __func__ << ": File \"" << svgFile.getFileName()
                << "\" not found.");
        return nullptr;
    }
    std::unique_ptr<juce::XmlElement> svgElement
            (juce::XmlDocument::parse(svgFile));
    if (svgElement == nullptr)
    {
        DBG(dbgPrefix << __func__ << ": File \"" << svgFile.getFileName()
                << "\" is not a valid svg file.");
        return nullptr;
    }
    return juce::Drawable::createFromSVG(*svgElement);
}


// Loads JSON data from an asset file.
juce::var Assets::loadJSONAsset
(const juce::String& assetName, bool lookOutsideAssets)
{
    juce::File jsonFile = findAssetFile(assetName, lookOutsideAssets);
    if (!jsonFile.existsAsFile() || jsonFile.getFileExtension() != ".json")
    {
        #ifdef JUCE_DEBUG
        if (jsonFile.existsAsFile())
        {
            DBG(dbgPrefix << __func__ << ": " << assetName
                    << " exists but is not a valid JSON file.");
        }
        #endif
        return juce::var();
    }
    return juce::JSON::parse(jsonFile);
}
