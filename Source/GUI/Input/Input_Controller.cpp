#include "Input_Controller.h"

// Action key definitions:
// TODO: define and load these using a configuration file.
namespace ActionKey
{
    // Deletes the last character of buffered input:
    static const constexpr char* backspace = "backspace";
    // Clears all buffered input:
    static const constexpr char* clearAll = "delete";
    // Clears and sends buffered input:
    static const constexpr char* send = "return";
    // Closes the application, sending buffered input:
    static const constexpr char* closeAndSend = "end";
    // Closes the application, discarding buffered input:
    static const constexpr char* closeWithoutSending = "escape";
    // Toggles immediate entry mode, where all input is immediately forwarded
    // to the target window without buffering:
    static const constexpr char* toggleImmediateMode = "I";
}

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
    bool sendUpdate = false;
    if (key == ActionKey::backspace)
    {
        inputBuffer.deleteLastChar();
        sendUpdate = true;
    }
    else if (key == ActionKey::clearAll)
    {
        inputBuffer.clearInput();
        sendUpdate = true;
    }
    else if (key == ActionKey::send)
    {
        inputBuffer.sendAndClearInput();
        sendUpdate = true;
    }
    else if (key == ActionKey::closeAndSend)
    {
        // inputBuffer will send its text on destruction.
        juce::JUCEApplication::getInstance()->quit();
    }
    else if (key == ActionKey::closeWithoutSending)
    {
        inputBuffer.clearInput();
        juce::JUCEApplication::getInstance()->quit();
    }
    else if (key == ActionKey::toggleImmediateMode)
    {
        immediateMode = ! immediateMode;
        if (immediateMode && ! inputBuffer.isEmpty())
        {
            inputBuffer.sendAndClearInput();
            sendUpdate = true;
        }
    }

    if (sendUpdate)
    {
        chordPreview->updateChordState(chordReader.getAlphabet(),
                chordReader.getSelectedChord(),
                inputBuffer.getInputText());
    }
}
