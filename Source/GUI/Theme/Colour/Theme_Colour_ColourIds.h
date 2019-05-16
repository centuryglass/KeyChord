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
namespace Image
{
    static const Element image0(
            0x1900000, UICategory::none);
    static const Element image1(
            0x1900001, UICategory::none);
    static const Element image2(
            0x1900002, UICategory::none);
    static const Element image3(
            0x1900003, UICategory::none);
    static const Element image4(
            0x1900004, UICategory::none);
}
namespace ListEditor
{
    static const Element background(
            0x1900100, UICategory::menuBackground);
    static const Element listItem(
            0x1900101, UICategory::textField);
    static const Element selectedListItem(
            0x1900102, UICategory::highlightedTextField);
    static const Element text(
            0x1900103, UICategory::text);
}
namespace AppMenuButton
{
    static const Element text(
            0x1900200, UICategory::text);
    static const Element selectedText(
            0x1900201, UICategory::text);
    static const Element textBackground(
            0x1900202, UICategory::widgetBackground);
    static const Element selectedTextBG(
            0x1900203, UICategory::widgetBackground);
    static const Element background(
            0x1900204, UICategory::widgetBackground);
    static const Element selectedBG(
            0x1900205, UICategory::widgetBackground);
    static const Element border(
            0x1900206, UICategory::outline);
    static const Element selectedBorder(
            0x1900207, UICategory::focusedOutline);
}
namespace FilePathEditor
{
    static const Element fileWindow(
            0x1900300, UICategory::windowBackground);
    static const Element text(
            0x1900301, UICategory::text);
}
namespace OverlaySpinner
{
    static const Element background(
            0x1900400, UICategory::widgetBackground);
    static const Element text(
            0x1900401, UICategory::text);
}
namespace LayoutContainer
{
    static const Element background(
            0x1900499, UICategory::windowBackground);
}
namespace Page
{
    static const Element background(
            0x1900500, UICategory::windowBackground);
}
namespace ColourPicker
{
    static const Element checkerboardLight(
            0x1900600, UICategory::widgetOn);
    static const Element checkerboardDark(
            0x1900601, UICategory::widgetOff);
    static const Element outline(
            0x1900602, UICategory::outline);
    static const Element focusedOutline(
            0x1900603, UICategory::focusedOutline);
}
namespace Switch
{
    static const Element background(
            0x1900700, UICategory::widgetBackground);
    static const Element handle(
            0x1900701, UICategory::widgetOn);
    static const Element handleOff(
            0x1900702, UICategory::widgetOff);
}
namespace HomePage
{
    static const Element text(
            0x1900800, UICategory::text);
}
namespace PowerPage
{
    static const Element background(
            0x1900900, UICategory::windowBackground);
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
    &Image::image0,
    &Image::image1,
    &Image::image2,
    &Image::image3,
    &Image::image4,
    &ListEditor::background,
    &ListEditor::listItem,
    &ListEditor::selectedListItem,
    &ListEditor::text,
    &AppMenuButton::text,
    &AppMenuButton::selectedText,
    &AppMenuButton::textBackground,
    &AppMenuButton::selectedTextBG,
    &AppMenuButton::background,
    &AppMenuButton::selectedBG,
    &AppMenuButton::border,
    &AppMenuButton::selectedBorder,
    &FilePathEditor::fileWindow,
    &FilePathEditor::text,
    &OverlaySpinner::background,
    &OverlaySpinner::text,
    &LayoutContainer::background,
    &Page::background,
    &ColourPicker::checkerboardLight,
    &ColourPicker::checkerboardDark,
    &ColourPicker::outline,
    &ColourPicker::focusedOutline,
    &Switch::background,
    &Switch::handle,
    &Switch::handleOff,
    &HomePage::text,
    &PowerPage::background
};

} } }
