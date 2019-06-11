#pragma once
/**
 * @file  Application.h
 *
 * @brief  Initializes and shuts down the JUCE application, and manages the
 *         application window.
 */

#include "JuceHeader.h"
#include "Theme_LookAndFeel.h"
#include "Component_MainView.h"
#include "Input_Controller.h"
#include "Input_Buffer.h"
#include "Theme_Colour_ConfigFile.h"
#include "Input_Key_ConfigFile.h"
#include "Text_CharSet_ConfigFile.h"
#include "Config_MainFile.h"


/**
 * @brief  Controls how the JUCE library initializes and shuts down the
 *         application.
 */
class Application : public juce::JUCEApplication
{
public:
    Application() { }

    virtual ~Application() { }

    /**
     * @brief  Gets the active application instance.
     *
     * @return  The single Application object that is created when the program
     *          starts.
     */
    static Application* getInstance();

    enum class WindowFlag
    {
        // Window is temporarily fullscreen to show help text:
        showingHelp = 0b001,
        // Chord previews are hidden so the window uses less space:
        minimized = 0b010,
        // Window should snap to the bottom edge of the display instead of the
        // top:
        snapToBottom = 0b100
    };

    /**
     * @brief  Gets the set of window flags currently applied to the application
     *         window.
     *
     * @return  The combination of window flags that are currently applied.
     */
    int getWindowFlags();

    /**
     * @brief  Recreates the MainWindow with bounds to suit the current
     *         circumstances.
     *
     * @param windowFlags  Any combination of values defined in the 
     *                     Application::WindowFlag enum. When omitted, the
     *                     window is returned to its default placement, filling
     *                     the bottom half of the display.
     */
    void resetWindow(const int windowFlags = 0);

    /**
     * @brief  Recreates the MainWindow, updating window flags to match the
     *         current selected configuration.
     */
    void resetUpdatingFlags();

    /**
     * @brief  Closes and recreates the application window.
     */
    void resetWindow();

private:
    /**
     * @brief  Performs all required initialization when the application is
     *         first launched.
     *
     *  This will be called by the JUCE library immediately when the application
     * starts.
     *
     * @param commandLine   The full command used to launch the application.
     */
    void initialise(const juce::String &commandLine) override;

    /**
     * @brief  Performs all necessary cleanup steps before the application can
     *         be safely closed.
     *
     *  This will be called by the JUCE framework shortly before the application
     * exits.
     */
    void shutdown() override;

    /**
     * @brief  Gets the name of this application.
     *
     * @return  The name the JUCE library uses to identify this application.
     */
    const juce::String getApplicationName() override;

    /**
     * @brief  Gets the application's version string.
     *
     * @return  The application version string tracked by the JUCE Library.
     */
    const juce::String getApplicationVersion() override;

    /**
     * @brief  Checks if multiple versions of this application may run
     *         simultaneously.
     *
     * @return  True, as a new instance of the application may need to replace
     *          this one if it loses focus and can't be refocused.
     */
    bool moreThanOneInstanceAllowed() override;

    /**
     * @brief  Kills this instance of the application when a new instance
     *         launches to replace it.
     *
     * @param commandLine  The new instance's command line arguments.
     */
    void anotherInstanceStarted(const juce::String& commandLine) override;

    #ifdef INCLUDE_TESTING
    /**
     * @brief  Runs application tests and shuts down the application.
     *
     *  The application window should be created and focused before this
     * function is called.
     */
    void runApplicationTests();
    #endif

    // Configuration files should remain loaded as long as the application still
    // exists:
    
    // Loads saved application state:
    Config::MainFile mainConfig;
    // Loads UI colour settings:
    Theme::Colour::ConfigFile colourConfig;
    // Loads custom character sets:
    Text::CharSet::ConfigFile charSetConfig;
    // Loads custom keybindings:
    Input::Key::ConfigFile inputConfig;

    // Stores buffered keyboard input:
    Input::Buffer inputBuffer;

    // These resources are dynamically allocated because they should be created
    // in the order listed here, and destroyed in the opposite order.

    // The program appearance manager:
    std::unique_ptr<Theme::LookAndFeel> lookAndFeel = nullptr;

    // The single application window:
    std::unique_ptr<juce::DocumentWindow> homeWindow = nullptr;

    // The main content component:
    std::unique_ptr<Component::MainView> mainView = nullptr;

    // The user input handler:
    std::unique_ptr<Input::Controller> inputController = nullptr;
};
