# KeyChord
A chorded software keyboard for the ClockworkPi GameShell and similar devices. It allows quick and easy typing using the GameShell's light keys and face buttons. Controls can be easily reconfigured to work with alternate key bindings.

[Basic installation instructions: full documentation coming soon](./docs/BuildAndInstall)

![Keyboard Preview](./docs/screenshots/typing.png?raw=true "typing with KeyChord")

With a chorded keyboard, rather than mapping each key to a different character, you have a small number of chord keys you use to type all characters. Each character uses a different combination of chord keys, and you type that character by holding down its chord key combination and releasing them all at once.


The KeyChord keyboard displays all the chord mappings on screen. As you hold down chord keys, it marks which character would be entered if you released the keys, and highlights the characters that you could select by holding down additional keys. Other button mappings handle switching between character sets, moving the keyboard window, and sending input from the keyboard to the target window.

| ![Default controls](./docs/screenshots/controls.png?raw=true "default controls") |
| :---: |
| Default keyboard controls. |

KeyChord is highly configurable. Some supported customizations:

| [![Custom controls](./docs/screenshots/altControls.png?raw=true "alternate control example")](./assets/configuration/keyBindings_alt.json)  | [![Alternate character sets](./docs/screenshots/altCharSet.png?raw=true "Danish/Norwegian alphabet example")](./assets/configuration/charSets_alt.json) | ![Custom colors](./docs/screenshots/altColors.png?raw=true "Custom color example") |
|:---:|:---:|:---:|
| Custom controls| Alternate character sets | Custom colors |


