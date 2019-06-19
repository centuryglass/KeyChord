#pragma once
/**
 * @file  Component_ColourIds.h
 *
 * @brief  Defines the shared JUCE ColourId codes used when selecting keyboard
 *         colors.
 */

namespace Component
{
    /**
     * @brief  The set of shared colors used by Component::KeyGrid classes.
     *
     */
    enum ColourIds
    {
        // Color used to draw text:
        text = 0x1900100,
        // Color used to draw text for characters that conflict with the current
        // selection:
        inactiveText = 0x1900101,
        // Colors used to draw held chord keys on the selected character:
        chord1Selected = 0x1900102,
        chord2Selected = 0x1900103,
        chord3Selected = 0x1900104,
        chord4Selected = 0x1900105,
        chord5Selected = 0x1900106,
        emptySelected =  0x1900107, // Empty = key should not be held down.
        // Colors used to draw held chord keys in chords that don't conflict
        // with the current selection:
        chord1Active = 0x1900108,
        chord2Active = 0x1900109,
        chord3Active = 0x190010a,
        chord4Active = 0x190010b,
        chord5Active = 0x190010c,
        emptyActive =  0x190010d,
        // Colors used to draw chord keys in chords that aren't held down and
        // don't conflict with the current selection:
        chord1Open = 0x190010e,
        chord2Open = 0x190010f,
        chord3Open = 0x1900110,
        chord4Open = 0x1900111,
        chord5Open = 0x1900112,
        emptyOpen =  0x1900113,
        // Colors used to draw inactive chord keys that conflict with the
        // current selection:
        chord1Blocked = 0x1900114,
        chord2Blocked = 0x1900115,
        chord3Blocked = 0x1900116,
        chord4Blocked = 0x1900117,
        chord5Blocked = 0x1900118,
        emptyBlocked  = 0x1900119,
    };
}
