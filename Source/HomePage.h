#pragma once
/**
 * @file  HomePage.h
 *
 * @brief  Creates the application's primary UI page.
 */

#include "Page_Component.h"
#include "Layout_Component_Manager.h"
#include "Theme_Image_ConfigListener.h"
#include "Util_Commands.h"
#include "ChordComponent.h"

/**
 * @brief  The root Page::Component within the application.
 *
 *  All initial application interface elements should be added within this
 * component.
 */
class HomePage : public Page::Component, public Theme::Image::ConfigListener
{
public:
    /**
     * @brief  Initializes all page components.
     */
    HomePage();

    virtual ~HomePage() { }

protected:
    /**
     * @brief  Tracks page background changes.
     *
     *  Only the Theme::Image ConfigFile should call this function. This will
     * update the page background.
     *
     * @param key  The page background key.
     */
    virtual void configValueChanged(const juce::Identifier& key) override;

private:
    /**
     * @brief  Updates all child component bounds when the page is resized.
     */
    void pageResized() override;

    // Loads system commands. This resource should exist as long as the home
    // page exists.
    Util::Commands systemCommands;

    // Loads the background image and ensures the image asset JSON resource
    // remains available:
    const Theme::Image::ConfigFile imageConfig;

    // Displays the chorded keyboard preview:
    ChordComponent chordComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HomePage);
};
