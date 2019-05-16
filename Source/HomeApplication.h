#pragma once
/**
 * @file  HomeApplication.h
 *
 * @brief  Initializes and shuts down the JUCE application.
 */

#include "JuceHeader.h"
#include "Theme_LookAndFeel.h"
#include "Layout_Component_ConfigFile.h"
#include "Theme_Colour_ConfigFile.h"
#include "Theme_Image_ConfigFile.h"
#include "Util_ConditionChecker.h"

/**
 * @brief  Controls how the JUCE library initializes and shuts down the
 *         application.
 */
class HomeApplication : public juce::JUCEApplication
{
public:
    HomeApplication() { }

    virtual ~HomeApplication() { }

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
     * @return  False, since there's no good reason to run multiple instances of
     *          this application, and any attempt to do so is probably
     *          accidental.
     */
    bool moreThanOneInstanceAllowed() override;

    /**
     * @brief  Attempts to activate the application window and grab keyboard
     *         focus.
     *
     * @return  Whether the main application window exists and is focused.
     */
    bool focusAppWindow();

    #ifdef INCLUDE_TESTING
    /**
     * @brief  Runs application tests and shuts down the application.
     *
     *  The application window should be created and focused before this
     * function is called.
     */
    void runApplicationTests();
    #endif

    // Waits for the main window to be focused before starting focus updates
    // and running tests(if applicable):
    Util::ConditionChecker focusChecker;

    // Application resource objects:
    // These objects remain allocated as long as one instance of them exists
    // somewhere. Declaring them here ensures that they will remain allocated
    // as long as the application is running.

    // Holds UI component layout data:
    Layout::Component::ConfigFile layoutConfig;

    // Holds UI colour settings:
    Theme::Colour::ConfigFile colourConfig;

    // These resources are dynamically allocated because they should be created
    // in the order listed here, and destroyed in the opposite order.

    // The program appearance manager:
    std::unique_ptr<Theme::LookAndFeel> lookAndFeel = nullptr;

    // The single program window:
    std::unique_ptr<juce::DocumentWindow> homeWindow = nullptr;
};
