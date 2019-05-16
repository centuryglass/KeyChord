#include "Assets_XPMLoader.h"
#include "Assets.h"
#include "Testing_Window.h"
#include "Widgets_DrawableImage.h"
#include "JuceHeader.h"
#include <X11/xpm.h>

// Test window bounds:
static const constexpr int winX = 5;
static const constexpr int winY = 5;
static const constexpr int winWidth = 480;
static const constexpr int winHeight = 272;

// Test file paths:
static const juce::StringArray testFiles =
{
    "testing/XPM/test1.xpm",
    "testing/XPM/test2.xpm",
    "testing/XPM/test3.xpm"
};

namespace Assets { class XpmTest; }

/**
 * @brief  Tests loading images with the .xpm image format using
 *         Assets::xpmImage.
 */
class Assets::XpmTest : public juce::UnitTest
{
public:
    XpmTest() : juce::UnitTest("xpm Image Testing",
            "Assets") {}


    void runTest() override
    {
        beginTest("Displaying xpm images");

        int testIndex = 0;
        for (const juce::String& assetPath : testFiles)
        {
            juce::File assetFile(findAssetFile(assetPath, false));
            expect(assetFile.existsAsFile(), assetPath + " not found!");
            juce::Image xpmImage = Assets::XPMLoader::loadXPMImage(assetFile);
            expect(xpmImage.isValid(), assetPath
                    + " not loaded as a valid image.");
            Widgets::DrawableImage* drawnImage
                    = new Widgets::DrawableImage(xpmImage);
            Testing::Window imageWindow("XPM Testing", drawnImage, winX, winY,
                    winWidth, winHeight);
            juce::MessageManager::getInstance()->runDispatchLoopUntil(5000);
        }

    }
};

static Assets::XpmTest test;
