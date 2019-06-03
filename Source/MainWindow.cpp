#include "MainWindow.h"
#include "Windows_FocusControl.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "MainWindow::";
// Make the window the size of the GameShell display on debug builds:
static const constexpr int dbgWidth = 320;
static const constexpr int dbgHeight = 240;
#endif

// Creates and shows the main application window.
MainWindow::MainWindow(juce::String windowName) :
juce::DocumentWindow(windowName, juce::Colours::darkgrey,
        juce::DocumentWindow::allButtons)
{
    juce::Rectangle<int> screenSize = juce::Desktop::getInstance()
            .getDisplays().getMainDisplay().userArea;
#ifdef JUCE_DEBUG
    setHeight(dbgHeight / 2);
#else
    setHeight(screenSize.getHeight() / 2);
#endif
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
MainWindow* MainWindow::getOpenWindow()
{
    juce::Component* rootComponent
            = juce::Desktop::getInstance().getComponent(0);
    return dynamic_cast<MainWindow*>(rootComponent);
}

// Toggles window placement between the top and bottom of the display.
void MainWindow::toggleEdge()
{
    bottomEdge = ! bottomEdge;
    setHeight(getHeight());
}

// Updates the window's height, while keeping the window snapped to the selected
// display edge.
void MainWindow::setHeight(const int newHeight)
{
    juce::Rectangle<int> screenSize = juce::Desktop::getInstance()
            .getDisplays().getMainDisplay().userArea;
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

    // The GameShell keeps losing focus when the window switches screen sides.
    // Make sure that doesn't happen here.
    Windows::FocusControl focusControl;
    focusControl.takeFocus();
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
