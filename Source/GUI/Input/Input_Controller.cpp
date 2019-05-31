#include "Input_Controller.h"
#include "Input_Key_JSONKeys.h"
#include "HomeWindow.h"
#include "JuceHeader.h"
#include <map>

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Input::Controller::";
#endif


// Sets up all keyboard input handling.
Input::Controller::Controller(Component::MainView* mainView,
        const int targetWindow, const int keyChordWindow) :
    chordReader(mainView),
    mainView(mainView),
    inputBuffer(targetWindow, keyChordWindow)
{
    mainView->updateChordState(&charsetConfig.getActiveSet(), 0, {});
    chordReader.addListener(this);
}

// Updates the ChordComponent when the current held chord changes.
void Input::Controller::selectedChordChanged(const Chord selectedChord)
{
    mainView->updateChordState(&charsetConfig.getActiveSet(), 
            selectedChord, inputBuffer.getInputText());
}


// Adds the new chord character to the input buffer and updates the
// ChordComponent when a chord character is entered.
void Input::Controller::chordEntered(const Chord selected)
{
    unsigned int enteredChar = charsetConfig.getActiveSet()
            .getChordCharacter(selected, false);
    DBG(dbgPrefix << __func__ << ": Entered character "
            << (char) enteredChar << "(0x" 
            << juce::String::toHexString((int) enteredChar) << ", "
            << (int) enteredChar << ")");
    inputBuffer.appendCharacter(enteredChar);
    if (immediateMode)
    {
        inputBuffer.sendAndClearInput();
    }
    mainView->updateChordState(&charsetConfig.getActiveSet(), 0,
            inputBuffer.getInputText());
}


// Handles key commands used to send buffered text, delete buffered text, or
// close the application.
void Input::Controller::keyPressed(const juce::KeyPress key)
{
    DBG("Controller got " << key.getTextDescription());
    namespace Keys = Input::Key::JSONKeys;
    using Text::CharSet::Type;
    if (! key.isValid())
    {
        return;
    }

    // Attempts to update the character set, returning whether the active set
    // changed:
    const std::function<bool(const Type)> charSetUpdate = 
    [this](const Type type)
    {
        if (charsetConfig.getActiveType() != type)
        {
            charsetConfig.setActiveType(type);
            return true;
        }
        return false;
    };

    bool sendUpdate = false;
    const std::map<const juce::Identifier*, std::function<void()>> actionMap =
    {
        {
            &Keys::selectMainSet,
            [this, &sendUpdate, &charSetUpdate]() 
            {
                sendUpdate = charSetUpdate(Type::main);
            }
        },
        {
            &Keys::selectAltSet,
            [this, &sendUpdate, &charSetUpdate]() 
            {
                sendUpdate = charSetUpdate(Type::alt);
            }
        },
        {
            &Keys::selectSpecialSet,
            [this, &sendUpdate, &charSetUpdate]() 
            {
                sendUpdate = charSetUpdate(Type::special);
            }
        },
        {
            &Keys::selectNextSet,
            [this, &sendUpdate, &charSetUpdate]() 
            {
                int currentSetIndex = (int) charsetConfig.getActiveType();
                Type nextSet = (Type) ((currentSetIndex + 1)
                        % Text::CharSet::numCharacterSets);
                sendUpdate = charSetUpdate(nextSet);
            }
        },
        {
            &Keys::openModSelect,
            [this, &sendUpdate]()
            {
                DBG(dbgPrefix << __func__ << ": modifiers not implemented.");
            } 
        },
        {
            &Keys::toggleShift,
            [this, &sendUpdate]()
            {
                DBG(dbgPrefix << __func__ << ": modifiers not implemented.");
            } 
        },
        {
            &Keys::toggleCtrl,
            [this, &sendUpdate]()
            {
                DBG(dbgPrefix << __func__ << ": modifiers not implemented.");
            } 
        },
        {
            &Keys::toggleAlt,
            [this, &sendUpdate]()
            {
                DBG(dbgPrefix << __func__ << ": modifiers not implemented.");
            } 
        },
        {
            &Keys::toggleCmd,
            [this, &sendUpdate]()
            {
                DBG(dbgPrefix << __func__ << ": modifiers not implemented.");
            } 
        },
        {
            &Keys::backspace,
            [this, &sendUpdate]() 
            {
                inputBuffer.deleteLastChar();
                sendUpdate = true;
            }
        },
        {
            &Keys::clearAll,
            [this, &sendUpdate]() 
            {
                inputBuffer.clearInput();
                sendUpdate = true;
            } 
        },
        {
            &Keys::sendText,
            [this, &sendUpdate]() 
            { 
                DBG("Attempting to send text:");
                inputBuffer.sendAndClearInput();
                sendUpdate = true;
            }
        },
        { 
            &Keys::closeAndSend,
            [this, &sendUpdate]() 
            { 
                // inputBuffer will send its text on destruction.
                juce::JUCEApplication::getInstance()->quit();
            }
        },
        { 
            &Keys::close,
            [this, &sendUpdate]() 
            {
                inputBuffer.clearInput();
                juce::JUCEApplication::getInstance()->quit();
            } 
        },
        {
            &Keys::toggleImmediate,
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
            &Keys::showHelp,
            [this, &sendUpdate]()
            {
                DBG(dbgPrefix << __func__ << ": help text not implemented.");
            } 
        },
        {
            &Keys::toggleWindowEdge,
            []() 
            { 
                HomeWindow* window = HomeWindow::getOpenWindow();
                if (window != nullptr)
                {
                    window->toggleEdge();
                }
            } 
        },
        {
            &Keys::toggleMinimize,
            [this, &sendUpdate]()
            {
                DBG(dbgPrefix << __func__ << ": minimize not implemented.");
            } 
        },
    };
    for (const juce::Identifier* binding : Keys::allKeys)
    {
        if (keyConfig.getBoundKey(*binding) == key)
        {
            if (actionMap.count(binding) > 0)
            {
                actionMap.at(binding)();
            }
        }
    }
    if (sendUpdate)
    {
        mainView->updateChordState(&charsetConfig.getActiveSet(),
                chordReader.getSelectedChord(),
                inputBuffer.getInputText());

    }
}
