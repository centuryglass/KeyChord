#!/bin/bash
### DeadLinkF.sh ############################################################
# Opens Vim editing the first detected line in the project that exceeds the 80
# character line length limit.
#--- Usage: ---
# ./project-scripts/OverflowSeek.sh
################################################################################


#==============================================================================#
#--- findMatch: ---
# Recursively searches within a directory until a line longer than 80 characters
# is found, or all files have been searched. The $match variable will be set to
# a string contaiing the match file and line, or the empty string if no match is
# found.
#--- Parameters: ---
# $searchDir:  A directory path to recursively search for files with lines
#              exceeding 80 characters.
#==============================================================================#
function findMatch()
{
    local searchDir=$1
    match=`grep -rn '.\{81\}' $searchDir | head -1`
}

# In order, scan the main source code directory, the test class directory, and
# the project script directory.
findMatch './Source' 
if [ -z "$match" ]; then
    findMatch './Tests'
fi
if [ -z "$match" ]; then
    findMatch './project-scripts'
fi
if [ -z "$match" ]; then
    echo "No lines found over 80 characters."
    exit 0;
fi
file=`echo "$match" | cut -d: -f1`
line=`echo "$match" | cut -d: -f2`

echo "Fixing line $line of file $file"

vim +$line $file
