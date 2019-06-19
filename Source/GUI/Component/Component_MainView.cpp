#include "Component_MainView.h"
#include "Input_Key_JSONKeys.h"
#include "Text_BinaryFont.h"
#include "Text_Painter.h"
#include "Text_Values.h"
#include "MainWindow.h"
#include "Config_MainFile.h"
#include <map>
#include <vector>

// Character padding values, as a fraction of character size:
static const constexpr float xPaddingFraction = 0.15;
static const constexpr float yPaddingFraction = 0.1;
// Input preview margin size, as a fraction of input preview area height:
static const constexpr float inputMargin = 0.05;

//  Requests keyboard focus on construction.
Component::MainView::MainView()
{
    setWantsKeyboardFocus(true);
    KeyGrid* keyGrids [] =
    {
        &charsetDisplay,
        &chordPreview,
        &chordKeyDisplay
    };

    const Text::CharSet::Cache& activeSet = charsetConfig.getActiveSet();

    for (KeyGrid* keyGrid : keyGrids)
    {
        keyGrid->setPaddingFractions(xPaddingFraction, yPaddingFraction);
        keyGrid->updateCharacterSet(&activeSet);
    }
    addAndMakeVisible(charsetDisplay);
    addAndMakeVisible(chordPreview);
    addAndMakeVisible(chordKeyDisplay);
    addAndMakeVisible(inputView);
    addChildComponent(helpScreen);
}


// Updates the current state of the chorded keyboard, immediately redrawing the
// component if the state changes.
void Component::MainView::updateChordState(
        const Text::CharSet::Cache* activeSet,
        const Input::Chord heldChord,
        const Text::CharString input)
{
    KeyGrid* keyGrids [] =
    {
        &charsetDisplay,
        &chordPreview,
        &chordKeyDisplay
    };
    for (KeyGrid* keyGrid : keyGrids)
    {
        keyGrid->updateCharacterSet(activeSet);
        keyGrid->updateChordState(heldChord);
    }
    inputView.updateInputText(input);
    resized();
}


// Shows the help screen if it's not currently visible, or hides it if it is
// visible.
void Component::MainView::toggleHelpScreen()
{
    const bool showHelpScreen = ! helpScreen.isVisible();
    const bool minimized = mainConfig.getMinimized();
    helpScreen.setVisible(showHelpScreen);
    charsetDisplay.setVisible(! showHelpScreen);
    chordKeyDisplay.setVisible(! showHelpScreen && ! minimized);
    chordPreview.setVisible(! showHelpScreen && ! minimized);
    inputView.setVisible(! showHelpScreen);
    repaint();
}


// Checks if the help screen is currently being shown.
bool Component::MainView::isHelpScreenShowing() const
{
    return helpScreen.isVisible();
}


// Update child component bounds if the component changes size.
void Component::MainView::resized()
{
    helpScreen.setBounds(getLocalBounds());
    const Text::CharSet::Cache& charSet = charsetConfig.getActiveSet();

    const bool minimized = mainConfig.getMinimized();
    const bool showHelpScreen = helpScreen.isVisible();

    chordKeyDisplay.setVisible(! showHelpScreen && ! minimized);
    chordPreview.setVisible(! showHelpScreen && ! minimized);

    // Only display the character set and input buffer if minimized:
    if (minimized)
    {
        const int rowHeight = getHeight() / 2;
        charsetDisplay.setBounds(getLocalBounds().withHeight(rowHeight));
        juce::Rectangle<int> inputBounds = getLocalBounds().withTop(
                charsetDisplay.getBottom());
        int marginSize = (inputBounds.getHeight() * inputMargin)
                / (inputMargin + 1);
        inputView.setBounds(inputBounds.reduced(marginSize, marginSize));
        DBG("Minimized input view bounds: " << inputView.getBounds().toString()
                << ", rowHeight=" << rowHeight << ", marginSize=" << marginSize);
        return;
    }

    // Calculate row and column counts and sizes:
    // One row for each chord key, one for the list of chord characters, and
    // one for the input text preview.
    const int rowCount = Input::Chord::numChordKeys() + 2;
    const int rowHeight = getHeight() / rowCount;

    // One column for each character in the current set, an extra column for
    // each wide-draw character, and one for the chord key display.
    int columnCount = 1 + charSet.getSize() + charSet.wideDrawCharacterCount();
    const int columnWidth = getWidth() / columnCount;

    // Use column and row sizes to align all components:
    chordKeyDisplay.setBounds(0, rowHeight, columnWidth,
            rowHeight * chordKeyDisplay.getRowCount());
    charsetDisplay.setBounds(columnWidth, 0, getWidth() - columnWidth,
            rowHeight);
    chordPreview.setBounds(columnWidth, rowHeight,
            getWidth() - columnWidth,
            rowHeight * chordPreview.getRowCount());
    juce::Rectangle<int> inputBounds = getLocalBounds().withTop(
            chordPreview.getBottom());
    int marginSize = (inputBounds.getHeight() * inputMargin)
            / (inputMargin + 1);
    inputView.setBounds(inputBounds.reduced(marginSize, marginSize));
}


// Makes sure the background is filled in with the appropriate background color.
void Component::MainView::paint(juce::Graphics& g)
{
    g.setColour(findColour(juce::DocumentWindow::backgroundColourId));
    g.fillRect(getLocalBounds());
}
