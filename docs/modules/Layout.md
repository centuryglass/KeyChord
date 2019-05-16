# Layout Module Documentation
The Layout module handles the placement of UI elements within the window. UI components can be individually placed at configurable locations using Layout\::Component\::ConfigLayout, or positioned in groups using Layout\::Group\::RelativeLayout. The Layout module also provides tools for animating UI components when applying new layouts.

## Relative Group Layout
The Relative Layout submodule defines the positions and sizes of UI components in relation to each other. It can be used to quickly define the placement of many non-overlapping UI components within a rectangular area.

#### [Layout\::Group\::RelativeLayout](../../Source/GUI/Layout/Group/Layout_Group_RelativeLayout.h)
Layout\::Group\::RelativeLayout objects hold layout values for a group of UI components. Each layout contains any number of layout rows, and each row contains any number of UI components. Each row is assigned a weight value used to calculate how much vertical space it is allocated relative to all other rows. Each component in the row also holds a weight value, defining how much horizontal space it is allocated relative to other components in the row.

#### [Layout\::Group\::Manager](../../Source/GUI/Layout/Group/Layout_Group_Manager.h)
Layout\::Group\::Manager applies a RelativeLayout object's layout to the components it contains. It uses a stored layout to calculate and set the bounds of each layout component within a rectangular area, optionally adding each layout component to a provided parent component.

## Configurable Component Layout
The Configurable Component layout submodule defines the positions of individual UI components within the application window. Component layout values are defined as fractions of the window width or height, saved within a JSON configuration file. This configuration file also provides the preferred font sizes used within the application.

#### [Layout\::Component\::ConfigLayout](../../Source/GUI/Layout/Component/Layout_Component_ConfigLayout.h)
ConfigLayout holds the position and size of a single UI component. Positions and sizes are relative to the dimensions of the application window. Width and x-coordinate are stored as fractions of the window's width, and height and y-coordinate are stored as fractions of the window's height.

#### [Layout\::Component\::Manager](../../Source/GUI/Layout/Component/Layout_Component_Manager.h)
Manager objects store a juce\::Component pointer and a ConfigLayout object loaded from the config file. The Manager will apply the layout to the Component on request, or allow the layout to be viewed directly.

#### [Layout\::Component\::ConfigFile](../../Source/GUI/Layout/Component/Layout_Component_ConfigFile.h)
ConfigFile objects share access to the component layout JSON file resource. They are used to load specific component layout objects, and to calculate ideal font heights using the configurable font heights saved in the JSON file.

#### [Layout\::Component\::TextSize](../../Source/GUI/Layout/Component/Layout_Component_TextSize.h)
TextSize lists the configurable text height values defined in the component layout JSON file. These sizes may be floating point values representing a height relative to the smallest window dimension, or integer values representing fixed pixel heights.

#### [Layout\::Component\::JSONKeys](../../Source/GUI/Layout/Component/Layout_Component_JSONKeys.h)
JSONKeys defines the key strings used to store component layouts and text heights in the JSON file.

#### [Layout\::Component\::JSONResource](../../Source/GUI/Layout/Component/Layout_Component_JSONResource.h)
The JSONResource object loads the component layout JSON file, and shares its data with all ConfigFile objects.

## Layout Transition Animations

#### [Layout\::Transition\::Animator](../../Source/GUI/Layout/Transition/Layout_Transition_Animator.h)
The Animator namespace provides component animation functions. These functions are primarily intended for animating changes in layout, but may be used for general purpose animation as well. Transition animations may be used to show old components moving out as new components move in, even if components in the old layout are reused in the new layout.

#### [Layout\::Transition\::Type](../../Source/GUI/Layout/Transition/Layout_Transition_Type.h)
Type defines the animation types that may be applied to component transitions. Components may slide onto or off of the page from any of the four cardinal directions, they may move directly from their current location to their destination, or they may be moved directly to their destination without animating.

