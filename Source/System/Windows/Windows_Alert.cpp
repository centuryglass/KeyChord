#include "Windows_Alert.h"
#include "JuceHeader.h"

// Requests user confirmation before performing some action.
void Windows::Alert::confirmAction(
        const juce::String title,
        const juce::String message,
        const std::function<void()> onConfirm)
{
    juce::NativeMessageBox::showOkCancelBox(juce::AlertWindow::QuestionIcon,
            title, message, nullptr,
            juce::ModalCallbackFunction::create([onConfirm](int response)
            {
                if (response == 1)
                {
                    onConfirm();
                }
            }));
}
