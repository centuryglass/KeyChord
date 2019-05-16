#pragma once
/**
 * @file  Windows_MainWindow.h
 *
 * @brief  A juce::Window class used as a basis for primary application window
 *         classes.
 */

#include "JuceHeader.h"

namespace Windows
{
    class FocusInterface;
    class MainWindow;
}

/**
 * @brief  A basis for a main application window class that can ensure all
 *         FocusListener objects are updated when the window gains or loses
 *         focus.
 *
 *  Only one MainWindow should exist, and it should exist for as long as the
 * application is running. Once MainWindow::startFocusTracking() is called on
 * the MainWindow object, whenever it gains or loses window focus it will pass
 * the focus state update to the FocusTracker resource.
 */
class Windows::MainWindow : public juce::DocumentWindow
{
public:
    /**
     * @brief  Ensures the focus tracker is initialized and sets initial window
     *         properties.
     *
     * @param title              Sets the window's title as printed on the
     *                           title bar.
     *
     * @param backgroundColour   The background colour to draw behind window
     *                           components.
     *
     * @param requiredButtons    Sets which title bar buttons will be shown.
     */
    MainWindow(const juce::String& title, juce::Colour backgroundColour,
            int requiredButtons);

    /**
     * @brief  Closes the window's connection to the FocusTracker resource.
     */
    virtual ~MainWindow();

    /**
     * @brief  Ensures all future window focus changes will be sent to the
     *         FocusTracker resource.
     *
     *  Until this function is called, focus events will never be sent to the
     * FocusTracker resource.
     */
    void startFocusTracking();

private:
    /**
     * @brief  Ensures all listeners are notified when the window focus state
     *         changes after tracking is enabled.
     */
    void activeWindowStatusChanged() override;

    bool trackingEnabled = false;
};
