#pragma once
/**
 * @file  Widgets_FilePathEditor.h
 *
 * @brief  Provides a file selection UI component.
 */

#include "Locale_TextUser.h"
#include "Widgets_TextEditor.h"
#include "JuceHeader.h"
#include <set>

namespace Widgets { class FilePathEditor; }

/**
 * @brief  A file selection UI component.
 *
 *  This component allows a user to either type in a text value, or click a
 * button to open a file picker and choose a file/directory path. The component
 * selects only image files by default, but may be changed to select other file
 * types.
 */
class Widgets::FilePathEditor : public juce::Component,
    public Locale::TextUser,
    public juce::TextEditor::Listener,
    public juce::Button::Listener
{
public:
    // JUCE Component ColourId values:
    enum ColourIds
    {
        fileWindowColourId = 0x1900300,
        textColourId = 0x1900301
    };

    /**
     * @brief  Creates a new FilePathEditor, optionally setting its initial
     *         properties.
     *
     * @param selectionTitle  Sets the title displayed in the file selection
     *                        window. If left empty, this will use the default
     *                        image selection title.
     *
     * @param selectionText   Sets the short description displayed in the file
     *                        selection window. If left empty, this will use
     *                        the default image selection text.
     *
     * @param fileFilter      Sets what types of files may be selected. By
     *                        default, this selects image files.
     */
    FilePathEditor(
            const juce::String& selectionTitle = juce::String(),
            const juce::String& selectionText  = juce::String(),
            const juce::WildcardFileFilter fileFilter = imageFilter);

    virtual ~FilePathEditor() { }

    /**
     * @brief  Sets the text value held by this editor.
     *
     * @param newText       The new text value.
     *
     * @param shouldNotify  Whether Listener objects will be notified of this
     *                      change.
     */
    void setText(const juce::String newText, const bool shouldNotify = true);

    /**
     * @brief  Gets the text value held by this editor.
     *
     * @return  The text stored within this component's text editor box.
     */
    juce::String getText() const;

    /**
     * @brief  Sets the visibility of the editor's file selection button.
     *
     * @param shouldShow  Whether the file selection button next to the text
     *                    editor box should be visible.
     */
    void showFileSelectButton(const bool shouldShow);

    /**
     * @brief  Subscribes to receive updates when the editor's text changes.
     */
    class Listener
    {
    public:
        Listener() { }

        virtual ~Listener() { }

        /**
         * @brief  This will be called by tracked editors whenever a file is
         *         selected, the return key is pressed while editing, or focus
         *         is lost.
         *
         * @param edited  The updated editor notifying this listener.
         */
        virtual void fileSelected(FilePathEditor* edited) = 0;
    };

    /**
     * @brief  Adds a Listener to the list of Listeners that will be notified
     *         of changes.
     *
     * @param listener  An object that needs to track the contents of this
     *                  editor.
     */
    void addFileSelectListener(Listener* listener);

private:
    /**
     * @brief  Signals to all listeners that editor data has updated.
     */
    void notifyListeners();

    /**
     * @brief  Calls notifyListeners when the editor loses focus.
     *
     * @param editor  This component's internal TextEditor object.
     */
    void textEditorFocusLost(juce::TextEditor& editor) override;

    /**
     * @brief  Calls notifyListeners when the editor is focused and the return
     *         key is pressed.
     *
     * @param editor  This component's internal TextEditor object.
     */
    void textEditorReturnKeyPressed(juce::TextEditor & editor) override;

    /**
     * @brief  Opens the file selection window when the file selection button
     *         is clicked.
     *
     * @param button  This component's file selection button.
     */
    void buttonClicked(juce::Button* button) override;

    /**
     * @brief  Resizes child components to fit when the component's bounds
     *         change.
     */
    void resized() override;

    // The editable text field. This shows the selected path, or whatever other
    // value the user types in.
    Widgets::TextEditor filePath;

    // Opens the file selection window:
    juce::TextButton fileSelectButton;

    // Sets which files the file selection window will allow:
    juce::WildcardFileFilter fileFilter;

    // The title to display on the file selection window:
    juce::String selectionTitle;

    // A brief description to display on the file selection window:
    juce::String selectionText;

    // Stores all objects that track when this editor's value changes:
    std::set<Listener*> listeners;

    // Image preview component to use in the file selection window:
    juce::ImagePreviewComponent imagePreview;

    // Default file filter, allowing image types:
    static const juce::WildcardFileFilter imageFilter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilePathEditor)
};
