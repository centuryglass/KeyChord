#pragma once
/**
 * @file  Layout_Group_Manager.h
 *
 * @brief  Updates and applies a Group::RelativeLayout to a set of Components.
 */

#include "Layout_Group_RelativeLayout.h"
#include "Layout_Transition_Type.h"
#include "JuceHeader.h"

namespace Layout { namespace Group { class Manager; } }

/**
 * @brief   Stores a single RelativeLayout object, and uses its held layout to
 *         position or animate the layout's components within a separate parent
 *         component.
 */
class Layout::Group::Manager
{
public:
    Manager() { }

    virtual ~Manager() { }

    /**
     * @brief  Gets the current component group layout held by this Manager.
     *
     * @return  A copy of the layout saved with setLayout().
     */
    RelativeLayout getLayout() const;

    /**
     * @brief  Set a new Component group layout removing any previously set
     *         layout, and optionally places all layout components within a
     *         containing Component.
     *
     * @param layout         Defines the position and weight of all components.
     *
     * @param parentToInit   If a non-null Component pointer is provided, this
     *                       will add all components in the layout as children
     *                       of parentToInit and make them visible.
     */
    void setLayout(const RelativeLayout& layout,
            juce::Component* parentToInit = nullptr);

    /**
     * @brief  Changes the current layout, immediately applies the updated
     *         layout to all components in the layout, and optionally animates
     *         the transition.
     *
     * @param newLayout       The new layout to apply.
     *
     * @param parent          The parent component of all layout components.
     *                        All components in the old layout will be removed
     *                        from this component's children. All components in
     *                        the new layout will be added to this component as
     *                        children. Layout components will be placed within
     *                        this component's bounds.
     *
     * @param transition      Optional transition animation to apply when
     *                        updating the layout.
     *
     * @param duration        If animating the transition, this defines the
     *                        animation duration in milliseconds. Animation
     *                        duration is set to zero by default.
     *
     * @param animateUnmoved  Sets whether transitioned items will be animated
     *                        if their bounds do not change in the transition.
     *                        By default, animations will apply to all layout
     *                        components.
     */
    void transitionLayout(
            const RelativeLayout& newLayout,
            juce::Component* parent,
            const Transition::Type transition = Transition::Type::none,
            const unsigned int duration = 0,
            const bool animateUnmoved = true);

    /**
     * @brief  Adds all components in the layout to a parent component and makes
     *         them all visible.
     *
     * @param parent  The parent component that will hold all layout components.
     */
    void addComponentsToParent(juce::Component* parent);

    /**
     * @brief  Arranges the components within a bounding rectangle, optionally
     *         applying a transition animation to all components in the layout.
     *
     * @param bounds          The rectangle where all layout components will be
     *                        arranged.
     *
     * @param transition      An optional transition animation to apply.
     *
     * @param duration        The duration of any transition animation in
     *                        milliseconds.
     *
     * @param animateUnmoved  Sets whether transitioned items will be animated
     *                        if their bounds do not change in the transition.
     *                        By default, animations will apply to all layout
     *                        components.
     */
    void layoutComponents(
            const juce::Rectangle<int>& bounds,
            const Transition::Type transition = Transition::Type::none,
            const unsigned int duration = 0,
            const bool animateUnmoved = true);

    /**
     * @brief  Remove all saved component layout parameters.
     *
     * @param removeComponentsFromParent   If true, all components will also be
     *                                     removed from their parent component.
     */
    void clearLayout(bool removeComponentsFromParent = false);

    #if JUCE_DEBUG
    /**
     * @brief  Print out the layout to the console for debugging.
     */
    void printLayout();
    #endif

private:
    // Stores the bounds of all components in a layout row, indexed by column
    // position.
    typedef juce::Array<juce::Rectangle<int>> BoundsList;

    // Stores the bounds of all components in a layout, indexed by row and
    // column position.
    typedef juce::Array<BoundsList> BoundsGrid;

    /**
     * @brief  Finds where the layout manager would place each layout item
     *         within a given bounding box.
     *
     * @param layoutBounds  The rectangle where all layout items would be
     *                      arranged.
     *
     * @return              The BoundsGrid storing the individual bounding boxes
     *                      that would be applied to each layout item.
     */
    BoundsGrid getBoundsGrid(const juce::Rectangle<int>& layoutBounds) const;

    /**
     * @brief  Updates the positions and sizes of all layout Components using an
     *         existing set of layout item bounding rectangles.
     *
     * @param boundsGrid      A grid containing bounding boxes for each item in
     *                        the manage layout.
     *
     * @param transition      An optional transition animation to apply.
     *
     * @param duration        The duration of any transition animation in
     *                        milliseconds.
     *
     * @param animateUnmoved  Sets whether transitioned items will be animated
     *                        if their bounds do not change in the transition.
     *                        By default, animations will apply to all layout
     *                        components.
     */
    void layoutComponents(
            const BoundsGrid& boundsGrid,
            const Transition::Type transition = Transition::Type::none,
            const unsigned int duration = 0,
            const bool animateUnmoved = true);

    // Current managed layout:
    RelativeLayout layout;

    // The sum of RowItem weights for each row.
    juce::Array<unsigned int> xWeightSums;
    // The sum of all row layout weights.
    unsigned int yWeightSum = 0;
};
