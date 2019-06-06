#pragma once
/**
 * @file  Config_MainFile.h
 *
 * @brief  Provides access to the main configuration file resource.
 */

#include "Config_FileHandler.h"
#include "Text_CharTypes.h"

namespace Config
{
    class MainFile;
    class MainResource;
}

class Config::MainFile : public Config::FileHandler<MainResource>
{
public:
    MainFile();
    virtual ~MainFile() { }

    /**
     * @brief  Checks if the application should be displayed in minimized mode.
     *
     * @return  Whether mimimized mode is in use, hiding
     *          Component::ChordKeyDisplay and Component::ChordPreview.
     */
    bool getMinimized() const;

    /**
     * @brief  Gets whether the application is currently configured to snap to
     *         the bottom edge of the display.
     *
     * @return  True if the window belongs on the bottom edge, false if it
     *          belongs on the top edge.
     */
    bool getSnapToBottom() const;

    /**
     * @brief  Gets whether the application is in immediate input mode.
     *
     * @return  Whether all key input should be sent directly to the target
     *          window, without buffering.
     */
    bool getImmediateMode() const;

    /**
     * @brief  Takes any input buffer contents cached in the configuration file,
     *         removing it from disk.
     *
     *  This should only be used to preserve input buffer text when restarting
     * the application.
     *
     * @return  Any cached text input.
     */
    Text::CharString takeCachedBuffer();

    /**
     * @brief  Sets whether the application should start up in minimized mode.
     *
     * @param minimized  Whether the application window will be reduced in size,
     *                   hiding the Component::ChordKeyDisplay and the
     *                   Component::ChordPreview.
     */
    void setMinimised(const bool minimized);

    /**
     * @brief  Sets which edge of the display the window should snap to on 
     *         construction.
     *
     * @param snapToBottom  True if the window should use the bottom edge, false
     *                      if it should use the top edge.
     */
    void setSnapToBottom(const bool snapToBottom);
    
    /**
     * @brief  Sets whether the application should start in immediate mode.
     *
     * @param immediateMode  Whether key input should be forwarded immediately
     *                       to the target window, or cached until the user
     *                       decides to send it.
     */
    void setImmediateMode(const bool immediateMode);

    /**
     * @brief  Temporarily saves an input buffer string to the configuration
     *         file. This should only be used to preserve text input when
     *         restarting the application.
     *
     * @param inputBuffer  A Text::CharString containing the text input buffer.
     */
    void cacheInputBuffer(const Text::CharString inputBuffer);
};
