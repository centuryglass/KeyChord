#!/bin/bash
# Prints the battery charge percentage

# Find the battery device or file:
scriptDir=`dirname $0`

batterySource=`$scriptDir/batteryCheck.sh`
# Check for PocketCHIP battery voltage file:
voltagePath="/usr/lib/pocketchip-batt/voltage"
if [[ $batterySource == $voltagePath ]]; then
    maxVoltage=4250
    minVoltage=3275
    currentVoltage=`cat "$voltagePath"`
    let range=maxVoltage-minVoltage
    let currentLevel=currentVoltage-minVoltage
    let percent=currentLevel*100/range
    echo "$percent"
    exit 0
fi

# If UPower device was found:
if [[ $batterySource == *"UPower"* ]]; then
    echo `upower -i $batterySource | grep "percentage:" | sed "s/[^0-9]//g"`
    exit 0
fi

# If acpi output was found:
if [[ $batterySource == "Battery"* ]]; then
    echo $batterySource | grep -oE "[0-9]+%" | sed "s/%//"
    exit 0
fi

# If /sys/class/power_supply battery directory was found:
if [[ $batterySource == "/sys/class/power_supply"* ]]; then
    cat "$batterySource/uevent" | grep "POWER_SUPPLY_CAPACITY" | sed "s/[^0-9]//g"
    exit 0
fi

# If /proc/acpi/battery directory was found:
if [[ $batterySource == "/proc/acpi/battery"* ]]; then
    infoFile=$batterySource/info
    stateFile=$batterySource/state
    max=`cat $infoFile | grep "design capacity:" | sed "s/[^0-9]//g"`
    min=`cat $infoFile | grep "design capacity low:" | sed "s/[^0-9]//g"`
    current=`cat $stateFile | grep "remaining capacity:" | sed "s/[^0-9]//g"`
    let range=max-min
    let current=current-min
    let percent=current*100/range
    echo "$percent"
fi
