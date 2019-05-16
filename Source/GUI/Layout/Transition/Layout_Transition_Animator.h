#pragma once
/**
 * @file  Layout_Transition_Animator.h
 *
 * @brief  Provides functions for animating juce::Component transitions.
 */

#include "JuceHeader.h"

/**
 * @brief  Provides functions for animating the transition when replacing
 *         visible Components in the window.
 *
 *  Transition::Animator is primarily used to make removed components slide
 * offscreen while added components slide in from the opposite direction.
 */
namespace Layout { namespace Transition {

/**
 * @brief  Provides all transition type options.
 *
 * @see  Layout_Transition_Type.h
 */
enum class Type;

namespace Animator {

/**
 * @brief  Checks if a component is being animated through the default juce
 *         Animator.
 *
 * @param possiblyAnimating  A component that is possibly being animated.
 *
 * @return                   Whether the component is being animated by the
 *                           default animator.
 */
bool isAnimating(juce::Component* possiblyAnimating);

/**
 * @brief  Moves one set of components off-window, while moving another set of
 *         components on-window, animating the transition.
 *
 *  Any component that is in both sets will appear to move off-window while a
 * duplicate component moves on-window simultaneously.
 *
 * @param movingOut              An array of all components that will move off
 *                               of the window. If a component in this array is
 *                               also in the transitionIn list, a proxy
 *                               component will be created to slide off-window.
 *                               Any changes made to the component after calling
 *                               this function will not affect the proxy
 *                               component.
 *
 * @param movingIn               An array of all components that will move onto
 *                               the window, paired with their target bounding
 *                               rectangle. Components will be moved before
 *                               animating so that they slide in from the
 *                               correct direction.
 *
 * @param transition             The animation type to apply to all moving
 *                               Components.
 *
 * @param animationMilliseconds  Duration of the component animation, in
 *                               milliseconds.
 */
void animateTransition(
    juce::Array<juce::Component*> movingOut,
    juce::Array<std::pair<juce::Component*, juce::Rectangle<int>>> movingIn,
    const Type transition,
    const unsigned int animationMilliseconds);

/**
 * @brief  Moves a component off-window, animating the transition.
 *
 * @param component              An on-window component to move off-window. If
 *                               the component is already outside of the window
 *                               bounds, nothing will happen.
 *
 * @param transition             The animation type used to move the component
 *                               out of window bounds.
 *
 * @param animationMilliseconds  The duration of the component animation, in
 *                               milliseconds.
 *
 * @param useProxy               If true, the component will immediately be
 *                               moved to its final bounds, and a temporary
 *                               proxy component will be used to simulate the
 *                               transition animation.
 */
void transitionOut(
    juce::Component* const component,
    const Type transition,
    const unsigned int animationMilliseconds,
    const bool useProxy = false);

/**
 * @brief  Moves a component into the window bounds, animating the transition.
 *
 * @param component              A component to move onto the window. Before
 *                               animating, it will be re-positioned outside the
 *                               window bounds to ensure it moves in from the
 *                               right direction.
 *
 * @param transition             The animation type used to move the component
 *                               onto window bounds.
 *
 * @param destination            The destination bounds of the component,
 *                               relative to its parent component. If this does
 *                               not intersect with the window bounds, the
 *                               component will not be re-positioned or
 *                               animated.
 *
 * @param animationMilliseconds  The duration of the component animation, in
 *                               milliseconds.
 */
void transitionIn(juce::Component* component,
    const Type transition,
    const juce::Rectangle<int> destination,
    const unsigned int animationMilliseconds);

/**
 * @brief  Updates a component's bounds, animating the transformation.
 *
 * @param component              The component being transformed.
 *
 * @param destination            The new bounds to apply to the component,
 *                               relative to its parent component.
 *
 * @param animationMilliseconds  The duration of the component animation, in
 *                               milliseconds.
 *
 * @param useProxy               Sets whether a proxy component should be
 *                               animated in place of the real component.
 *
 * @param onFinish               An optional function to run once animation is
 *                               finished.
 */
void transformBounds(
    juce::Component* component,
    const juce::Rectangle<int>& destination,
    const unsigned int animationMilliseconds,
    const bool useProxy = false,
    const std::function<void()> onFinish = std::function<void()>());
}}}
