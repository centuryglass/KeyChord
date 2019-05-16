# Widgets Module Documentation
The Widgets module provides basic UI component classes used to build the application, expanding on the Component classes provided by the Juce library. It includes all custom Component classes designed to be reused throughout the application.

#### [Widgets\::BoundedLabel](../../Source/GUI/Widgets/Widgets_BoundedLabel.h)
BoundedLabel is a juce\::Label component with additional options for controlling font size.

#### [Widgets\::TextEditor](../../Source/GUI/Widgets/Widgets_TextEditor.h)
TextEditor is a subclass of juce\::TextEditor that updates its font height to match its current bounds.

#### [Widgets\::Switch](../../Source/GUI/Widgets/Widgets_Switch.h)
The Switch class provides an animated binary switch component.

#### [Widgets\::IconSlider](../../Source/GUI/Widgets/Widgets_IconSlider.h)
The IconSlider class provides a horizontal slider component with icons displayed on both of its sides.

#### [Widgets\::DelayedIconSlider](../../Source/GUI/Widgets/Widgets_DelayedIconSlider.h)
The DelayedIconSlider class is an IconSlider subclass meant for simple, single purpose sliders that run an update callback at a controlled frequency.

#### [Widgets\::Spinner](../../Source/GUI/Widgets/Widgets_Spinner.h)
The Spinner class provides a loading indicator component that shows an animated spinning circle.

#### [Widgets\::OverlaySpinner](../../Source/GUI/Widgets/Widgets_OverlaySpinner.h)
The OverlaySpinner class provides a Spinner and an optional text label displayed over an overlay that covers the entire window.

#### [Widgets\::FilePathEditor](../../Source/GUI/Widgets/Widgets_FilePathEditor.h)
The FilePathEditor class provides a text editor and button used to select and edit a file path.

#### [Widgets\::LayoutContainer](../../Source/GUI/Widgets/Widgets_LayoutContainer.h)
LayoutContainer objects display and arrange a set of child components.

#### [Widgets\::DrawableImage](../../Source/GUI/Widgets/Widgets_DrawableImage.h)
DrawableImage is an image component class that automatically scales its image to match its bounds.

#### [Widgets\::DrawableImageButton](../../Source/GUI/Widgets/Widgets_DrawableImageButton.h)
DrawableImageButton is a juce\::Button subclass that also displays a DrawableImage.

#### [Widgets\::Counter](../../Source/GUI/Widgets/Widgets_Counter.h)
Counter objects provide simple controls used to select an integer value.

#### [Widgets\::NavButton](../../Source/GUI/Widgets/Widgets_NavButton.h)
NavButton is a navigation button class that can be set to point up, down, left, or right.

#### [Widgets\::PagedList](../../Source/GUI/Widgets/Widgets_PagedList.h)
PagedList is an abstract list class that can split its content between multiple pages, navigated using NavButton components.

#### [Widgets\::FocusingPagedList](../../Source/GUI/Widgets/Widgets_FocusingPagedList.h)
FocusingPagedList is an abstract PagedList class with selectable list items. Selected list items expand to fill the entire list component, hiding all other list content until they are deselected.

#### [Widgets\::PopupEditor](../../Source/GUI/Widgets/Widgets_PopupEditor.h)
PopupEditor is an abstract component class used to display popup content editor tools.

#### [Widgets\::ListEditor](../../Source/GUI/Widgets/Widgets_ListEditor.h)
ListEditor is a PopupEditor class used to edit a list of strings.

#### [Widgets\::ColourPicker](../../Source/GUI/Widgets/Widgets_ColourPicker.h)
ColourPicker provides a component used to select colour values.
