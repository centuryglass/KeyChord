#!/bin/bash
# Launch your application with gdb, using local libraries compiled with debug flags.
APPNAME=
env LD_LIBRARY_PATH=/home/$USER/.local/lib:$LD_LIBRARY_PATH gdb "$APPNAME"
