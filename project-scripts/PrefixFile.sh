#!/bin/bash
# Recursively adds a prefix string to the name of each file within a directory.
# Use: ./prefixFile.sh /path/to/directory "PrefixString"
if [[ ! -d $1 ]]; then
    echo "$1 is not a valid directory."
    exit 0
fi
if [[ -z $2 ]]; then
    echo "Failed to provide a filename prefix string."
    exit 0
fi

rootDir=$1
prefix=$2
count=0

function recursiveRename()
{
    local searchDir=$1
    for filePath in $searchDir/*; do
        if [[ -d $filePath ]]; then
            recursiveRename $filePath
        else
            fileName=$(basename $filePath)
            mv $filePath "$searchDir/$prefix$fileName"
            ((count++))
        fi
    done
}
recursiveRename $rootDir
echo "Renamed $count files."
