#pragma once
/**
 * @file  HomeWindow.h
 *
 * @brief  Creates the main application window.
 */

#include "Windows_MainWindow.h"
#include "Page_Factory.h"
#include "Page_StackComponent.h"

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

private:
    /**
     * @brief  Closes the application normally when the window closes.
     */
    void closeButtonPressed() override;

    /**
     * @brief  Resizes page content to match the window size.
     */
    virtual void resized() override;

    // Creates all pages on the page stack:
    Page::Factory pageFactory;
    // Holds all page components besides the login page:
    Page::StackComponent pageStack;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomeWindow)
};
