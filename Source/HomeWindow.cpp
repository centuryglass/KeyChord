#include "HomeWindow.h"
#include "Hardware_Display.h"

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
    juce::Rectangle<int> screenSize = Hardware::Display::getSize();
    setBounds(0, screenSize.getHeight() / 2, screenSize.getWidth(),
            screenSize.getHeight() / 2);
    setUsingNativeTitleBar(true);
    setResizable(true, false);
    setLookAndFeel(&juce::LookAndFeel::getDefaultLookAndFeel());
    setVisible(true);
    setWantsKeyboardFocus(false);

    setContentNonOwned(&pageStack, true);
    pageStack.setRootPage((Page::Interface::Component*)
            pageFactory.createHomePage());
}


// Gets a pointer to the current open window object.
HomeWindow* HomeWindow::getOpenWindow()
{
    juce::Component* rootComponent
            = juce::Desktop::getInstance().getComponent(0);
    return dynamic_cast<HomeWindow*>(rootComponent);
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
    pageStack.setBounds(bounds);
}

