# Image Asset Configuration
The [imageAssets.json](../../assets/configuration/imageAssets.json) configuration file defines all image assets used in the application. Image assets may be provided as absolute paths, or as paths relative to the application's assets directory.

## Basic image keys
Basic image keys refer to a single image file with fixed colors.

Key                       | Description
------------------------- | ---
"home page background"    | An optional background image drawn behind the main application page.

## Complex image keys
Complex image keys may provide multiple image file paths to switch between, and may provide custom color values. Each of these keys stores a JSON object with an optional color list stored under the "colours" key, and an image list stored under the "image files" key. Configurable colors may only be applied to .svg image files. When provided, each configurable color value will replace a single color in each image file. The colors to be replaced are defined in order in the colours.json configuration file, under the keys "Image0", "Image1", "Image2", "Image3", and "Image4".

Key                    | Image Count | Description
---------------------- | ----------- | ---
"popup menu"           | 1           | The frame drawn behind the component used to edit application menu items.
"navigation buttons"   | 4           | Directional navigation button icons.
"loading spinner"      | 8           | The animated loading spinner.
"cancel button"        | 1           | The small button used to cancel changes and remove items in editor components.
"confirm button"       | 1           | The small button used to confirm changes in editor components.

### Alternate image descriptions
Image keys with multiple image assets use each image for a specific purpose, documented here.

### Navigation buttons

- Up navigation button
- Down navigation button
- Left navigation button
- Right navigation button

### Loading spinner
Each image is a separate frame in the loading spinner animation.
