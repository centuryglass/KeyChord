#pragma once
/**
 * @file  Widgets_PopupEditor.h
 *
 * @brief  A basis for temporary editor UI components.
 */

#include "Widgets_BoundedLabel.h"
#include "Widgets_DrawableImageButton.h"
#include "Layout_Component_Manager.h"
#include "Layout_Group_Manager.h"
#include "Theme_Image_Component.h"
#include "JuceHeader.h"

namespace Widgets { class PopupEditor; }

/**
 * @brief  A generic data editing overlay component.
 *
 *  PopupEditorComponent shows a small pop-up panel containing a title,
 * some type of content editing components, a cancel button, and a confirm
 * button. Both buttons will remove the component, and the confirm button will
 * also run a callback function to save all changes. Inheriting classes are
 * responsible for adding all other controls to the component layout.
 */
class Widgets::PopupEditor : public Theme::Image::Component<>,
public juce::Button::Listener
{
protected:
    /**
     * @brief  Sets on construction how the editor should save changes.
     *
     * @param onConfirm  The callback function to run when the confirm button
     *                   is pressed. When called, this editor passes in a
     *                   pointer to itself as the only argument.
     */
    PopupEditor(std::function<void(PopupEditor*) > onConfirm);

public:
    virtual ~PopupEditor() { }

    /**
     * @brief  Removes this component from the window.
     */
    void closePopup();

    /**
     * @brief  Applies the editor bounds defined in the layout configuration
     *         file.
     */
    void applyConfigBounds();

protected:
    /**
     * @brief  Sets the title printed across the top of the component.
     *
     * @param newTitle  The new title string to show.
     */
    void setEditorTitle(const juce::String newTitle);

    /**
     * @brief  Adds all Components in a layout as child components, and uses
     *         the layout to place the new child components below the title
     *         label and above the cancel and confirm buttons.
     *
     * @param layout  The layout of all child components that should be added
     *                to the editor. The title row will be added to the
     *                beginning, and the cancel/confirm row will be added to
     *                the end. Each of these rows will have a vertical weight
     *                of 1.
     */
    void setLayout(const Layout::Group::RelativeLayout layout);

    /**
     * @brief  Sets the amount of space in pixels to leave between all child
     *         components and the outer component bounds.
     *
     * @param margin  The new child component margin size.
     */
    void setMargin(const int margin);

    /**
     * @brief  Sets the amount of space in pixels to leave between child
     *         components.
     *
     * @param xPadding  The number of pixels to leave between child components
     *                  that are in the same row.
     *
     * @param yPadding  The number of pixels to leave between rows of child
     *                  components.
     */
    void setPadding(int xPadding, int yPadding);

private:
    /**
     * @brief  Handles button click events for any buttons other than the
     *         cancel and confirm buttons.
     *
     *  Subclasses should override this instead of buttonClicked to handle
     * button events.
     *
     * @param button  The button component that was clicked.
     */
    virtual void editorButtonClicked(juce::Button * button) { }

    /**
     * @brief  Runs whenever the Component object's bounds change.
     *
     *  Subclasses should override this function instead of resized to handle
     * resize events.
     */
    virtual void editorResized() { }

    /**
     * @brief  Manages click events from the cancel and confirm buttons,
     *         passing all other button events to the editorButtonClicked
     *         function.
     *
     * @param buttonClicked  The button that triggered the click event. If this
     *                       is the cancel or confirm button, the editor will
     *                       be closed. If it is the confirm button, the
     *                       onConfirm callback will also run to save editor
     *                       changes.
     */
    virtual void buttonClicked(juce::Button* buttonClicked) final override;

    /**
     * @brief  Handles key press events.
     *
     *  The escape and return keys work the same as pressing the cancel and
     * confirm buttons, respectively. When this component is open, it won't let
     * any button presses through to the components beneath it.
     *
     * @param key  A keyPress event that was not handled by the editor's child
     *             components.
     *
     * @return     True, to keep the JUCE library key handling system from
     *             passing the key event to another component.
     */
    virtual bool keyPressed(const juce::KeyPress& key) final override;

    /**
     * @brief  Reapplies the child component layout when the component bounds
     *         are updated.
     */
    virtual void resized() final override;

    /**
     * @brief  Grabs keyboard focus when the component becomes visible.
     */
    virtual void visibilityChanged() override;

    // Displays the editor title:
    Widgets::BoundedLabel titleLabel;

    // Closes the editor without saving changes:
    Theme::Image::Component<DrawableImageButton> cancelButton;

    // Closes the editor, saving all changes:
    Theme::Image::Component<DrawableImageButton> confirmButton;

    // Manages the editor's bounds:
    Layout::Component::Manager boundsManager;

    // Manages editor child component layout:
    Layout::Group::Manager layoutManager;

    // Saved layout margin/padding values:
    int marginPixels;
    int xPaddingPixels;
    int yPaddingPixels;

    // Callback function to run when confirm is pressed, passing "this" as
    // the sole parameter:
    std::function<void(PopupEditor*)> onConfirm;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PopupEditor)
};
