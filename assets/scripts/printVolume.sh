#!/bin/bash
#
# Prints the system volume percentage
#

#The mixer simple control to check:

# For PocketCHIP:
simpleControl='Power Amplifier'

# For Linux desktop testing:
#simpleControl='Master'

status=`amixer sget "$simpleControl"`
if [[ $status =~ \[([0-9]+)\% ]]; then
    echo "${BASH_REMATCH[1]}"
fi

