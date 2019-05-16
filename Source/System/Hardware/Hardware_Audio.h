#pragma once
/**
 * @file  Hardware_Audio.h
 *
 * @brief  Provides functions for initializing system audio and viewing/changing
 *         system volume.
 */

namespace Hardware
{
    namespace Audio
    {
        /**
         * @brief  Gets the system's volume level.
         *
         * @return  The system audio volume level, as a percentage of the
         *          maximum volume.
         */
        int getVolumePercent();

        /**
         * @brief  Changes the system audio volume level.
         *
         * @param volumePercent  The volume level, which will be constrained to
         *                       values between 0 and 100.
         */
        void setVolume(const int volumePercent);
    }
}
