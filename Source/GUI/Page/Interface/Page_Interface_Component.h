#pragma once
/**
 * @file  Page_Interface_Component.h
 *
 * @brief  Allows the Page::StackComponent to notify Page::Component objects
 *         when it interacts with them.
 */

#include "JuceHeader.h"

namespace Page
{
    namespace Interface
    {
        class Component;
        class Stack;
    }
    class StackComponent;
}

/**
 * @brief  The private interface used by the StackComponent to control the
 *         Page::Component.
 */
class Page::Interface::Component : public juce::Component
{
public:
    Component() { }

    virtual ~Component() { }

protected:
    /**
     * @brief  Gets the page component's interface to the page stack.
     *
     * @return   The stack interface pointer, or nullptr if the page is not on
     *           a stack.
     */
    Stack* getStackInterface();

private:
    friend StackComponent;

    /**
     * @brief  Sets the page component's interface to the page stack.
     *
     * @param stack  The stack interface pointer of the stack holding this page.
     */
    void setStackInterface(Stack* stack);

    /**
     * @brief  Whenever this page is added to a page stack, the StackComponent
     *         will call this function.
     */
    virtual void pageAddedToStack() { }

    /**
     * @brief  Whenever this page becomes the top page on the page stack, the
     *         StackComponent will call this function.
     */
    virtual void pageRevealedOnStack() { }

    Stack* stackInterface = nullptr;
};
