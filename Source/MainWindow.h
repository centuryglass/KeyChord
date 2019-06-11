#pragma once
/**
 * @file  MainWindow.h
 *
 * @brief  Creates the main application window, and makes sure the application
 *         is closed if the window is blocked.
 */

#include "JuceHeader.h"

/**
 * @brief  The sole application window object and the root component in the
 *         component display tree.
 */
class MainWindow : public juce::DocumentWindow
{
public:
    /**
     * @brief  Creates and shows the main application window.
     *
     * @param windowName  Sets the text of the window title bar.
     */
    MainWindow(juce::String windowName);

    virtual ~MainWindow() { }

private:
    /**
     * @brief  Closes the application normally when the window closes.
     */
    void closeButtonPressed() override;

    /**
     * @brief  Resizes page content to match the window size.
     */
    virtual void resized() override;

    /**
     * @brief  Starts the exit timer when the window loses focus, and stops
     *         if if the window gains focus again.
     */
    void activeWindowStatusChanged() override;

    /**
     * @brief  Shuts down the application if the window loses focus and doesn't
     *         regain it.
     */
    class ExitTimer : public juce::Timer
    {
    private:
        virtual void timerCallback() override;
    };
    ExitTimer exitTimer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
