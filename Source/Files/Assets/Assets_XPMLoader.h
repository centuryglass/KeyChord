#pragma once
/**
 * @file  Assets_XPMLoader.h
 *
 * @brief  Loads juce::Image objects from .xpm files.
 */

namespace juce
{
    class Image;
    class File;
}

namespace Assets
{
    namespace XPMLoader
    {
        /**
         * @brief  Loads a juce Image object from a xpm image file.
         *
         * @param xpmFile  A xpm file object.
         *
         * @return         An image loaded from the file, or a null image if
         *                 the file was not a valid xpm file.
         */
        juce::Image loadXPMImage(const juce::File xpmFile);
    }
}
