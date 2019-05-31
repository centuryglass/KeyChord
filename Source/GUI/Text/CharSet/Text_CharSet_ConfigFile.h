#pragma once
/**
 * @file  Text_CharSet_ConfigFile.h
 *
 * @brief  Controls access to the character set configuration resource.
 */

#include "Config_FileHandler.h"
#include "Text_CharSet_Type.h"
#include "Text_CharSet_Cache.h"

namespace Text
{
    namespace CharSet
    {
        class ConfigFile;
        class JSONResource;
    }
}

class Text::CharSet::ConfigFile : public Config::FileHandler<JSONResource>
{
public:
    ConfigFile();

    virtual ~ConfigFile() { }

    /**
     * @brief  Gets cached data for the active character set.
     *
     * @return      All cached data for the active character set.
     */
    const Cache& getActiveSet() const;

    /**
     * @brief  Gets a character set's displayed name.
     *
     * @param type  The character set being named.
     *
     * @return      The configurable character set display name, used on the
     *              help screen.
     */
    juce::String getSetName(const Type type) const;

    /**
     * @brief  Gets the character set type that is currently selected.
     *
     * @return  The character set currently being used.
     */
    const Type getActiveType() const;

    /**
     * @brief  Updates the active character set type
     *
     * @param newActiveType  The type that should replace the former active
     *                       type.
     */
    void setActiveType(const Type newActiveType);
};
