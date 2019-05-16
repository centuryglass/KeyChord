#include "Page_Component.h"
#include "Page_Type.h"
#include "Page_Interface_Stack.h"
#include "Page_Interface_Factory.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Page::Component::";
#endif

// Sets the layout of the page component.
void Page::Component::setLayout(Layout::Group::RelativeLayout layout)
{
    using Widgets::NavButton;
    layoutManager.clearLayout(true);
    // Make sure layout margins fit the back button:
    if (backButton != nullptr)
    {
        float minMargin
                = NavButton::xMarginFractionNeeded(backButton->getEdge());
        layout.setXMarginFraction(std::max(layout.getXMarginFraction(),
                minMargin));
    }
    layoutManager.setLayout(layout, this);
    if (!getBounds().isEmpty())
    {
        resized();
    }
}


// Sets the type of back button(if any) that should be shown on the page.
void Page::Component::setBackButton(const BackButtonType buttonType)
{
    using Widgets::NavButton;
    NavButton::WindowEdge newEdge = NavButton::WindowEdge::down;
    if (buttonType == BackButtonType::left)
    {
        newEdge = NavButton::WindowEdge::left;
    }
    else if (buttonType == BackButtonType::right)
    {
        newEdge = NavButton::WindowEdge::right;
    }
    if (backButton != nullptr)
    {
        if (newEdge == backButton->getEdge())
        {
            return;
        }
        removeChildComponent(backButton.get());
        backButton.reset(nullptr);
        if (buttonType == BackButtonType::none)
        {
            return;
        }

    }
    backButton.reset(new NavButton(newEdge));
    addAndMakeVisible(backButton.get());
    backButton->addListener(this);

    Layout::Group::RelativeLayout layout = layoutManager.getLayout();
    if (!layout.isEmpty() && layout.getXMarginFraction()
            < NavButton::xMarginFractionNeeded(backButton->getEdge()))
    {
        layout.setXMarginFraction(NavButton::xMarginFractionNeeded
                (backButton->getEdge()));
        layoutManager.setLayout(layout);
    }
}


// Sets a background image to draw behind all page child components.
void Page::Component::setBackgroundImage(const juce::Image bgImage)
{
    backgroundImage = bgImage;
}


// Adds all components in the layout to the page and makes them visible.
void Page::Component::addAndShowLayoutComponents()
{
    layoutManager.addComponentsToParent(this);
}


// Checks if this page is on top of a page stack.
bool Page::Component::isStackTop()
{
    Interface::Stack* stackInterface = getStackInterface();
    return stackInterface != nullptr && stackInterface->isTopPage(this);
}


// If this page is currently on top of a page stack, this will remove it from
// the stack and destroy it.
void Page::Component::removeFromStack(Layout::Transition::Type transition)
{
    if (isStackTop())
    {
        getStackInterface()->popPage(transition);
    }
}


// Creates and pushes a new page on top of the stack.
void Page::Component::pushPageToStack(const Type pageType,
        const Layout::Transition::Type transition)
{
    if (isStackTop() && getFactoryInterface() != nullptr)
    {
        DBG(dbgPrefix << __func__ << ": Page " << getName()
                << " pushing new page.");
        Component* newPage = static_cast<Component*>
                (getFactoryInterface()->createPage(pageType));
        getStackInterface()->pushPage(newPage, transition);
    }
}


// Potentially overrides the usual behavior of the page's back button.
bool Page::Component::overrideBackButton()
{
    return false;
}


// Updates component layout and back button bounds when the page is resized.
void Page::Component::resized()
{
    if (backButton != nullptr)
    {
        backButton->applyConfigBounds();
    }
    layoutManager.layoutComponents(getLocalBounds());
    pageResized();
}


// Closes the page when the back button is clicked.
void Page::Component::buttonClicked(juce::Button* button)
{
    if (button == backButton.get())
    {
        if (!overrideBackButton())
        {
            Layout::Transition::Type transition
                    = Layout::Transition::Type::moveRight;
            if (backButton->getEdge() == Widgets::NavButton::WindowEdge::right)
            {
                transition = Layout::Transition::Type::moveLeft;
            }
            removeFromStack(transition);
        }
    }
}


// Fills the page background with an image or colour.
void Page::Component::paint(juce::Graphics& graphics)
{
    if (backgroundImage.isValid())
    {
        graphics.drawImage(backgroundImage, getLocalBounds().toFloat());
    }
    else
    {
        graphics.fillAll(findColour(backgroundColourId));
    }
}
