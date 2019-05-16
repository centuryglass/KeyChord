#!/bin/bash
#
# Checks if the display is off.
#
# Returns 0 if the display is off, 1 if the display is on.
#
status=$(xset q)
if [[ "$status" =~ .*Monitor\sis\sOff.* ]]; then
    exit 0
fi
exit 1
