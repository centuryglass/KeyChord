#include "Input_Controller.h"
#include "Input_Key_JSONKeys.h"
#include "Text_Values.h"
#include "Output_Sending.h"
#include "Output_Modifiers.h"
#include "Application.h"
#include "JuceHeader.h"
#include <map>

static const juce::Identifier localeKey("Input_Controller");
static const juce::Identifier immediateModeKey("immediateMode");

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Input::Controller::";
#endif


// Sets up all keyboard input handling.
Input::Controller::Controller(Component::MainView* mainView,
        const int targetWindow, Output::Buffer& outputBuffer) :
    Locale::TextUser(localeKey),
    chordReader(mainView),
    mainView(mainView),
    targetWindow(targetWindow),
    outputBuffer(outputBuffer)
{
    chordReader.addListener(this);
    mainView->updateChordState(&charsetConfig.getActiveSet(), 0,
            getInputPreview());
}


// Gets a CharString displaying appropriate input preview text.
Text::CharString Input::Controller::getInputPreview() const
{
    using juce::String;
    namespace Modifiers = Output::Modifiers;
    Text::CharString inputText;
    // Add active modifiers to drawn text:
    const int modifierFlags = outputBuffer.getModifierFlags();
    const std::pair<Modifiers::TypeFlag, Text::CharValue> modMappings [] =
    {
        { Modifiers::control, Text::Values::ctrl },
        { Modifiers::alt    , Text::Values::alt },
        { Modifiers::shift  , Text::Values::shift },
        { Modifiers::super  , Text::Values::super }
    };
    for (const auto& mapping : modMappings)
    {
        if ((modifierFlags & (int) mapping.first) != 0)
        {
            inputText.add(mapping.second);
            inputText.add((Text::CharValue) '+');
        }
    }
    if (mainConfig.getImmediateMode())
    {
        // Remove the last '+' if modifiers were set.
        inputText.removeLast();
        String immediateModeText = localeText(immediateModeKey);
        for (int i = 0; i < immediateModeText.length(); i++)
        {
            inputText.add(Text::Values::getCharValue(
                    immediateModeText.substring(i, i + 1)));
        }
    }
    else
    {
        inputText.addArray(outputBuffer.getBufferedText());
    }
    return inputText;
}


// Updates the ChordComponent when the current held chord changes.
void Input::Controller::selectedChordChanged(const Chord selectedChord)
{
    mainView->updateChordState(&charsetConfig.getActiveSet(), 
            selectedChord, getInputPreview());
}


// Adds the new chord character to the input buffer and updates the
// ChordComponent when a chord character is entered.
void Input::Controller::chordEntered(const Chord selected)
{
    namespace Modifiers = Output::Modifiers;
    const juce::ScopedTryLock inputLock(inputGuard);
    if (!inputLock.isLocked())
    {
        DBG(dbgPrefix << __func__ 
                << ": Still busy doing something else, ignoring input.");
        return;
    }
    if (mainView->isHelpScreenShowing())
    {
        // When the help screen is open, all input events just close the help
        // screen.
        closeHelpScreen();
        return;
    }
    Text::CharValue enteredChar = charsetConfig.getActiveSet()
            .getChordCharacter(selected, charsetConfig.getShifted());
    /*
    DBG(dbgPrefix << __func__ << ": Entered character "
            << (char) enteredChar << "(0x" 
            << juce::String::toHexString((int) enteredChar) << ", "
            << (int) enteredChar << ")");
    */
    if (Text::Values::isModifier(enteredChar))
    {
        int modFlags = 0;
        switch (enteredChar)
        {
            case Text::Values::ctrl:
                modFlags = Modifiers::control;
                break;
            case Text::Values::alt:
                modFlags = Modifiers::alt;
                break;
            case Text::Values::shift:
                modFlags = Modifiers::shift;
                break;
            case Text::Values::super:
                modFlags = Modifiers::super;
                break;
            default:
                DBG(dbgPrefix << __func__ << ": Unexpected key value "
                        << juce::String(enteredChar) 
                        << " incorrectly evaluated as modifier.");
                jassertfalse;
        }
        const int currentFlags = outputBuffer.getModifierFlags();
        if ((currentFlags & modFlags) != 0)
        {
            modFlags = currentFlags & ~modFlags;
        }
        else
        {
            modFlags |= currentFlags;
        }
        outputBuffer.setModifiers(modFlags);
        mainView->repaint();
    }
    else if (mainConfig.getImmediateMode())
    {
        Output::Sending::sendKey(enteredChar, outputBuffer.getModifierFlags(),
                targetWindow);
    }
    else
    {
        outputBuffer.appendCharacter(enteredChar);
    }
    mainView->updateChordState(&charsetConfig.getActiveSet(), 0,
            getInputPreview());
}


