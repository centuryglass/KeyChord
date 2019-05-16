#pragma once
/**
 * @file  Widgets_IconSlider.h
 *
 * @brief  Provides a slider UI component with images drawn on either side to
 *         show the slider's purpose.
 */

#include "Theme_Image_Component.h"
#include "JuceHeader.h"

namespace Widgets { class IconSlider; }


/**
 * @brief  A horizontal slider component that draws icons on its left and right
 *         sides.
 */
class Widgets::IconSlider : public juce::Component
{
public:
    /**
     * @brief  Creates a slider with a default range of 0-100.
     *
     * @param imageKey  The key to the slider's image assets within the image
     *                  asset configuration file.
     */
    IconSlider(const juce::Identifier& imageKey);

    virtual ~IconSlider() { }

    /**
     * @brief  Changes the slider's stored value.
     *
     * @param newValue      Sets the slider position and stored value. Values
     *                      will be restricted to the current slider range.
     *
     * @param notification  Determines if listeners will be notified of this
     *                      change.
     */
    void setValue(double newValue,
            juce::NotificationType notification = juce::sendNotificationAsync);

    /**
     * @brief  Gets the slider's stored value.
     *
     * @return  The current slider position value.
     */
    double getValue() const;

    /**
     * @brief  Subscribes a slider listener object to changes in the slider's
     *         value.
     *
     * @param listener   The listener to receive updates whenever the slider
     *                   value changes.
     */
    void addListener(juce::Slider::Listener* listener);

    /**
     * @brief  Compares a raw slider pointer to this object.
     *
     * @param sliderPtr  A pointer to a juce::Slider object.
     *
     * @return           Whether the sliderPtr points to this object's internal
     *                   slider.
     */
    bool operator== (const juce::Slider* sliderPtr) const ;

    /**
     * @brief  Sets the slider value's limits.
     *
     * @param newMinimum   The lowest permitted value.
     *
     * @param newMaximum   The highest permitted value.
     *
     * @param newInterval  Controls which values between the minimum and
     *                     maximum are considered valid.
     *
     *  If set to a nonzero value, the slider value will only change in
     * discrete steps equal to the interval size. Otherwise, the slider will be
     * able to select any value in the continuous range between the minimum and
     * the maximum.
     */
    void setRange(double newMinimum, double newMaximum,
            double newInterval = 0);

private:
    /**
     * @brief  Updates the icons and slider to fit the component bounds.
     */
    void resized() override;

    // Icon displayed to the left of the slider:
    Theme::Image::Component<> lowIcon;

    // Icon displayed to the right of the slider:
    Theme::Image::Component<> highIcon;

    // Internal slider component:
    juce::Slider slider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IconSlider)
};
