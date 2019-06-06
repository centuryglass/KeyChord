#include "Input_Controller.h"
#include "Input_Key_JSONKeys.h"
#include "Text_Values.h"
#include "MainWindow.h"
#include "JuceHeader.h"
#include "Application.h"
#include <map>

static const juce::Identifier localeKey("Input_Controller");
static const juce::Identifier immediateModeKey("immediateMode");

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Input::Controller::";
#endif


// Sets up all keyboard input handling.
Input::Controller::Controller
(Component::MainView* mainView, const int targetWindow) :
    Locale::TextUser(localeKey),
    chordReader(mainView),
    mainView(mainView),
    inputBuffer(targetWindow)
{
    chordReader.addListener(this);
    const bool immediateMode = mainConfig.getImmediateMode();
    Text::CharString cachedInput = mainConfig.takeCachedBuffer();
    for (const Text::CharValue& charValue : cachedInput)
    {
        inputBuffer.appendCharacter(charValue);
        if (immediateMode)
        {
            inputBuffer.sendAndClearInput();
        }
    }
    mainView->updateChordState(&charsetConfig.getActiveSet(), 0,
            getInputPreview());
}


// Gets a CharString displaying appropriate input preview text.
Text::CharString Input::Controller::getInputPreview() const
{
    using juce::String;
    Text::CharString inputText = inputBuffer.getInputText();
    if (mainConfig.getImmediateMode())
    {
        String immediateModeText = localeText(immediateModeKey);
        for (int i = 0; i < immediateModeText.length(); i++)
        {
            inputText.add(Text::Values::getCharValue(
                    immediateModeText.substring(i, i + 1)));
        }
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
    if (mainView->isHelpScreenShowing())
    {
        // When the help screen is open, all input events just close the help
        // screen.
        mainView->toggleHelpScreen();
        const int height = juce::Desktop::getInstance().getDisplays()
            .getMainDisplay().userArea.getHeight() / 2;
        MainWindow::getOpenWindow()->setHeight(height);
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
        Text::ModTracker::ModKey modKey;
        switch (enteredChar)
        {
            case Text::Values::ctrl:
                modKey = Text::ModTracker::ModKey::control;
                break;
            case Text::Values::alt:
                modKey = Text::ModTracker::ModKey::alt;
                break;
            case Text::Values::shift:
                modKey = Text::ModTracker::ModKey::shift;
                break;
            case Text::Values::cmd:
                modKey = Text::ModTracker::ModKey::command;
                break;
            default:
                DBG(dbgPrefix << __func__ << ": Unexpected key value "
                        << juce::String(enteredChar) 
                        << " incorrectly evaluated as modifier.");
                jassertfalse;
        }
        modTracker.toggleKey(modKey);
        mainView->repaint();
    }
    else
    {
        inputBuffer.appendCharacter(enteredChar);
    }
    if (mainConfig.getImmediateMode())
    {
        inputBuffer.sendAndClearInput();
    }
    mainView->updateChordState(&charsetConfig.getActiveSet(), 0,
            getInputPreview());
}


// Handles key commands used to send buffered text, delete buffered text, or
// close the application.
void Input::Controller::keyPressed(const juce::KeyPress key)
{
    if (mainView->isHelpScreenShowing())
    {
        // When the help screen is open, all input events just close the help
        // screen.
        mainView->toggleHelpScreen();
        const int height = juce::Desktop::getInstance().getDisplays()
            .getMainDisplay().userArea.getHeight() / 2;
        MainWindow::getOpenWindow()->setHeight(height);
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
                juce::JUCEApplication::getInstance()->systemRequestedQuit();
            }
        },
        { 
            &Keys::close,
            [this, &sendUpdate]() 
            {
                inputBuffer.clearInput();
                juce::JUCEApplication::getInstance()->systemRequestedQuit();
            } 
        },
        {
            &Keys::toggleImmediate,
            [this, &sendUpdate]() 
            {
                const bool immediateMode = ! mainConfig.getImmediateMode();
                mainConfig.setImmediateMode(immediateMode);
                if (immediateMode && ! inputBuffer.isEmpty())
                {
                    inputBuffer.sendAndClearInput();
                }
                sendUpdate = true;
            } 
        },
        {
            &Keys::showHelp,
            [this, &sendUpdate]()
            {
                mainView->toggleHelpScreen();
                const int height = juce::Desktop::getInstance().getDisplays()
                    .getMainDisplay().userArea.getHeight();
                MainWindow::getOpenWindow()->setHeight(height);
            } 
        },
        {
            &Keys::toggleWindowEdge,
            [this]() 
            { 
                mainConfig.setSnapToBottom(! mainConfig.getSnapToBottom());
                restartApplication();
            } 
        },
        {
            &Keys::toggleMinimize,
            [this, &sendUpdate]()
            {
                mainConfig.setMinimised(! mainConfig.getMinimized());
                restartApplication();
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


// Restarts the application, preserving settings and input buffer contents
// across application instances. 
void Input::Controller::restartApplication()
{
    Config::MainFile mainConfig;
    mainConfig.cacheInputBuffer(inputBuffer.getRawInput());
    inputBuffer.clearInput();
    DBG(dbgPrefix << __func__ << ": Restarting application object");
    juce::MessageManager::callAsync([]()
    {
        static_cast<Application*>(juce::JUCEApplication::getInstance())
                ->restart();
    });
}
