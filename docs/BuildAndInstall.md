# Building and Installing KeyChord for Gameshell
These instructions will be improved shortly.

## 1. Remapping Light Keys:

  To properly use the light keys for chorded keyboard entry, the key module needs to be remapped so that LK3 is no longer assigned as shift. Follow [these instructions](https://forum.clockworkpi.com/t/tutorial-how-to-compile-and-upload-code-to-the-keypad/1065) to get everything set up to update the keypad.  I've included my alternate keypad code in this project in the clockworkpi_Keypad directory. This will remap LK3 to 'z', and shift + LK3 to f12.

All steps from this point on should be done on the Gameshell over SSH.

## 2. Building KeyChord from Source:

#### - Install Required Packages
     sudo apt-get install \
     git \
     build-essential \
     libx11-dev \
     libxrandr-dev \
     libxcursor-dev \
     libxft-dev \
     libxinerama-dev

####  - Clone, Build, and Install
      git clone --recursive https://www.github.com/centuryglass/KeyChord
      cd KeyChord
      make
      make install

## 3. Configuring KeyChord to launch with shift + LK3
- Install xbindkeys with `sudo apt-get install xbindkeys`
- Create or modify the file at /home/cpi/.xbindkeysrc to launch KeyChord when F12 is pressed. 
    * You can set that up by running `echo -e "\"KeyChord\"\n  F12" >> /home/cpi/.xbindkeysrc`
- Setup xbindkeys to run automatically on launch. 
    * Edit /home/cpi/launcher/.xinitrc. 
    * Within that file, you'll see two lines that start with "feh --bg-center". 
    * Under each of those lines, add `xbindkeys` as a new line.
