#include "HomeWindow.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "HomeWindow::";
// Make the window the size of the pocketCHIP display on debug builds:
static const constexpr int dbgWidth = 480;
static const constexpr int dbgHeight = 272;
#endif

// Creates and shows the main application window.
HomeWindow::HomeWindow(juce::String windowName) :
Windows::MainWindow(windowName, juce::Colours::darkgrey,
        juce::DocumentWindow::allButtons)
{
    juce::Rectangle<int> screenSize = juce::Desktop::getInstance()
            .getDisplays().getMainDisplay().userArea;
    setHeight(screenSize.getHeight() / 2);
    setUsingNativeTitleBar(true);
    setResizable(false, false);
    setLookAndFeel(&juce::LookAndFeel::getDefaultLookAndFeel());
    setVisible(true);
    setWantsKeyboardFocus(false);
    setColour(juce::DocumentWindow::backgroundColourId,
            juce::LookAndFeel::getDefaultLookAndFeel()
            .findColour(backgroundColourId));
}


// Gets a pointer to the current open window object.
HomeWindow* HomeWindow::getOpenWindow()
{
    juce::Component* rootComponent
            = juce::Desktop::getInstance().getComponent(0);
    return dynamic_cast<HomeWindow*>(rootComponent);
}

// Toggles window placement between the top and bottom of the display.
void HomeWindow::toggleEdge()
{
    bottomEdge = ! bottomEdge;
    setHeight(getHeight());
}

// Updates the window's height, while keeping the window snapped to the selected
// display edge.
void HomeWindow::setHeight(const int newHeight)
{
    juce::Rectangle<int> screenSize = juce::Desktop::getInstance()
            .getDisplays().getMainDisplay().userArea;
    int yPos = bottomEdge ? screenSize.getHeight() - newHeight : 0;
    setBounds(0, yPos, screenSize.getWidth(), newHeight);
}

// Closes the application normally when the window closes.
void HomeWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}


// Resizes page content to match the window size.
void HomeWindow::resized()
{
    const juce::Rectangle<int> bounds = getLocalBounds();
    for (juce::Component* contentComponent : getChildren())
    {
        if (contentComponent != nullptr)
        {
            contentComponent->setBounds(bounds);
        }
    }
}

/*
void HomeWindow::paint(juce::Graphics& g)
{
    g.setColour(juce::Colour(0xffffffff));
    g.fillRect(getLocalBounds());
}
*/
