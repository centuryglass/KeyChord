#!/bin/bash
#
# Calibrates the touch screen. If possible, the calibration settings are saved.
#
settings=$(xinput_calibrator)

# Check for returned calibration settings
settings=`expr match "$settings" 'Section .*'`

# TODO: Check this output, then remove old settings from file, and add these new
#       settings.
echo $settings
