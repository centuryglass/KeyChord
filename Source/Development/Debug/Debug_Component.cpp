#include "Debug_Component.h"

#ifdef JUCE_DEBUG
// Print the full namespace name before all debug output:
static const constexpr char* dbgPrefix = "Debug::Component::";
#endif


// Prints the structure of the component tree to debug output.
void Debug::Component::trace()
{
    using namespace juce;
    static DrawableRectangle highlightFocus;
    highlightFocus.setFill(FillType(Colour(0x0)));
    highlightFocus.setStrokeFill(FillType(Colour(0xff00ff00)));
    highlightFocus.setStrokeType(PathStrokeType(4));
    std::function<void(juce::Component*, int) > recursiveInfo;
    recursiveInfo = [&recursiveInfo](juce::Component* component, int depth)
    {
        String indent = ":";
        for (int i = 0; i < depth; i++)
        {
            indent += "\t";
        }
        DBG(dbgPrefix << __func__ << indent << "Component:"
                << component->getName());
        indent += " ";
        DBG(dbgPrefix << __func__ << indent << "Position: ("
                << component->getX() << "," << component->getY() << ")");
        DBG(dbgPrefix << __func__ << indent << "Size: "
                << component->getWidth() << "x" << component->getHeight());
        String properties;
        if (component->getWantsKeyboardFocus())
        {
            properties += "wantsKeyFocus,";
        }
        if (component->hasKeyboardFocus(false))
        {
            properties += "hasKeyFocus,";
            highlightFocus.setBounds(component->getScreenBounds());
        }
        properties += component->isShowing() ? "showing" : "not showing";
        DBG(indent + properties);
        int numChildren = component->getNumChildComponents();
        if (numChildren > 0)
        {
            DBG(dbgPrefix << __func__ << indent << "Children:" << numChildren);
        }
        for (int i = 0; i < numChildren; i++)
        {
            recursiveInfo(component->getChildComponent(i), depth + 1);
        }
    };
    juce::Component * rootComponent = Desktop::getInstance().getComponent(0);
    recursiveInfo(rootComponent, 0);
    rootComponent->addAndMakeVisible(highlightFocus);
}
