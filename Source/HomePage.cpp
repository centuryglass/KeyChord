#include "HomePage.h"
#include "Theme_Image_JSONKeys.h"
#include "Assets.h"
#include "Page_Type.h"
#include "Util_SafeCall.h"


// Initializes all page components.
HomePage::HomePage()
{
    #if JUCE_DEBUG
    setName("HomePage");
    #endif

    addTrackedKey(Theme::Image::JSONKeys::homeBackground);
    addAndMakeVisible(chordComponent);
    setWantsKeyboardFocus(false);
    loadAllConfigProperties();
}


// Tracks page background changes.
void HomePage::configValueChanged(const juce::Identifier& key)
{
    using juce::String;
    using juce::Image;
    if (key == Theme::Image::JSONKeys::homeBackground)
    {
        String background = imageConfig.getHomeBackgroundPath();
        setBackgroundImage(Assets::loadImageAsset(background));
    }
    else
    {
        //invalid key!
        jassertfalse;
    }
}


// Update all child component bounds when the page is resized.
void HomePage::pageResized()
{ 
    chordComponent.setBounds(getLocalBounds());
}
