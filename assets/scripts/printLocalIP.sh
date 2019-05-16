#!/bin/bash
# Prints the system's local ip address.

IFCONFIG=`ip a`
IFACE=$1
if [ -z "$IFACE" ]; then
    # Get first active interface if one isn't provided:
    if [[ $IFCONFIG =~ [0-9]:\ ([^:]*):[^:]*state\ UP ]]; then
        IFACE="${BASH_REMATCH[1]}"
    fi
fi

# Quit if no interface was found.
if [ -z "$IFACE" ]; then
    exit 0
fi

# Match interface's local IP
EXPR="$IFACE.*inet ([^/]*)"
if [[ $IFCONFIG =~ $EXPR ]]; then
    echo "${BASH_REMATCH[1]}"
fi
