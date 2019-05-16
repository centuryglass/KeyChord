#include "Layout_Component_ConfigFile.h"
#include "Layout_Component_JSONKeys.h"
#include "Layout_Component_JSONResource.h"
#include "Layout_Component_ConfigLayout.h"
#include "Windows_Info.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Layout::Component::ConfigFile::";
#endif

namespace ComponentLayout = Layout::Component;

ComponentLayout::ConfigFile::ConfigFile() { }


// Gets configured component layouts from shared JSON file data.
ComponentLayout::ConfigLayout ComponentLayout::ConfigFile::getLayout
(const juce::Identifier& componentKey)
{
    SharedResource::LockedPtr<const JSONResource> config
            = getReadLockedResource();
    return config->getLayout(componentKey);
}


// Return the most appropriate font size for drawing text
int ComponentLayout::ConfigFile::getFontHeight
(juce::Rectangle <int> textBounds, juce::String text)
{
    SharedResource::LockedPtr<const JSONResource> config
            = getReadLockedResource();
    int numLines = 1;
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            numLines++;
        }
    }
    int height = textBounds.getHeight() / numLines;
    juce::Font defaultFont(height);
    int width = defaultFont.getStringWidth(text);
    if (width > textBounds.getWidth())
    {
        height = textBounds.getWidth() * height / width;
    }
    int testHeight = getFontHeight(TextSize::largeText);
    if (height > testHeight)
    {
        return testHeight;
    }
    testHeight = getFontHeight(TextSize::mediumText);
    if (height > testHeight)
    {
        return testHeight;
    }
    testHeight = getFontHeight(TextSize::smallText);
    if (height > testHeight)
    {
        return testHeight;
    }
    return height;
}


// Gets the height in pixels of one of the three configured text sizes.
int ComponentLayout::ConfigFile::getFontHeight(TextSize sizeType)
{
    using juce::var;
    namespace Keys = Layout::Component::JSONKeys;
    const juce::Identifier& key = (sizeType == TextSize::smallText ?
            Keys::smallText :
            (sizeType == TextSize::mediumText ?
                Keys::mediumText : Keys::largeText));
    var size = getReadLockedResource()->getConfigValue<var>(key);
    if (size.isDouble())
    {
        const int height = Windows::Info::getHeight();
        const int width = Windows::Info::getWidth();
        return (int)((double) size * (height < width ? height : width));
    }
    else if (size.isInt())
    {
        return (int) size;
    }
    else
    {
        DBG(dbgPrefix << __func__ << ": Invalid text size value: "
                << size.toString());
        jassertfalse;
        return 0;
    }
}
