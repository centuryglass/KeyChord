#include "Assets_XPMLoader.h"
#include "JuceHeader.h"
#include <X11/xpm.h>

#ifdef JUCE_DEBUG
// Print the full namespace name before all debug output:
static const constexpr char* dbgPrefix = "Assets::XPMLoader::";
#endif


// Loads a juce Image object from a xpm image file.
juce::Image Assets::XPMLoader::loadXPMImage(const juce::File xpmFile)
{
    using juce::Image;
    Image juceImage;
    if (!xpmFile.existsAsFile())
    {
        return juceImage;
    }

    // Read file to xpm image data:
    XpmImage imageData;
    int result = XpmReadFileToXpmImage(xpmFile.getFullPathName().toRawUTF8(),
            &imageData, nullptr);
    if (result != XpmSuccess)
    {
        DBG(dbgPrefix << __func__ << ": Error loading \""
                << xpmFile.getFullPathName());
        DBG(dbgPrefix << __func__ << ": Error=" << XpmGetErrorString(result));
        return juceImage;
    }
    juceImage = Image(Image::PixelFormat::RGB, imageData.width,
            imageData.height, true);

    // Load X11 color names:
    juce::Array<juce::Colour> imageColours;
    Display * display = XOpenDisplay(nullptr);
    if (display != nullptr)
    {
        for (int i = 0; i < imageData.ncolors; i++)
        {
            char* c_color = imageData.colorTable[i].c_color;
            if (c_color == nullptr)
            {
                imageColours.add(juce::Colours::transparentBlack);
                continue;
            }
            XColor colorValue;
            Colormap colorMap = XDefaultColormap(display, 0);
            Status searchResult = XParseColor(display, colorMap, c_color,
                    &colorValue);
            if (searchResult == 0)
            {
                imageColours.add(juce::Colours::transparentBlack);
                continue;
            }
            //Divide color values by 256 to convert to uint8:
            imageColours.add(juce::Colour(
                        juce::uint8(colorValue.red / 256),
                        juce::uint8(colorValue.green / 256),
                        juce::uint8(colorValue.blue / 256)));
        }
        XCloseDisplay(display);
        display = nullptr;
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Failed to load X11 color values");
        return juceImage;
    }

    // Copy image data into juce::Image:
    for (int y = 0; y < imageData.height; y++)
    {
        for (int x = 0; x < imageData.width; x++)
        {
            const int dataIndex = x + y * imageData.width;
            const int colourIndex = imageData.data[dataIndex];
            juceImage.setPixelAt(x, y, imageColours[colourIndex]);
        }
    }

    XpmFreeXpmImage(&imageData);
    return juceImage;
}
