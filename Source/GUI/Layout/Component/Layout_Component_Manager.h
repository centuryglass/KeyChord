#pragma once
/**
 * @file  Layout_Component_ConfigLayout.h
 *
 * @brief  Applies properties set in the component layout JSON file to a
 *         juce::Component object.
 */

#include "Layout_Component_ConfigLayout.h"

/**
 * @brief  Defines the position and size of a single Component relative to the
 *         window size.
 *
 *  Component width and x-position are defined as fractions of the window's
 * width. Component height and y-position are defined as fractions of the
 * window's height. Any combination of width, height, x-position, and y-position
 * may be left undefined when the values they represent should not vary with
 * window size.
 */
namespace Layout { namespace Component { class Manager; } }
class Layout::Component::Manager
{
public:
    /**
     * @brief  Sets the layout manager's component and layout on construction.
     *
     * @param component     The juce::Component to be positioned by this
     *                      Manager.
     *
     * @param componentKey  The component key used to select a ConfigLayout that
     *                      will be applied to the component.
     */
    Manager(juce::Component* component, const juce::Identifier& layoutKey);

    virtual ~Manager() { }

    /**
     * @brief  Uses layout data to set the component's bounds.
     *
     *  All sizes and coordinates are relative to the application window size.
     * Any coordinates or dimensions that are not defined in the configuration
     * file will remain unchanged.
     */
    void applyConfigBounds();

    /**
     * @brief  Gets the key that selects the component's layout.
     *
     * @return  The stored layout key.
     */
    const juce::Identifier& getLayoutKey() const;

    /**
     * @brief  Gets the layout object this Manager will apply.
     *
     * @return  The saved layout object.
     */
    ConfigLayout getLayout() const;

private:
    // The managed juce::Component
    juce::Component* component;
    // The layout applied to the managed juce::Component.
    ConfigLayout layout;
    // The component layout key
    const juce::Identifier& layoutKey;
};
