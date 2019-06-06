#pragma once
/**
 * @file  MainWindow.h
 *
 * @brief  Creates the main application window.
 */

#include "JuceHeader.h"
#include "Config_MainFile.h"

/**
 * @brief  The sole application window object and the root component in the
 *         component display tree.
 *
 *  The open window object can be accessed statically using
 * MainWindow::getOpenWindow(). Use this when you need to check or adjust the
 * bounds of the window, or see if the window is currently open.
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

    /**
     * @brief  Gets a pointer to the current open window object.
     *
     * @return  The application window pointer, or nullptr if the window is not
     *          currently open.
     */
    static MainWindow* getOpenWindow();

    /**
     * @brief  Updates the window's height, while keeping the window snapped
     *         to the selected display edge.
     *
     * @param newHeight  Desired window height in pixels.
     */
    void setHeight(const int newHeight);

private:
    /**
     * @brief  Closes the application normally when the window closes.
     */
    void closeButtonPressed() override;

    /**
     * @brief  Resizes page content to match the window size.
     */
    virtual void resized() override;

    // Keeps the main configuration file loaded.
    Config::MainFile mainConfig;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
