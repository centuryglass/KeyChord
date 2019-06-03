#pragma once
#include "Theme_Colour_Element.h"

/**
 * @file  Theme_Colour_ColourIds.h
 *
 * @brief  Provides a centralized location for accessing all project ColourId
 *         values.
 *
 *  The JUCE library assigns a set of unique ColourId values to each Component
 * class. These values are stored and loaded by the Theme::Colour::JSONResource,
 * and set by the Theme::LookAndFeel class. This allows the application's colour
 * theme to be centrally managed and easily configured.
 *
 *  In order to more easily manage ColourId values, the ColourIds namespace
 * lists all ColourIds relevant to this application. Each Component's IDs are
 * grouped into a separate inner namespace, and each ID is assigned a generic
 * Colour category used to determine its default value.
 */

namespace Theme { namespace Colour { namespace ColourIds {

namespace TextButton
{
    static const Element buttonOff(
            0x1000100, UICategory::widgetBackground);
    static const Element button(
            0x1000101, UICategory::widgetOn);
    static const Element textOff(
            0x1000102, UICategory::text);
    static const Element text(
            0x1000103, UICategory::text);
}
namespace TextEditor
{
    static const Element background(
            0x1000200, UICategory::textField);
    static const Element text(
            0x1000201, UICategory::text);
    static const Element highlight(
            0x1000202, UICategory::highlightedTextField);
    static const Element highlightedText(
            0x1000203, UICategory::highlightedText);
    static const Element outline(
            0x1000205, UICategory::outline);
    static const Element focusedOutline(
            0x1000206, UICategory::focusedOutline);
    static const Element shadow(
            0x1000207, UICategory::widgetOff);
}
namespace Caret
{
    static const Element caret(
            0x1000204, UICategory::text);
}
namespace Label
{
    static const Element background(
            0x1000280, UICategory::windowBackground);
    static const Element text(
            0x1000281, UICategory::text);
    static const Element outline(
            0x1000282, UICategory::outline);
    static const Element backgroundWhenEditing(
            0x1000283, UICategory::textField);
    static const Element textWhenEditing(
            0x1000284, UICategory::text);
    static const Element outlineWhenEditing(
            0x1000285, UICategory::focusedOutline);
}
namespace ScrollBar
{
    static const Element background(
            0x1000300, UICategory::widgetBackground);
    static const Element thumb(
            0x1000400, UICategory::widgetOn);
    static const Element track(
            0x1000401, UICategory::widgetOff);
}
namespace DirectoryContentsDisplay
{
    static const Element highlight(
            0x1000540, UICategory::highlightedTextField);
    static const Element text(
            0x1000541, UICategory::text);
    static const Element highlightedText(
            0x1000542, UICategory::highlightedText);
}
namespace PopupMenu
{
    static const Element text(
            0x1000600, UICategory::text);
    static const Element headerText(
            0x1000601, UICategory::text);
    static const Element background(
            0x1000700, UICategory::menuBackground);
    static const Element highlightedText(
            0x1000800, UICategory::highlightedText);
    static const Element highlightedBackground(
            0x1000900, UICategory::highlightedTextField);
}
namespace FileBrowser
{
    static const Element currentPathBoxBackground(
            0x1000640, UICategory::widgetBackground);
    static const Element currentPathBoxText(
            0x1000641, UICategory::text);
    static const Element currentPathBoxArrow(
            0x1000642, UICategory::text);
    static const Element filenameBoxBackground(
            0x1000643, UICategory::textField);
    static const Element filenameBoxText(
            0x1000644, UICategory::text);
}
namespace FileChooserDialogBox
{
    static const Element titleText(
            0x1000850, UICategory::text);
}
namespace ComboBox
{
    static const Element text(
            0x1000a00, UICategory::text);
    static const Element background(
            0x1000b00, UICategory::menuBackground);
    static const Element outline(
            0x1000c00, UICategory::outline);
    static const Element button(
            0x1000d00, UICategory::widgetOff);
    static const Element arrow(
            0x1000e00, UICategory::text);
    static const Element focusedOutline(
            0x1000f00, UICategory::focusedOutline);
}
namespace Slider
{
    static const Element background(
            0x1001200, UICategory::widgetBackground);
    static const Element thumb(
            0x1001300, UICategory::widgetOn);
    static const Element track(
            0x1001310, UICategory::widgetOff);
    static const Element textBoxText(
            0x1001400, UICategory::text);
    static const Element textBoxBackground(
            0x1001500, UICategory::windowBackground);
    static const Element textBoxHighlight(
            0x1001600, UICategory::highlightedText);
    static const Element textBoxOutline(
            0x1001700, UICategory::outline);
}
namespace AlertWindow
{
    static const Element background(
            0x1001800, UICategory::windowBackground);
    static const Element text(
            0x1001810, UICategory::text);
    static const Element outline(
            0x1001820, UICategory::focusedOutline);
}
namespace ProgressBar
{
    static const Element background(
            0x1001900, UICategory::widgetBackground);
    static const Element foreground(
            0x1001a00, UICategory::widgetOn);
}
namespace ListBox
{
    static const Element background(
            0x1002800, UICategory::menuBackground);
    static const Element outline(
            0x1002810, UICategory::outline);
    static const Element text(
            0x1002820, UICategory::text);
}
namespace DrawableButton
{
    static const Element text(
            0x1004010, UICategory::text);
    static const Element background(
            0x1004011, UICategory::widgetBackground);
    static const Element backgroundOn(
            0x1004012, UICategory::widgetBackground);
    static const Element textOn(
            0x1004013, UICategory::text);
}
namespace FileSearchPathList
{
    static const Element background(
            0x1004100, UICategory::menuBackground);
}
namespace ResizableWindow
{
    static const Element background(
            0x1005700, UICategory::windowBackground);
}
namespace DocumentWindow
{
    static const Element text(
            0x1005701, UICategory::text);
}
namespace ToggleButton
{
    static const Element text(
            0x1006501, UICategory::text);
    static const Element tick(
            0x1006502, UICategory::text);
    static const Element tickDisabled(
            0x1006503, UICategory::widgetOff);
}
namespace KeyMappingEditor
{
    static const Element background(
            0x100ad00, UICategory::windowBackground);
    static const Element text(
            0x100ad01, UICategory::text);
}
namespace Component_ChordPreview
{
    static const Element text(
            0x1900100, UICategory::text);
    static const Element inactiveText(
            0x1900101, UICategory::text);
    static const Element chord1Selected(
            0x1900102, UICategory::none);
    static const Element chord2Selected(
            0x1900103, UICategory::none);
    static const Element chord3Selected(
            0x1900104, UICategory::none);
    static const Element chord4Selected(
            0x1900105, UICategory::none);
    static const Element chord5Selected(
            0x1900106, UICategory::none);
    static const Element emptySelected(
            0x1900107, UICategory::none);
    static const Element chord1Active(
            0x1900108, UICategory::none);
    static const Element chord2Active(
            0x1900109, UICategory::none);
    static const Element chord3Active(
            0x190010a, UICategory::none);
    static const Element chord4Active(
            0x190010b, UICategory::none);
    static const Element chord5Active(
            0x190010c, UICategory::none);
    static const Element emptyActive(
            0x190010d, UICategory::none);
    static const Element chord1Open(
            0x190010e, UICategory::none);
    static const Element chord2Open(
            0x190010f, UICategory::none);
    static const Element chord3Open(
            0x1900110, UICategory::none);
    static const Element chord4Open(
            0x1900111, UICategory::none);
    static const Element chord5Open(
            0x1900112, UICategory::none);
    static const Element emptyOpen(
            0x1900113, UICategory::none);
    static const Element chord1Blocked(
            0x1900114, UICategory::none);
    static const Element chord2Blocked(
            0x1900115, UICategory::none);
    static const Element chord3Blocked(
            0x1900116, UICategory::none);
    static const Element chord4Blocked(
            0x1900117, UICategory::none);
    static const Element chord5Blocked(
            0x1900118, UICategory::none);
    static const Element emptyBlocked(
            0x1900119, UICategory::none);
}
namespace Component_InputView
{
    static const Element text(
            0x1900200, UICategory::text);
    static const Element background(
            0x1900201, UICategory::textField);
    static const Element outline(
            0x1900202, UICategory::outline);
    static const Element inputHighlight(
            0x1900203, UICategory::highlightedTextField);
}
namespace Component_ChordKeyDisplay
{
    static const Element chord1Selected(
            0x1900300, UICategory::none);
    static const Element chord2Selected(
            0x1900301, UICategory::none);
    static const Element chord3Selected(
            0x1900302, UICategory::none);
    static const Element chord4Selected(
            0x1900303, UICategory::none);
    static const Element chord5Selected(
            0x1900304, UICategory::none);
}

// The list of all Colour Element objects, sorted by ID:
const juce::Array<const Element*> allElements =
{
    &TextButton::buttonOff,
    &TextButton::button,
    &TextButton::textOff,
    &TextButton::text,
    &TextEditor::background,
    &TextEditor::text,
    &TextEditor::highlight,
    &TextEditor::highlightedText,
    &Caret::caret,
    &TextEditor::outline,
    &TextEditor::focusedOutline,
    &TextEditor::shadow,
    &Label::background,
    &Label::text,
    &Label::outline,
    &Label::backgroundWhenEditing,
    &Label::textWhenEditing,
    &Label::outlineWhenEditing,
    &ScrollBar::background,
    &ScrollBar::thumb,
    &ScrollBar::track,
    &DirectoryContentsDisplay::highlight,
    &DirectoryContentsDisplay::text,
    &DirectoryContentsDisplay::highlightedText,
    &PopupMenu::text,
    &PopupMenu::headerText,
    &FileBrowser::currentPathBoxBackground,
    &FileBrowser::currentPathBoxText,
    &FileBrowser::currentPathBoxArrow,
    &FileBrowser::filenameBoxBackground,
    &FileBrowser::filenameBoxText,
    &PopupMenu::background,
    &PopupMenu::highlightedText,
    &FileChooserDialogBox::titleText,
    &PopupMenu::highlightedBackground,
    &ComboBox::text,
    &ComboBox::background,
    &ComboBox::outline,
    &ComboBox::button,
    &ComboBox::arrow,
    &ComboBox::focusedOutline,
    &Slider::background,
    &Slider::thumb,
    &Slider::track,
    &Slider::textBoxText,
    &Slider::textBoxBackground,
    &Slider::textBoxHighlight,
    &Slider::textBoxOutline,
    &AlertWindow::background,
    &AlertWindow::text,
    &AlertWindow::outline,
    &ProgressBar::background,
    &ProgressBar::foreground,
    &ListBox::background,
    &ListBox::outline,
    &ListBox::text,
    &DrawableButton::text,
    &DrawableButton::background,
    &DrawableButton::backgroundOn,
    &DrawableButton::textOn,
    &FileSearchPathList::background,
    &ResizableWindow::background,
    &DocumentWindow::text,
    &ToggleButton::text,
    &ToggleButton::tick,
    &ToggleButton::tickDisabled,
    &KeyMappingEditor::background,
    &KeyMappingEditor::text,
    &Component_ChordPreview::text,
    &Component_ChordPreview::inactiveText,
    &Component_ChordPreview::chord1Selected,
    &Component_ChordPreview::chord2Selected,
    &Component_ChordPreview::chord3Selected,
    &Component_ChordPreview::chord4Selected,
    &Component_ChordPreview::chord5Selected,
    &Component_ChordPreview::emptySelected,
    &Component_ChordPreview::chord1Active,
    &Component_ChordPreview::chord2Active,
    &Component_ChordPreview::chord3Active,
    &Component_ChordPreview::chord4Active,
    &Component_ChordPreview::chord5Active,
    &Component_ChordPreview::emptyActive,
    &Component_ChordPreview::chord1Open,
    &Component_ChordPreview::chord2Open,
    &Component_ChordPreview::chord3Open,
    &Component_ChordPreview::chord4Open,
    &Component_ChordPreview::chord5Open,
    &Component_ChordPreview::emptyOpen,
    &Component_ChordPreview::chord1Blocked,
    &Component_ChordPreview::chord2Blocked,
    &Component_ChordPreview::chord3Blocked,
    &Component_ChordPreview::chord4Blocked,
    &Component_ChordPreview::chord5Blocked,
    &Component_ChordPreview::emptyBlocked,
    &Component_InputView::text,
    &Component_InputView::background,
    &Component_InputView::outline,
    &Component_InputView::inputHighlight,
    &Component_ChordKeyDisplay::chord1Selected,
    &Component_ChordKeyDisplay::chord2Selected,
    &Component_ChordKeyDisplay::chord3Selected,
    &Component_ChordKeyDisplay::chord4Selected,
    &Component_ChordKeyDisplay::chord5Selected
};

} } }