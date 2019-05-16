#include "Widgets_PopupEditor.h"
#include "Layout_Component_JSONKeys.h"
#include "Theme_Image_JSONKeys.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Widgets::PopupEditor::";
#endif

// Default layout margin/padding values: */
static const constexpr int defaultMarginPixels = 6;
static const constexpr int defaultXPaddingPixels = 3;
static const constexpr int defaultYPaddingPixels = 4;

Widgets::PopupEditor::PopupEditor
(const std::function<void(PopupEditor*)> onConfirm) :
Theme::Image::Component<>(Theme::Image::JSONKeys::popupMenu, 0,
        juce::RectanglePlacement::stretchToFit),
boundsManager(this, Layout::Component::JSONKeys::popupMenu),
onConfirm(onConfirm),
titleLabel("EditorTitle", "", 2),
cancelButton(Theme::Image::JSONKeys::cancelButton),
confirmButton(Theme::Image::JSONKeys::confirmButton),
marginPixels(defaultMarginPixels),
xPaddingPixels(defaultXPaddingPixels),
yPaddingPixels(defaultYPaddingPixels)
{
    setWantsKeyboardFocus(true);
    #if JUCE_DEBUG
    setName(juce::String("Widgets::PopupEditor"));
    #endif
    titleLabel.setJustificationType(juce::Justification::centred);

    cancelButton.setWantsKeyboardFocus(false);
    cancelButton.addListener(this);

    confirmButton.setWantsKeyboardFocus(false);
    confirmButton.addListener(this);

    setInterceptsMouseClicks(true, true);
}


// Removes this component from the window.
void Widgets::PopupEditor::closePopup()
{
    setVisible(false);
    juce::Component* parent = getParentComponent();
    if (parent != nullptr)
    {
        parent->removeChildComponent(this);
    }
}


// Applies the editor bounds defined in the layout configuration file.
void Widgets::PopupEditor::applyConfigBounds()
{
    boundsManager.applyConfigBounds();
}


// Sets the title printed across the top of the component.
void Widgets::PopupEditor::setEditorTitle(const juce::String newTitle)
{
    titleLabel.setText(newTitle,
            juce::NotificationType::dontSendNotification);
}


// Adds all Components in a layout as child components.
void Widgets::PopupEditor::setLayout(Layout::Group::RelativeLayout layout)
{
    using namespace Layout::Group;
    layout.insertRow(0, Row(10, { RowItem(&titleLabel) }));
    layout.addRow(Row(10,
    {
        RowItem(&cancelButton, 10),
        RowItem(&confirmButton, 10)
    }));
    layoutManager.setLayout(layout, this);
    if (!getBounds().isEmpty())
    {
        resized();
    }
}


// Sets the amount of space in pixels to leave between all child components and
// the outer component bounds.
void Widgets::PopupEditor::setMargin(const int margin)
{
    marginPixels = margin;
}


// Sets the amount of space in pixels to leave between child components.
void Widgets::PopupEditor::setPadding(int xPadding, int yPadding)
{
    xPaddingPixels = xPadding;
    yPaddingPixels = yPadding;
}


// Manages click events from the cancel and confirm buttons, passing all other
// button events to editorButtonClicked function.
void Widgets::PopupEditor::buttonClicked(juce::Button* buttonClicked)
{
    if (buttonClicked == &cancelButton)
    {
        closePopup();
    }
    else if (buttonClicked == &confirmButton)
    {
        onConfirm(this);
        closePopup();
    }
    else
    {
        editorButtonClicked(buttonClicked);
    }
}


// Handles key press events.
bool Widgets::PopupEditor::keyPressed(const juce::KeyPress & key)
{
    if (key == juce::KeyPress::escapeKey)
    {
        closePopup();
    }
    else if (key == juce::KeyPress::returnKey)
    {
        onConfirm(this);
        closePopup();
    }
    return true;
}


// Reapplies the child component layout when the component bounds are updated.
void Widgets::PopupEditor::resized()
{
    DrawableImage::resized();
    layoutManager.layoutComponents(getLocalBounds());
    editorResized();
}


// Grabs keyboard focus when the component becomes visible.
void Widgets::PopupEditor::visibilityChanged()
{
    if (isShowing())
    {
        grabKeyboardFocus();
    }
}
