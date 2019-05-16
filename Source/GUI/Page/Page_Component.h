#pragma once
/**
 * @file  Page_Component.h
 *
 * @brief  Organizes and manages one screen of the program UI.
 */

#include "Page_Interface_Component.h"
#include "Page_Interface_FactoryHolder.h"
#include "Layout_Group_Manager.h"
#include "Layout_Group_RelativeLayout.h"
#include "Layout_Transition_Type.h"
#include "Widgets_NavButton.h"
#include "JuceHeader.h"

namespace Page
{
    class Component;
    enum class Type;
}

/**
 * @brief  A container component that organizes and manages one screen of the
 *         program UI.
 *
 *  Page::Component objects fill the entire program window, so that only one
 * can be visible at a time.
 *
 *  Active pages are held in a stack, with the topmost page visible. The top
 * page on the stack may add another page to the stack above it. It may also
 * remove itself to reveal the page beneath it, as long as it's not the only
 * page on the stack. The stack takes ownership of all pages it holds, deleting
 * them when they're removed from the stack. The page stack is managed by an
 * object implementing Page::StackInterface that is linked to the
 * Page::Component after the page is added to the stack.
 *
 *  All pages are aware of all other page types that they can create through
 * the Page::Type enum class. To be able to create pages, a page must have a
 * pointer to a Page::FactoryInterface object assigned to it on construction.
 * If it has a Page::FactoryInterface, the page on the top of the page stack
 * can create any other page type to add to the stack, without needing to know
 * anything about the created page.
 */
class Page::Component : public Interface::Component,
    public Interface::FactoryHolder,
    public juce::Button::Listener
{
public:
    /**
     * @brief  juce::Component ColourId values, used by juce::LookAndFeel.
     */
    enum ColourIds
    {
        backgroundColourId = 0x1900500
    };

    Component() { }

    virtual ~Component() { }

    /**
     * @brief  Sets the layout of the page component.
     *
     *  If the page layout was set previously, the old layout will be cleared,
     * and its components will be removed from the page.
     *
     * @param layout  Defines the layout of all child components within the
     *                page, not including the back button. These components
     *                will be added to the page as child components and made
     *                visible. If necessary, the layout margins will be resized
     *                to make room for the back button.
     */
    void setLayout(const Layout::Group::RelativeLayout layout);

    /**
     * @brief  Options available for showing a back button on the page.
     */
    enum class BackButtonType
    {
        left,
        right,
        none
    };

    /**
     * @brief  Sets the type of back button(if any) that should be shown on the
     *         page.
     *
     *  If necessary, page layout margins will be resized to make room for the
     * back button.
     *
     * @param buttonType  The type of back button to show on the page.
     */
    void setBackButton(const BackButtonType buttonType);

    /**
     * @brief  Sets a background image to draw behind all page child components.
     *
     * @param bgImage   The image to draw, or a null Image(). If a null image
     *                  is used, the background will be filled with the
     *                  background colour instead of an image.
     */
    void setBackgroundImage(const juce::Image bgImage);

    /**
     * @brief  Adds all components in the layout to the page and makes them
     *         visible.
     */
    void addAndShowLayoutComponents();

protected:
    /**
     * @brief  Handles any actions necessary whenever the page is resized,
     *         besides updating the layout manager and back button.
     *
     *  Inheriting classes should override this instead of resized() to handle
     * page resize events.
     */
    virtual void pageResized() { }

    /**
     * @brief  Checks if this page is on top of a page stack.
     *
     * @return  Whether the page is currently on the top of a page stack.
     */
    bool isStackTop();

    /**
     * @brief  Removes this component from the page stack and destroys it, as
     *         long as it is the top page on the stack.
     *
     * @param transition  The transition animation to run if the page was
     *                    removed successfully.
     */
    void removeFromStack(Layout::Transition::Type transition
                = Layout::Transition::Type::moveLeft);

    /**
     * @brief  Creates a new page and pushes it on top of the page stack.
     *
     * @param pageType    The type of page to create and add.

     * @param transition  The transition animation to run if the page is
     *                    successfully added to the stack.
     */
    void pushPageToStack(Page::Type pageType,
            Layout::Transition::Type transition
                = Layout::Transition::Type::moveLeft);

private:
    /**
     * @brief  Potentially overrides the usual behavior of the page's back
     *         button.
     *
     *  Inheriting classes can override this function to change the behavior of
     * the back button. It will be called every time the back button is clicked.
     * By default, the back button removes the page from the page stack and
     * destroys it. If this function returns true, that default action will be
     * skipped.
     *
     * @return  True if the back button's action was replaced, false to allow
     *          the back button to remove the page as usual.
     */
    virtual bool overrideBackButton();


    /**
     * @brief  Updates component layout and back button bounds when the page is
     *         resized.
     */
    virtual void resized() final override;

    /**
     * @brief  Closes the page when the back button is clicked.
     *
     * @param button  The back button.
     */
    void buttonClicked(juce::Button* button) final override;

    /**
     * @brief  Fills the page background with an image or colour.
     *
     * @param graphics  The graphics context used to draw the page.
     */
    virtual void paint(juce::Graphics& graphics) override;

    // A button that removes the page from the page stack may be held here:
    std::unique_ptr<Widgets::NavButton> backButton = nullptr;

    // Manages the relative layout of page child components:
    Layout::Group::Manager layoutManager;

    // Optional page background image:
    juce::Image backgroundImage;
};
