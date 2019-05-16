# System Command File
The [commands.json](../../assets/commands.json) system command file stores system commands used by the application that may vary between different systems. Because system commands usually shouldn't be changed, and because of the potential security risks involved, the command file is stored in the application's assets directory instead of with other configuration files.

Changes to commands.json will be overwritten whenever this application is updated. To make changes to system commands that don't get undone by updates, create a file called overrideCommands.json within the same directory as the commands.json file. Commands defined in this file will override commands defined in the default command file.

## Script Directory Value
The commands.json file includes a path to the default application script directory stored under the key "PROJECT_SCRIPTS". Within all command values in this file, the literal string "PROJECT_SCRIPTS" will be replaced with the script directory defined by this value.

## System Command List
Command Name        | Parameters Passed | Return Value Used | Expected Output | Expected Action
------------------- | ----------------- | ----------------- | --------------- | ---
shutdown            |                   |                   |                 | The system shuts down.
restart             |                   |                   |                 | The system restarts.
sleep               |                   |                   |                 | The display turns off.
wake                |                   |                   |                 | The display turns on.
calibrate           |                   |                   | New XInput calibration settings. | A display calibration window opens.
set volume          | Volume percentage.|                   |                 | The system volume levels are updated.
set brightness      | Display brightness, from one to ten. |      |      | The system's display brightness is updated.
sleep check         |                   | 0 if the display is off, 1 if it is on.
command check       | The name of a system command. |       | The full path to the command, or the empty string if the command is invalid.
get volume          |                   |                   | Volume percentage, or an error message if the command failed.
has battery         |                   |                   | A battery file path, DBus path, or other relevant battery source if a system battery is visible, nothing otherwise.
get battery percent |                   |                   | The remaining battery percentage.
get battery charging|                   |                   | '1' if the battery is charging, '0' if it isn't charging, or nothing if no battery is found.
get brightness      |                   |                   | Display brightness between one and ten, or an error message if the command failed.
get local IP        |                   |                   | The system's IP on the local network.
get public IP       |                   |                   | The system's current global IP.