// Handles key commands used to send buffered text, delete buffered text, or
// close the application.
void Input::Controller::keyPressed(const juce::KeyPress key)
{
    const juce::ScopedTryLock inputLock(inputGuard);
    if (!inputLock.isLocked())
    {
        DBG(dbgPrefix << __func__ 
                << ": Still busy doing something else, ignoring input.");
        return;
    }
    if (mainView->isHelpScreenShowing())
    {
        // When the help screen is open, all input events just close the help
        // screen.
        closeHelpScreen();
        return;
    }
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
            &Keys::selectModSet,
            [this, &sendUpdate, &charSetUpdate]()
            {
                sendUpdate = charSetUpdate(Type::modifier);
            } 
        },
        {
            &Keys::toggleShift,
            [this, &sendUpdate]()
            {
                charsetConfig.setShifted(! charsetConfig.getShifted());
                mainView->repaint();
            } 
        },
        {
            &Keys::backspace,
            [this, &sendUpdate]() 
            {
                // In immediate mode, actually send a backspace character
                if (mainConfig.getImmediateMode())
                {
                    Output::Sending::sendKey(Text::Values::backspace, 0,
                            targetWindow);
                }
                else
                {
                    outputBuffer.deleteLastChar();
                }
                sendUpdate = true;
            }
        },
        {
            &Keys::clearAll,
            [this, &sendUpdate]() 
            {
                outputBuffer.clear();
                sendUpdate = true;
            } 
        },
        {
            &Keys::sendText,
            [this, &sendUpdate]() 
            { 
                // In immediate mode, send a return character instead
                if (mainConfig.getImmediateMode())
                {
                    Output::Sending::sendKey(Text::Values::enter, 0,
                            targetWindow);
                }
                else
                {
                    Output::Sending::sendBufferedOutput(outputBuffer,
                            targetWindow);
                }
                sendUpdate = true;
            }
        },
        { 
            &Keys::closeAndSend,
            [this, &sendUpdate]() 
            { 
                Output::Sending::sendBufferedOutput(outputBuffer, targetWindow);
                juce::JUCEApplication::getInstance()->systemRequestedQuit();
            }
        },
        { 
            &Keys::close,
            [this, &sendUpdate]() 
            {
                juce::JUCEApplication::getInstance()->systemRequestedQuit();
            } 
        },
        {
            &Keys::toggleImmediate,
            [this, &sendUpdate]() 
            {
                const bool immediateMode = ! mainConfig.getImmediateMode();
                mainConfig.setImmediateMode(immediateMode);
                if (immediateMode && ! outputBuffer.isEmpty())
                {
                    Output::Sending::sendBufferedOutput(outputBuffer,
                            targetWindow);
                }
                sendUpdate = true;
            } 
        },
        {
            &Keys::showHelp,
            [this, &sendUpdate]()
            {
                mainView->toggleHelpScreen();
                DBG(dbgPrefix << __func__ << ": Showing help screen.");
                Application* application = Application::getInstance();
                application->resetWindow(application->getWindowFlags()
                        | (int) Application::WindowFlag::showingHelp);
            } 
        },
        {
            &Keys::toggleWindowEdge,
            [this]() 
            { 
                mainConfig.setSnapToBottom(! mainConfig.getSnapToBottom());
                DBG(dbgPrefix << __func__ << ": Setting windowEdge = "
                        << (mainConfig.getSnapToBottom() ? "bottom" : "top"));
                Application* application = Application::getInstance();
                application->resetUpdatingFlags();
            } 
        },
        {
            &Keys::toggleMinimize,
            [this, &sendUpdate]()
            {
                mainConfig.setMinimised(! mainConfig.getMinimized());
                DBG(dbgPrefix << __func__ << ": Setting minimized = "
                        << (mainConfig.getMinimized() ? "true" : "false"));
                Application* application = Application::getInstance();
                application->resetUpdatingFlags();
            } 
        },
    };
    juce::KeyPress unmoddedKey(key.getKeyCode());
    for (const juce::Identifier* binding : Keys::allKeys)
    {
        juce::KeyPress boundKey = keyConfig.getBoundKey(*binding);
        if (boundKey == key || boundKey == unmoddedKey)
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
                getInputPreview());
    }
}


// Ensures the help screen is currently closed.
void Input::Controller::closeHelpScreen()
{
    jassert(mainView->isHelpScreenShowing());
    DBG(dbgPrefix << __func__ << ": Closing help screen.");
    mainView->toggleHelpScreen();
    Application* application = Application::getInstance();
    jassert(application != nullptr);
    application->resetWindow(application->getWindowFlags() 
            & ~ (int) Application::WindowFlag::showingHelp);
}
