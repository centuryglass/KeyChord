# Building and Installing the JUCE Template Project

## Installing From Compiled Packages
After building and uploading project packages, describe the installation process here.

## Building from sources

#### 1. Required Packages
     sudo apt-get install \
     git \
     build-essential \
     TODO: Update for specific projects
####  2. Cloning and building
      git clone --recursive [Project Git URL]
      cd [ProjectDir]
      make
      make install
#### 3. Updating
      git pull
      git submodule update
      make
      make install
