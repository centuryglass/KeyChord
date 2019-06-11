#include "MainWindow.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "MainWindow::";
#endif

// Milliseconds to wait before closing the application after window focus is
// lost.
static const constexpr int windowTimeout = 2000;

// Creates and shows the main application window.
MainWindow::MainWindow(juce::String windowName) :
juce::DocumentWindow(windowName, juce::Colours::darkgrey,
        juce::DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(false);
    setResizable(false, false);
    setLookAndFeel(&juce::LookAndFeel::getDefaultLookAndFeel());
    setVisible(true);
    setWantsKeyboardFocus(false);
    setColour(juce::DocumentWindow::backgroundColourId,
            juce::LookAndFeel::getDefaultLookAndFeel()
            .findColour(backgroundColourId));
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


// Starts the exit timer when the window loses focus, and stops if if the window
// gains focus again.
void MainWindow::activeWindowStatusChanged()
{
    if (isActiveWindow())
    {
        exitTimer.stopTimer();
    }
    else
    {
        exitTimer.startTimer(windowTimeout);
    }
}

void MainWindow::ExitTimer::timerCallback()
{
    DBG(dbgPrefix << __func__ << ": Window focus lost, closing application.");
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
