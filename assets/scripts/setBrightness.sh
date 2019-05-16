#!/bin/bash
#
# Sets the display brightness percentage
#

echo "$1" > /sys/class/backlight/backlight/brightness
