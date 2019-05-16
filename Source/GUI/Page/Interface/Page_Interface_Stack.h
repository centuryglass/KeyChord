#pragma once
/**
 * @file  Page_Interface_Stack.h
 *
 * @brief  The interface used by Page::Component objects to interact with their
 *         Page::StackComponent.
 */

#include "Layout_Transition_Type.h"

namespace Page
{
    class Component;
    namespace Interface
    {
        class Stack;
        class Component;
    }
}

/**
 * @brief  An abstract interface implemented by the StackComponent, used by
 *         Page::Component objects to add and remove pages.
 */
class Page::Interface::Stack
{
public:
    Stack() { }

    virtual ~Stack() { }

protected:
    friend Page::Component;

    /**
     * @brief  Pushes a new Component on top of the stack, optionally animating
     *         the transition.
     *
     * @param page        The new top page to add.
     *
     * @param transition  The animation type to use for the page transition.
     */
    virtual void pushPage(Interface::Component* page,
            const Layout::Transition::Type transition
            = Layout::Transition::Type::moveLeft) = 0;

    /**
     * @brief  Removes the top page from the stack, optionally animating the
     *         transition.
     *
     * @param transition  The animation type to use for the page transition.
     */
    virtual void popPage(const Layout::Transition::Type transition
            = Layout::Transition::Type::moveRight) = 0;

    /**
     * @brief  Checks if a page is the top page on the stack.
     *
     * @param page  The page to check for on the stack.
     *
     * @return      Whether the page is displayed on top of the page stack.
     */
    virtual bool isTopPage(const Interface::Component* page) = 0;
};
