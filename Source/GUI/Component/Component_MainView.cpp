#include "Component_MainView.h"
#include "Input_Key_JSONKeys.h"
#include "Text_BinaryFont.h"
#include "Text_Painter.h"
#include "Text_ModTracker.h"
#include "Text_CharSet_Values.h"
#include "Text_CharSet_ConfigFile.h"
#include "MainWindow.h"
#include <map>
#include <vector>

// Character padding values, as a fraction of character size:
static const constexpr float xPaddingFraction = 0.1;
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

    Text::CharSet::ConfigFile charSetConfig;
    const Text::CharSet::Cache& activeSet = charSetConfig.getActiveSet();

    for (KeyGrid* keyGrid : keyGrids)
    {
        keyGrid->setPaddingFractions(xPaddingFraction, yPaddingFraction);
        keyGrid->updateCharacterSet(&activeSet);
        addAndMakeVisible(keyGrid);
    }
    addAndMakeVisible(inputView);
    addChildComponent(helpScreen);
}


// Sets the current state of the chorded keyboard, immediately redrawing the
// component if the state changes.
void Component::MainView::updateChordState(
        const Text::CharSet::Cache* activeSet, 
        const Chord heldChord,
        const juce::Array<unsigned int> input)
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
    helpScreen.setVisible(showHelpScreen);
    charsetDisplay.setVisible(! showHelpScreen);
    chordKeyDisplay.setVisible(! showHelpScreen);
    chordPreview.setVisible(! showHelpScreen);
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
    Text::CharSet::ConfigFile charSetConfig;
    const Text::CharSet::Cache& charSet = charSetConfig.getActiveSet();

    // One row for each chord key, one for the list of chord characters, and
    // one for the input text preview.
    const int rowCount = Chord::numChordKeys() + 2;
    const int rowHeight = getHeight() / rowCount;
    // One column for each character in the current set, an extra column for
    // each wide-draw character, and one for the chord key display.
    int columnCount = 1 + charSet.getSize() + charSet.wideDrawCharacterCount();
    const int columnWidth = getWidth() / columnCount;

    chordKeyDisplay.setBounds(0, rowHeight, columnWidth,
            rowHeight * chordKeyDisplay.getRowCount());
    charsetDisplay.setBounds(columnWidth, 0, getWidth() - columnWidth,
            rowHeight);
    chordPreview.setBounds(columnWidth, rowHeight,
            chordPreview.getColumnCount() * columnWidth,
            rowHeight * chordPreview.getRowCount());
    juce::Rectangle<int> inputBounds = getLocalBounds().withTop(
            chordPreview.getBottom());
    int marginSize = (inputBounds.getHeight() * inputMargin)
            / (inputMargin + 1);
    inputView.setBounds(inputBounds.reduced(marginSize, marginSize));
    helpScreen.setBounds(getLocalBounds());
}
