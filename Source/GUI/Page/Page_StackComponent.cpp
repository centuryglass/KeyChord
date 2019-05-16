#include "Page_StackComponent.h"
#include "Layout_Transition_Animator.h"
#include "Util_TempTimer.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Page::StackComponent::";
#endif

// Page transition animation duration in milliseconds:
static const constexpr int transitionDurationMS = 300;

// Sets the juce::Component properties of the StackComponent.
Page::StackComponent::StackComponent()
{
#if JUCE_DEBUG
    setName("Page::StackComponent");
#endif
    setInterceptsMouseClicks(false, true);
    setWantsKeyboardFocus(false);
}


// Sets the root page on the page stack.
void Page::StackComponent::setRootPage(Interface::Component* page)
{
    if (page != nullptr && stack.isEmpty())
    {
        pushPage(page, Layout::Transition::Type::none);
    }
}


// Pushes a new PageComponent on top of the stack, optionally animating the
// transition.
void Page::StackComponent::pushPage(Interface::Component* page,
        const Layout::Transition::Type transition)
{
    DBG(dbgPrefix << __func__ << ": Pushing page " << page->getName());
    stack.add(page);
    page->setStackInterface(this);
    page->pageAddedToStack();
    transitionPage(page, transition, transitionDurationMS);
}


// Removes the top page from the stack, optionally animating the transition.
// This will not remove the last page from the stack.
void Page::StackComponent::popPage(const Layout::Transition::Type transition)
{
    if (stack.size() > 1)
    {
        Interface::Component* page = stack.getLast();
        transitionPage(page, transition, transitionDurationMS,
        [this](Interface::Component * page)
        {
            removeChildComponent(page);
            stack.removeObject(page);
            stack.getLast()->pageRevealedOnStack();
        }, false);
    }
}


// Checks if a page is the top page on the stack.
bool Page::StackComponent::isTopPage(const Interface::Component* page)
{
    return page == stack.getLast();
}


// Sets all page component bounds to match the page stack component.
void Page::StackComponent::resized()
{
    for (Interface::Component* page : stack)
    {
        page->setBounds(getBounds());
    }
}


// Animates a page as it is added to or removed from the stack.
void Page::StackComponent::transitionPage(Interface::Component* page,
        const Layout::Transition::Type transition,
        const int duration,
        const std::function<void(Interface::Component*) > postAnimation,
        const bool addingPage)
{
    if (addingPage)
    {
        addAndMakeVisible(page);
    }
    page->setEnabled(false);
    if (addingPage)
    {
        Layout::Transition::Animator::transitionIn(page, transition,
                getLocalBounds(), duration);
    }
    else
    {
        Layout::Transition::Animator::transitionOut(page, transition, duration,
                true);
    }
    Util::TempTimer::initTimer(duration * 1.2, [this, postAnimation, page]()
    {
        page->setEnabled(true);
        postAnimation(page);
    });
}
