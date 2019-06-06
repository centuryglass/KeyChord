#include "MainWindow.h"
#include "Windows_FocusControl.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "MainWindow::";

// Make the window the size of the GameShell display on debug builds:
static const constexpr int dbgWidth = 320;
static const constexpr int dbgHeight = 240;
#endif

// Used to determine how much of the screen height to use:
static const constexpr int standardHeightDivisor = 2;
static const constexpr int minimizedHeightDivisor = 10;

// Creates and shows the main application window.
MainWindow::MainWindow(juce::String windowName) :
juce::DocumentWindow(windowName, juce::Colours::darkgrey,
        juce::DocumentWindow::allButtons)
{
    const int heightDivisor = (mainConfig.getMinimized()
            ? minimizedHeightDivisor : standardHeightDivisor);
    juce::Rectangle<int> screenSize = juce::Desktop::getInstance()
            .getDisplays().getMainDisplay().userArea;
//#ifdef JUCE_DEBUG
//    setHeight(dbgHeight / heightDivisor);
//#else
    setHeight(screenSize.getHeight() / heightDivisor);
//#endif
    setUsingNativeTitleBar(false);
    setResizable(false, false);
    setLookAndFeel(&juce::LookAndFeel::getDefaultLookAndFeel());
    setVisible(true);
    setWantsKeyboardFocus(false);
    setColour(juce::DocumentWindow::backgroundColourId,
            juce::LookAndFeel::getDefaultLookAndFeel()
            .findColour(backgroundColourId));
}


// Gets a pointer to the current open window object.
MainWindow* MainWindow::getOpenWindow()
{
    juce::Component* rootComponent
            = juce::Desktop::getInstance().getComponent(0);
    return dynamic_cast<MainWindow*>(rootComponent);
}


// Updates the window's height, while keeping the window snapped to the selected
// display edge.
void MainWindow::setHeight(const int newHeight)
{
    juce::Rectangle<int> screenSize = juce::Desktop::getInstance()
            .getDisplays().getMainDisplay().userArea;
    const bool bottomEdge = mainConfig.getSnapToBottom();
#ifdef JUCE_DEBUG
    int adjustedHeight = newHeight;
    if (adjustedHeight > dbgHeight)
    {
        adjustedHeight = newHeight * dbgHeight / screenSize.getHeight();
    }
    int yPos = bottomEdge ? screenSize.getHeight() - adjustedHeight : 0;
    setBounds((screenSize.getWidth() - dbgWidth) / 2 , yPos, dbgWidth,
            adjustedHeight);
#else
    int yPos = bottomEdge ? screenSize.getHeight() - newHeight : 0;
    setBounds(0, yPos, screenSize.getWidth(), newHeight);
#endif
}


// Closes the application normally when the window closes.
void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}


// Resizes page content to match the window size.
void MainWindow::resized()
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
