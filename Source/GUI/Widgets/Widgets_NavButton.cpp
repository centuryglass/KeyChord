#include "Widgets_NavButton.h"
#include "Layout_Component_ConfigFile.h"
#include "Layout_Component_JSONKeys.h"
#include "Theme_Image_JSONKeys.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Widgets::NavButton::";
#endif

// Extra space to add to button margins:
static const constexpr float marginPadding = 0.02;

// Constructs the navigation button to sit on particular window edge.
Widgets::NavButton::NavButton(const WindowEdge edge) :
Theme::Image::Component<DrawableImageButton>(
        Theme::Image::JSONKeys::navButton),
boundsManager(this, getEdgeComponentKey(edge)),
edge(edge)
{
    setImageAssetIndex((int) edge);
    setWantsKeyboardFocus(false);
}


// Gets the window edge assigned to this NavButton.
Widgets::NavButton::WindowEdge Widgets::NavButton::getEdge() const
{
    return edge;
}


// Gets the amount of horizontal margin space needed to keep window content from
// overlapping with a NavButton.
float Widgets::NavButton::xMarginFractionNeeded
(const NavButton::WindowEdge edge)
{
    Layout::Component::ConfigFile config;
    Layout::Component::ConfigLayout layout
            = config.getLayout(getEdgeComponentKey(edge));
    return std::min(layout.getXFraction() + layout.getWidthFraction(),
            1.0f - layout.getXFraction()) + marginPadding;
}


// Gets the amount of vertical margin space needed to keep window content from
// overlapping with a NavButton.
float Widgets::NavButton::yMarginFractionNeeded(NavButton::WindowEdge edge)
{
    Layout::Component::ConfigFile config;
    Layout::Component::ConfigLayout layout
            = config.getLayout(getEdgeComponentKey(edge));
    return std::min(layout.getYFraction() + layout.getHeightFraction(),
            1.0f - layout.getYFraction()) + marginPadding;
}


// Applies the navigation button bounds defined in the Layout::Component
// configuration file.
void Widgets::NavButton::applyConfigBounds()
{
    boundsManager.applyConfigBounds();
}


// Given a window edge, return the component key string for the corresponding
// NavButton type.
const juce::Identifier& Widgets::NavButton::getEdgeComponentKey
(const WindowEdge edge)
{
    switch(edge)
    {
        case WindowEdge::up:
            return Layout::Component::JSONKeys::pageUp;
        case WindowEdge::down:
            return Layout::Component::JSONKeys::pageDown;
        case WindowEdge::left:
            return Layout::Component::JSONKeys::pageLeft;
        case WindowEdge::right:
            return Layout::Component::JSONKeys::pageRight;
    }
    DBG(dbgPrefix << __func__ << ": Unknown edge value!");
    return juce::Identifier::null;
}
