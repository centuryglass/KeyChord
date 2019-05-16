#pragma once
/**
 * @file  Layout_Transition_Type.h
 *
 * @brief  Lists the types of Component transition that can be created using
 *         Layout::Transition::Animator functions.
 */

namespace Layout { namespace Transition { enum class Type; } }


//Lists all available transition animations
enum class Layout::Transition::Type
{
    // Old items slide off the top of the window, new ones slide in from the
    // bottom.
    moveUp,
    // Old items slide off the bottom of the window, new ones slide in from the
    // top.
    moveDown,
    // Old items slide off the left side of the window, new ones slide in from
    // the right.
    moveLeft,
    // Old items slide off the right side of the window, new ones slide in from
    // the left.
    moveRight,
    // Old items are immediately removed, new items move from their current
    // bounds to their destination bounds.
    toDestination,
    // Old items are immediately replaced with new items without animating.
    none
};
