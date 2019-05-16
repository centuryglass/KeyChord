#include "Layout_Component_Manager.h"
#include "Layout_Component_ConfigFile.h"

namespace ComponentLayout = Layout::Component;

// Sets the layout manager's component and layout on construction.
ComponentLayout::Manager::Manager
(juce::Component* component, const juce::Identifier& layoutKey) :
component(component),
layoutKey(layoutKey)
{
    ConfigFile layoutReader;
    layout = layoutReader.getLayout(layoutKey);
}


// Uses configuration data to set the component's bounds.
void ComponentLayout::Manager::applyConfigBounds()
{
    if (component != nullptr)
    {
        juce::Rectangle<int> newBounds = component->getBounds();
        newBounds.setX(layout.getX(newBounds.getX()));
        newBounds.setY(layout.getY(newBounds.getY()));
        newBounds.setWidth(layout.getWidth(newBounds.getWidth()));
        newBounds.setHeight(layout.getHeight(newBounds.getHeight()));
        component->setBounds(newBounds);
    }
}


// Gets the key that selects the component's layout.
const juce::Identifier& ComponentLayout::Manager::getLayoutKey() const
{
    return layoutKey;
}


// Gets the layout object this Manager will apply.
ComponentLayout::ConfigLayout ComponentLayout::Manager::getLayout() const
{
    return layout;
}
