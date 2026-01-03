Pico Launcher

Pico Launcher is a lightweight Nintendo DS front-end for Pico Loader that lets you launch homebrew and retail games with automatic emulator selection and ROM handoff, similar to TWiLight Menu++ Virtual Console AddOns.

It’s designed to be fast, simple, and flashcart-friendly.

✨ Features

Front-end for Pico Loader

Automatic emulator launching with ROM pre-selected (or auto-boot where supported)

Works on real hardware, flashcarts, and emulators

Multiple display modes:

Grid (horizontal / vertical)

Banner list

Coverflow

File associations per system

Cover art support

Material Design 3 & custom themes

Optional background music

📦 Requirements

Pico Loader

TWiLight Menu++ Virtual Console AddOn

Emulators installed in:

/_nds/TWiLightMenu/emulators/

GBA Support

GBARunner2 required for GBA games

🗂 Basic SD Layout
/LAUNCHER.nds
/_pico/
/_nds/TWiLightMenu/emulators/
/roms/


Pico Loader files must exist in /_pico.

🛠 Build
Requirements

WSL or MSYS2

BlocksDS toolchain

Compile
make


Output:

LAUNCHER.nds


Copy it to your SD card.
For DSpico, rename to _picoboot.nds.

🎮 Notes

Not all emulators support true auto-boot
(some open with the ROM pre-selected — this is expected)

Emulators are not bundled

FAT (flashcart) and SD (DSi) configs should not be mixed
