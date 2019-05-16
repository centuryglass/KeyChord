#include "Widgets_FilePathEditor.h"
#include "Layout_Component_ConfigFile.h"
#include "Windows_Info.h"

// Default file filter, allowing image types:
const juce::WildcardFileFilter Widgets::FilePathEditor::imageFilter
("*.png;*.jpg;*.jpeg;*.svg;*.gif;*.xpm", "", "Image files");

// Class localized text key:
static const juce::Identifier localeClassKey = "Widgets::FilePathEditor";

// Localized text value keys:
static const juce::Identifier defaultTitleKey       = "defaultTitle";
static const juce::Identifier defaultDescriptionKey = "defaultDescription";

// Creates a new FilePathEditor, optionally setting its initial properties.
Widgets::FilePathEditor::FilePathEditor(
        const juce::String& selectionTitle,
        const juce::String& selectionText,
        const juce::WildcardFileFilter fileFilter) :
Locale::TextUser(localeClassKey),
fileFilter(fileFilter),
selectionTitle(selectionTitle),
selectionText(selectionText),
fileSelectButton("...")
{
#if JUCE_DEBUG
    setName("Widgets::FilePathEditor");
#endif
    if (selectionTitle.isEmpty())
    {
        this->selectionTitle = localeText(defaultTitleKey);
    }
    if (selectionText.isEmpty())
    {
        this->selectionText = localeText(defaultDescriptionKey);
    }
    fileSelectButton.addListener(this);
    filePath.addListener(this);
    Layout::Component::ConfigFile config;
    filePath.setFont(juce::Font(config.getFontHeight
            (Layout::Component::TextSize::smallText)));
    using juce::Justification;
    const Justification pathJustification(Justification::bottom
            | Justification::left);
    filePath.setJustification(pathJustification);
    addAndMakeVisible(filePath);
    addAndMakeVisible(fileSelectButton);
}


// Sets the text value held by this editor.
void Widgets::FilePathEditor::setText
(const juce::String newText, const bool shouldNotify)
{
    filePath.setText(newText, shouldNotify);
    if (shouldNotify)
    {
        notifyListeners();
    }
}


// Gets the text value held by this editor.
juce::String Widgets::FilePathEditor::getText() const
{
    return filePath.getText();
}


// Sets the visibility of the editor's file selection button.
void Widgets::FilePathEditor::showFileSelectButton(const bool shouldShow)
{
    if (shouldShow == fileSelectButton.isShowing())
    {
        return;
    }

    if (shouldShow)
    {
        addAndMakeVisible(fileSelectButton);
    }
    else
    {
        removeChildComponent(&fileSelectButton);
        fileSelectButton.setVisible(false);
    }
    resized();
}


// Adds a Listener to the list of Listeners that will be notified of changes.
void Widgets::FilePathEditor::addFileSelectListener(Listener * listener)
{
    listeners.insert(listener);
}


// Signals to all listeners that editor data has updated.
void Widgets::FilePathEditor::notifyListeners()
{
    for (auto iter = listeners.begin(); iter != listeners.end(); iter++)
    {
        (*iter)->fileSelected(this);
    }
}


// Calls notifyListeners when the editor loses focus.
void Widgets::FilePathEditor::textEditorFocusLost(juce::TextEditor& editor)
{
    if (editor.getText().isNotEmpty())
    {
        notifyListeners();
    }
}


// Calls notifyListeners when the editor is focused and the return key is
// pressed.
void Widgets::FilePathEditor::textEditorReturnKeyPressed
(juce::TextEditor & editor)
{
    if (editor.getText().isNotEmpty())
    {
        notifyListeners();
    }
}


// Opens the file selection window when the file selection button is clicked.
void Widgets::FilePathEditor::buttonClicked(juce::Button* button)
{
    using juce::FileBrowserComponent;
    using juce::FileChooserDialogBox;
    FileBrowserComponent browser(FileBrowserComponent::canSelectFiles |
            FileBrowserComponent::openMode,
            juce::File(),
            &fileFilter,
            &imagePreview);
    FileChooserDialogBox dialogBox(
            selectionTitle,
            selectionText,
            browser,
            false,
            findColour(fileWindowColourId));
    dialogBox.setColour(FileChooserDialogBox::titleTextColourId,
            juce::Colours::red);
    jassert(dialogBox.isColourSpecified
            (FileChooserDialogBox::titleTextColourId));
    juce::Rectangle<int> size = Windows::Info::getBounds();
    if (dialogBox.show(size.getWidth(), size.getHeight()))
    {
        juce::File selectedFile = browser.getSelectedFile(0);
        setText(selectedFile.getFullPathName());
        notifyListeners();
    }
}


// Resizes child components to fit within the parent component.
void Widgets::FilePathEditor::resized()
{
    using juce::Rectangle;
    Rectangle<int> bounds = getLocalBounds();
    if (!fileSelectButton.isVisible())
    {
        filePath.setBounds(bounds);
        return;
    }
    Rectangle<int> buttonBounds = bounds;
    Rectangle<int> textBounds = bounds;

    buttonBounds.setLeft(bounds.getRight() - bounds.getHeight());
    textBounds.setRight(buttonBounds.getX());

    filePath.setBounds(textBounds);
    fileSelectButton.setBounds(buttonBounds);
    // Layout::Component::ConfigFile componentConfig;
    // int fontSize = componentConfig.getFontHeight
    // (Layout::Component::TextSize::smallText);
    // fontSize = std::min(fontSize, textBounds.getHeight());
    // filePath.applyFontToAllText(juce::Font(fontSize));
}
