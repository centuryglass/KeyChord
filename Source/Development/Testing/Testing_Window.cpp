#include "Testing_Window.h"
#include "JuceHeader.h"

// Creates and shows a window holding a test component.
Testing::Window::Window(const juce::String testName,
        juce::Component* const testComponent,
        const int xPos,
        const int yPos,
        const int width,
        const int height) :
Windows::MainWindow(testName, juce::Colours::white,
        juce::DocumentWindow::allButtons)
{
    setBounds(xPos, yPos, width, height);
    setLookAndFeel(&juce::LookAndFeel::getDefaultLookAndFeel());
    setUsingNativeTitleBar(true);
    setResizable(true, false);
    setVisible(true);
    if (testComponent != nullptr)
    {
        if (testComponent->getBounds().isEmpty())
        {
            testComponent->setBounds(0, 0, width, height);
        }
        testComponent->setVisible(true);
        DocumentWindow::setContentOwned(testComponent, false);
    }
    addToDesktop();
}
