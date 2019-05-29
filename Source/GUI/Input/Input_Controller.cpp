#include "Input_Controller.h"
#include "Input_Key_JSONKeys.h"
#include "JuceHeader.h"
#include <map>

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Input::Controller::";
#endif


// Sets up all keyboard input handling.
Input::Controller::Controller(Component::ChordPreview* chordPreview,
        const Window targetWindow, const Window keyChordWindow) :
    chordReader(chordPreview),
    chordPreview(chordPreview),
    inputBuffer(targetWindow, keyChordWindow)
{
    chordPreview->updateChordState(chordReader.getAlphabet(), 0, "");
    chordReader.addListener(this);
}

// Updates the ChordComponent when the current held chord changes.
void Input::Controller::selectedChordChanged(const Chord selectedChord)
{
    chordPreview->updateChordState(chordReader.getAlphabet(), 
            selectedChord, inputBuffer.getInputText());
}


// Adds the new chord character to the input buffer and updates the
// ChordComponent when a chord character is entered.
void Input::Controller::chordEntered(const Chord selected)
{
    inputBuffer.appendCharacter(chordReader.getAlphabet()
            ->getChordCharacter(selected));
    if (immediateMode)
    {
        inputBuffer.sendAndClearInput();
    }
    chordPreview->updateChordState(chordReader.getAlphabet(), 0,
            inputBuffer.getInputText());
}


// Updates the ChordComponent when the active alphabet changes.
void Input::Controller::alphabetChanged(const Input::Key::Alphabet* alphabet)
{
    chordPreview->updateChordState(alphabet, chordReader.getSelectedChord(),
            inputBuffer.getInputText());
}


// Handles key commands used to send buffered text, delete buffered text, or
// close the application.
void Input::Controller::keyPressed(const juce::String key)
{
    using namespace Input::Key::JSONKeys;
    if (key.isEmpty())
    {
        return;
    }
    bool sendUpdate = false;
    const std::map<const juce::Identifier*, std::function<void()>> actionMap =
    {
        {
            &backspace,
            [this, &sendUpdate]() 
            {
                inputBuffer.deleteLastChar();
                sendUpdate = true;
            }
        },
        {
            &clearAll,
            [this, &sendUpdate]() 
            {
                inputBuffer.clearInput();
                sendUpdate = true;
            } 
        },
        {
            &sendText,
            [this, &sendUpdate]() 
            { 
                inputBuffer.sendAndClearInput();
                sendUpdate = true;
            }
        },
        { 
            &closeAndSend,
            [this, &sendUpdate]() 
            { 
                // inputBuffer will send its text on destruction.
                juce::JUCEApplication::getInstance()->quit();
            }
        },
        { 
            &close,
            [this, &sendUpdate]() 
            {
                inputBuffer.clearInput();
                juce::JUCEApplication::getInstance()->quit();
            } 
        },
        {
            &toggleImmediate,
            [this, &sendUpdate]() 
            {
                immediateMode = ! immediateMode;
                if (immediateMode && ! inputBuffer.isEmpty())
                {
                    inputBuffer.sendAndClearInput();
                    sendUpdate = true;
                }
            } 
        },
        {
            &showHelp,
            [this, &sendUpdate]()
            {
                DBG(dbgPrefix << __func__ << ": help text not implemented.");
            } 
        },
        {
            &toggleWindowEdge,
            [this, &sendUpdate]() 
            { 
                DBG(dbgPrefix << __func__ << ": window move not implemented.");
            } 
        },
        {
            &toggleMinimize,
            [this, &sendUpdate]()
            {
                DBG(dbgPrefix << __func__ << ": minimize not implemented.");
            } 
        },
    };
    for (const juce::Identifier* binding : allKeys)
    {
        if (keyConfig.getBoundKey(*binding).getTextDescription()
                == key)
        {
            if (actionMap.count(binding) > 0)
            {
                actionMap.at(binding)();
            }
        }
    }
    if (sendUpdate)
    {
        chordPreview->updateChordState(chordReader.getAlphabet(),
                chordReader.getSelectedChord(),
                inputBuffer.getInputText());

    }
}
