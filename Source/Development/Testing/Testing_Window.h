#pragma once
/**
 * @file  Testing_Window.h
 *
 * @brief  Creates a basic window for testing a single component.
 */

#include "Windows_MainWindow.h"

namespace Testing { class Window; }

/**
 * @file  Testing_Window.h
 *
 * @brief  A simple window object that owns a single component.
 */
class Testing::Window : public Windows::MainWindow
{
public:
    /**
     * @brief  Creates and shows a window holding a test component.
     *
     * @param testName       The name of the tested component, to use as the
     *                       window title.
     *
     * @param testComponent  The component that the window will display.
     *
     * @param xPos           The initial x-coordinate position of the window.
     *
     * @param yPos           The initial y-coordinate position of the window.
     *
     * @param width          The initial width of the window.
     *
     * @param height         The initial height of the window.
     */
    Window(const juce::String testName,
            juce::Component* const testComponent,
            const int xPos,
            const int yPos,
            const int width,
            const int height);

    virtual ~Window() { }
};
