#pragma once
/**
 * @file  Page_StackComponent.h
 *
 * @brief  Manages all open page components within the application.
 *
 *  PageStackComponent implements PageComponent::PageStackInterface to manage
 * the stack of open pages. It takes ownership of new pages, and allows them to
 * add new pages above themselves or remove and delete themselves from the top
 * of the stack.
 */

#include "Page_Interface_Stack.h"
#include "Page_Interface_Component.h"
#include "JuceHeader.h"

namespace Page
{
    class Component;
    class StackComponent;
}

/**
 * @brief  Creates, shows, removes, and destroys Page::Component objects.
 *
 *  StackComponent implements Page::Interface::Stack to manage the stack of
 * open page components. It takes ownership of new pages, and allows them to
 * add new pages above themselves or remove and delete themselves from the top
 * of the stack.
 */
class Page::StackComponent : public Page::Interface::Stack,
    public juce::Component
{
public:
    /**
     * @brief  Sets the juce::Component properties of the StackComponent.
     */
    StackComponent();

    virtual ~StackComponent() { }

    /**
     * @brief  Sets the root page on the page stack.
     *
     * @param page  The new page to add to the base of the stack.
     */
    void setRootPage(Interface::Component* page);

private:
    /**
     * @brief  Pushes a new Component on top of the stack, optionally animating
     *         the transition.
     *
     * @param page        The new page to display.
     *
     * @param transition  The animation type to use for the page transition.
     */
    virtual void pushPage(Interface::Component* page,
            const Layout::Transition::Type transition
            = Layout::Transition::Type::moveLeft) override;

    /**
     * @brief  Removes the top page from the stack, optionally animating the
     *         transition. This will not remove the last page from the stack.
     *
     * @param transition  The animation type to use for the page transition.
     */
    virtual void popPage(const Layout::Transition::Type transition
                = Layout::Transition::Type::moveRight) override;

    /**
     * @brief  Checks if a page is the top page on the stack.
     *
     * @param page  The page to check for on the stack.
     *
     * @return      Whether the page is displayed on top of the page stack.
     */
    virtual bool isTopPage(const Interface::Component* page) override;

private:
    /**
     * @brief  Sets all page component bounds to match the page stack component.
     */
    void resized() override;

    /**
     * @brief  Animates a page as it is added to or removed from the stack.
     *
     *  If the page is being added, this will add it to the StackComponent,
     * make it visible, and set its initial bounds. The page will be disabled
     * while animating, and re-enabled once animation finishes.
     *
     * @param page           The page being added or removed.
     *
     * @param transition     The type of transition animation to show.
     *
     * @param duration       The animation duration in milliseconds.
     *
     * @param postAnimation  A callback function to run when animation finishes.
     *
     * @param addingPage     True if the page is being added, false if it's
     *                       being removed.
     */
    void transitionPage(Interface::Component* page,
            const Layout::Transition::Type transition,
            const int duration,
            const std::function<void(Interface::Component*) > postAnimation
                    = [](Interface::Component* page) { },
            const bool addingPage = true);


    // Holds all pages in order:
    juce::OwnedArray<Interface::Component> stack;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StackComponent)
};
