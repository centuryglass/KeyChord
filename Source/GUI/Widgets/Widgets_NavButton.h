#pragma once
/**
 * @file  Widgets_NavButton.h
 *
 * @brief  Provides a directional navigation button assigned to one edge of the
 *         window. Button position and direction are chosen based on the
 *         assigned edge.
 */

#include "Theme_Image_Component.h"
#include "Layout_Component_Manager.h"
#include "Widgets_DrawableImageButton.h"

namespace Widgets { class NavButton; }

/**
 * @brief  A directional navigation button assigned to one edge of the window.
 *
 *  NavButton objects are meant to be used for navigating between page
 * components and scrolling page content. NavButton's relative position and
 * size are defined in the Layout::Component configuration file, and its image
 * resources and colours are defined in the Theme::Image configuration file.
 *
 *  Unless the Layout::Component configuration file has been edited to place
 * them somewhere else, NavButtons sit centered against the side of the window
 * assigned to them on construction, pointing towards the assigned edge.
 *
 *  Because NavButtons are meant to be placed on the margins of the window,
 * convenience functions are provided to find the margin fractions needed to
 * keep other window content from overlapping with the arrow buttons.
 */
class Widgets::NavButton : public Theme::Image::Component<DrawableImageButton>
{
public:
    /**
     * @brief  Defines the four available button edge options.
     */
    enum class WindowEdge
    {
        up    = 0,
        down  = 1,
        left  = 2,
        right = 3
    };

    /**
     * @brief  Constructs the navigation button to sit on particular window
     *         edge.
     *
     * @param edge  The window edge to assign.
     */
    NavButton(const WindowEdge edge);

    virtual ~NavButton() { }

    /**
     * @brief  Gets the window edge assigned to this NavButton.
     *
     * @return  The WindowEdge value set when the button was created.
     */
    WindowEdge getEdge() const;

    /**
     * @brief  Gets the amount of horizontal margin space needed to keep window
     *         content from overlapping with a NavButton.
     *
     * @param edge  Sets which NavButton type's margin values are returned.
     *
     * @return      The fraction of the window's width that should be set aside
     *              as margin space.
     */
    static float xMarginFractionNeeded(const WindowEdge edge);

    /**
     * @brief  Gets the amount of vertical margin space needed to keep window
     *         content from overlapping with a NavButton.
     *
     * @param edge  Sets which NavButton type's margin values are returned.
     *
     * @return      The fraction of the window's height that should be set
     *              aside as margin space.
     */
    static float yMarginFractionNeeded(const WindowEdge edge);

    /**
     * @brief  Applies the navigation button bounds defined in the
     *         Layout::Component configuration file.
     */
    void applyConfigBounds();

private:
    /**
     * @brief  Given a window edge, return the component key string for the
     *         corresponding NavButton type.
     *
     * @param edge  One edge of the application window.
     *
     * @return  The component key string for the NavButton placed on that side.
     */
    static const juce::Identifier& getEdgeComponentKey(const WindowEdge edge);

    // Finds appropriate bounds using config data and window size:
    Layout::Component::Manager boundsManager;

    // Edge of the window assigned to the navigation button:
    const WindowEdge edge;
};
