#pragma once
#ifndef TEXT_CHARSET_CONFIG_IMPLEMENTATION
    #error "Text_CharSet_JSONResource.h included outside of config implementation!"
#endif
/**
 * @file  Text_CharSet_JSONResource.h
 *
 * @brief  Reads all configurable character set data.
 */

#include "Config_FileResource.h"
#include "Text_CharSet_Type.h"
#include "Text_CharSet_Cache.h"
#include "JuceHeader.h"
#include <map>

namespace Text { namespace CharSet { class JSONResource; } }

/**
 * @brief  Reads all configurable character set data.
 *
 *  The display order and types of characters, and chord assignment precedence
 * are fully configurable in the character set configuration file.
 *
 *  This resource also tracks and shares which character set is currently
 * active.
 */
class Text::CharSet::JSONResource : public Config::FileResource
{
public:
    // SharedResource object key:
    static const juce::Identifier resourceKey;

    /**
     * @brief  Loads all character sets on construction.
     */
    JSONResource();

    virtual ~JSONResource() { }

    /**
     * @brief  Gets a character set.
     *
     * @param setType  The type of character set to get.
     *
     * @return         The object containing the character set, shifted values,
     *                 and assigned chords.
     */
    const Cache& getCharacterSet(const Type setType) const;

    /**
     * @brief  Gets the character set type that is currently selected.
     *
     * @return  The character set currently being used.
     */
    Type getActiveType() const;

    /**
     * @brief  Checks if shifted character sets are currently in use.
     *
     * @return   Whether shifted sets are currently enabled.
     */
    bool getShifted() const;

    /**
     * @brief  Updates the active character set type
     *
     * @param newActiveType  The type that should replace the former active
     *                       type.
     */
    void setActiveType(const Type newActiveType);

    /**
     * @brief  Sets whether the shifted versions of character sets will be used.
     *
     * @param shifted  True to use shifted versions, false to use the default
     *                 versions.
     */
    void setShifted(const bool shifted);

private:
    /**
     * @brief  Gets the set of all basic(non-array, non-object) properties
     *         tracked by this Resource.
     *
     * @return  All specific string values stored by this resouve
     */
    const std::vector<Config::DataKey>& getConfigKeys() const final override;

    /**
     * @brief  Checks if a key string is valid for this FileResource.
     *
     * @param key  A key string value to check.
     *
     * @return     Whether the key is a valid string value or character set.
     */
    bool isValidKey(const juce::Identifier& key) const override;

    // All character sets, indexed by Type.
    juce::Array<Cache> characterSets;

    // The active set type:
    Type activeType = Type::main;

    // Whether shifted character sets are in use:
    bool shifted = false;
};
