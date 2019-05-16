#!/bin/bash
### BuildLabel.sh ##############################################################
# Generates an appropriate build label to apply while compiling.
################################################################################

# Removes quotes from around a string parameter and prints the result.
unquote()
{
    unquoted="${1%\"}"
    unquoted="${unquoted#\"}"
    echo $unquoted
}

# Attempts to generate and print a build label using the contents of a release
# file, printing an appropriate build name if the file contains expected data.
readReleaseFile()
{
    filePath=$1
    buildName=`grep -oP '(?<=^PRETTY_NAME=).+' $filePath`
    if [ -z "$buildName" ]; then
        buildName=`grep -oP '(?<=^NAME=).+' $filePath`
        buildName=`unquote "$buildName"`
        version=`grep -oP '(?<=^VERSION=").+' $filePath`
        if [ -n "$version" ]; then
            version=`unquote "$version"`
            buildName="$buildName $version"
        fi
    fi
    buildName=`unquote "$buildName"`

    if [ -z "$buildName" ]; then
        echo "Unknown"
    else
        echo $buildName
    fi
}

# Standard and alternate paths to the Linux OS release files:
releasePath="/etc/os-release"
altReleasePath="/usr/lib/os-release"

# Attempt to read build information from either release file:
if [ -f $releasePath ]; then
    readReleaseFile $releasePath
elif [ -f $altReleasePath ]; then
    readReleaseFile $altReleasePath
else
    echo "Unknown"
fi
