#!/bin/bash
# DepClean.sh
# Removes build files that depend on removed/renamed files

buildDir=$1
projectDir=$(cd $(dirname $0)/.. && pwd)


if [[ ! -d $buildDir ]]; then
   echo "$buildDir is not a valid directory!"
   exit -1
fi

if [[ ! $(ls -A $buildDir | grep "d\$") ]]; then
   echo "No dependency files to check."
   exit 0
fi


echo "Removing build files with missing dependencies:"

numScanned=0
numRemoved=0
for fileName in $buildDir/*.d; do
    numDeps=0
    for path in $(<$fileName); do
        numDeps=$(($numDeps + 1))
        echo -en "\r\033[KChecking $numScanned, dependency $numDeps"
        if [[ $path == '\' ]]; then
            continue
        fi
        if [[ ${path: -1} == ':' ]]; then
            strlen=${#path}
            path=${path:0:$(($strlen - 1))}
        fi
        if [[ -f $path ]]; then
            continue
        fi
        fullPath="$projectDir/$path"
        if [[ ! -f $fullPath ]]; then
            oldFile=$(echo $fileName | sed 's/\(.*\)\..*/\1/')
            rm "$oldFile.d"
            rm "$oldFile.o"
            numRemoved=$(($numRemoved + 1))
            break
        fi
    done
    numScanned=$(($numScanned + 1))
done
echo -e "\r\033[K$numRemoved/$numScanned build files were removed."

