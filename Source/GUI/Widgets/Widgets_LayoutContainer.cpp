#include "Widgets_LayoutContainer.h"

// Sets the layout of the container component.
void Widgets::LayoutContainer::setLayout
(const Layout::Group::RelativeLayout layout)
{
    layoutManager.clearLayout(true);
    layoutManager.setLayout(layout, this);
    if (!getBounds().isEmpty())
    {
        resized();
    }
}


// Sets a background image to draw behind all child components.
void Widgets::LayoutContainer::setBackgroundImage(const juce::Image bgImage)
{
    backgroundImage = bgImage;
}


// Adds all components in the layout to the container and makes them visible.
void Widgets::LayoutContainer::addAndShowLayoutComponents()
{
    layoutManager.addComponentsToParent(this);
}


// Updates component layout and back button bounds when the container is
// resized.
void Widgets::LayoutContainer::resized()
{
    layoutManager.layoutComponents(getLocalBounds());
    containerResized();
}


// Optionally fills the container background with an image.
void Widgets::LayoutContainer::paint(juce::Graphics& graphics)
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
