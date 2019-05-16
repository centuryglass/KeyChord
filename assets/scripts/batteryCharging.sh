#!/bin/bash
# Checks if the system battery is charging.
# Prints '1' if charging, '0' if not charging, or nothing if no battery is
# found.

# Find the battery device or file:
scriptDir=`dirname $0`
batterySource=`$scriptDir/batteryCheck.sh`

# If PocketCHIP battery file was found:
if [[ $batterySource == "/usr/lib/pocketchip-batt/voltage" ]]; then
    cat /usr/lib/pocketchip-batt/charging
    exit 0
fi


# If UPower device was found:
if [[ $batterySource == *"UPower"* ]]; then
    if `upower -i $batterySource | grep "state:.*charging"`; then
        echo "1"
    else
        echo "0"
    fi
    exit 0
fi

# If acpi output was found:
if [[ $batterySource == "Battery"* ]]; then
    if [[ $batterySource == *"Charging"* ]]; then
        echo "1"
    else
        echo "0"
    fi
    exit 0
fi

# If /sys/class/power_supply battery directory was found:
if [[ $batterySource == "/sys/class/power_supply"* ]]; then
    if [[ `cat "$batterySource/status"` == "Charging" ]]; then
        echo "1"
    else
        echo "0"
    fi
    exit 0
fi

# If /proc/acpi/battery directory was found:
if [[ $batterySource == "/proc/acpi/battery"* ]]; then
    if cat "$batterySource/state" | grep "charging state:\s*charging"; then
        echo "1"
    else
        echo "0"
    fi
fi
