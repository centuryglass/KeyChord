#pragma once
/**
 * @file  Application.h
 *
 * @brief  Initializes and shuts down the JUCE application.
 */

#include "JuceHeader.h"
#include "Theme_LookAndFeel.h"
#include "Theme_Colour_ConfigFile.h"
#include "Component_MainView.h"
#include "Input_Controller.h"


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
     * @brief  Cleans up all application resources, then sets them up again.
     */
    void restart();
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

    // Holds UI colour settings:
    Theme::Colour::ConfigFile colourConfig;

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
