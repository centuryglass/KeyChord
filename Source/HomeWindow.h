#pragma once
/**
 * @file  HomeWindow.h
 *
 * @brief  Creates the main application window.
 */

#include "Windows_MainWindow.h"

/**
 * @brief  The sole application window object and the root component in the
 *         component display tree.
 */
class HomeWindow : public Windows::MainWindow
{
public:
    /**
     * @brief  Creates and shows the main application window.
     *
     * @param windowName  Sets the text of the window title bar.
     */
    HomeWindow(juce::String windowName);

    virtual ~HomeWindow() { }

    /**
     * @brief  Gets a pointer to the current open window object.
     *
     * @return  The application window pointer, or nullptr if the window is not
     *          currently open.
     */
    static HomeWindow* getOpenWindow();

    /**
     * @brief  Toggles window placement between the top and bottom of the
     *         display.
     */
    void toggleEdge();

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

    // Tracks whether the window should snap to the top or the bottom of the
    // display.
    bool bottomEdge = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomeWindow)
};
