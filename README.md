# Pico Launcher
This repository contains Pico Launcher, which is a front-end for [Pico Loader](https://github.com/LNH-team/pico-loader).

![Horizontal display mode with custom theme](docs/images/HorizontalCustom.png)
![Banner list display mode](docs/images/List.png)
![Coverflow display mode](docs/images/Coverflow.png)

## Features
- Can load homebrew and retail games using [Pico Loader](https://github.com/LNH-team/pico-loader).
- Various display modes
    - Horizontal and vertical icon grid
    - Banner list
    - Coverflow
- [File associations](docs/FileAssociations.md)
- [Covers](docs/Covers.md)
- [Material Design 3 and custom themes](docs/Themes.md)
- Support for background music (see [Themes](docs/Themes.md))

General usage documentation can be found here: [Usage](docs/Usage.md).

## Setup & Configuration
We recommend using WSL (Windows Subsystem for Linux), or MSYS2 to compile this repository.
The steps provided will assume you already have one of those environments set up.

1. Install [BlocksDS](https://blocksds.skylyrac.net/docs/setup/options/)

## Compiling

1. Run `make`

The launcher can be found in the root directory under the name `LAUNCHER.nds`.

2. Copy `LAUNCHER.nds` to your SD card.
    - If you are using DSpico, rename to `_picoboot.nds` and place it in the root of your SD card.
3. Copy the `_pico` pico folder to the root of your SD card.

> [!NOTE]
> To use Pico Launcher, the Pico Loader files (`aplist.bin`, `savelist.bin`, `picoLoader7.bin` and `picoLoader9.bin`) must also be present in the `/_pico` folder on your SD card.

For DSpico the final directory structure will look like this:
```
.
├── _pico
│   ├── themes
│   │   ├── material
│   │   │   └── theme.json
│   │   └── raspberry
│   │       ├── bannerListCell.bin
│   │       ├── bannerListCellPltt.bin
│   │       ├── bannerListCellSelected.bin
│   │       ├── bannerListCellSelectedPltt.bin
│   │       ├── bottombg.bin
│   │       ├── gridcell.bin
│   │       ├── gridcellPltt.bin
│   │       ├── gridcellSelected.bin
│   │       ├── gridcellSelectedPltt.bin
│   │       ├── scrim.bin
│   │       ├── scrimPltt.bin
│   │       ├── theme.json
│   │       └── topbg.bin
│   ├── aplist.bin
│   ├── savelist.bin
│   ├── picoLoader7.bin
│   └── picoLoader9.bin
└── _picoboot.nds
```
Note: If you want to play DSiWare on the DSpico, additional files are required. See the [Pico Loader](https://github.com/LNH-team/pico-loader) readme for more information.

## License

Icons by [icons8](https://icons8.com/)

This project is licensed under the Zlib license. For details, see `LICENSE.txt`.

Additional licenses may apply to the project. For details, see the `license` directory.

## Contributors
- [@Gericom](https://github.com/Gericom)
- [@XLuma](https://github.com/XLuma)
- [@Dartz150](https://github.com/Dartz150)
- [@lifehackerhansol](https://github.com/lifehackerhansol)

---

# Pico Launcher

Pico Launcher is a **Nintendo DS front-end for Pico Loader** that supports **direct ROM handoff to DS emulators**, using the same **Virtual Console AddOn architecture as TWiLight Menu++**.

Instead of manually launching an emulator and browsing for a ROM, pico-launcher can:

- Automatically launch the correct emulator  
- Pre-select the ROM you clicked  
- *(On supported emulators)* directly boot the game  

This makes pico-launcher especially useful on **Ace3DSX / flashcarts**, as well as **DSi / 3DS** environments.

---

## ✨ Features

- Front-end for **Pico Loader**
- Direct ROM handoff using **TWiLight Menu++ Virtual Console AddOn**
- Works on real hardware and emulators
- Multiple display modes:
  - Horizontal / vertical icon grid
  - Banner list
  - Coverflow
- File associations per system
- Cover art support
- Material Design 3 and custom themes
- Background music support

---

## 🧠 Direct ROM Handoff Explained

pico-launcher mirrors how **TWiLight Menu++** launches VC emulators:

- ROM paths are written to a **handoff file**
- The emulator is launched
- If supported, the emulator auto-loads the ROM

⚠️ **Important:**  
Not all DS emulators support true “auto-boot”.  
Some will open with the ROM **pre-selected** instead.

This is an **emulator limitation**, not a pico-launcher issue.

---

## 🎮 Supported Emulator Behavior

### Direct Auto-Boot
- **SNES** – SNEmulDS (legacy TWL path build)
- **Genesis / Mega Drive** – PicoDriveTWL

### Stable Launch-Only (Expected)
- NES
- GB / GBC
- Game Gear / SMS / SG-1000
- TurboGrafx-16 / PC-Engine
- WonderSwan
- Neo Geo Pocket
- Atari 2600 / 5200 / 7800
- ColecoVision
- Intellivision
- CPC and other legacy systems

This behavior **matches TWiLight Menu++ itself**.

---

## 📦 What’s Included

- Built `launcher.nds`
- Example setup archives (FAT + SD variants)
- VC AddOn-compatible configuration
- ARM9 heap / linking fixes
- Updated routing logic for flashcarts and DSi

**Emulators are not bundled.**

---

## 🔗 Required / Recommended Dependencies

### 1️⃣ TWiLight Menu++ Virtual Console AddOn (REQUIRED)

pico-launcher relies on the same VC AddOn used by TWiLight Menu++.

Download TWiLight Menu++ (includes VC AddOn):  
https://github.com/DS-Homebrew/TwilightMenu/releases

Emulators must be installed in:
```
/_nds/TWiLightMenu/emulators/
```

---

### 2️⃣ GBARunner2 (REQUIRED for GBA)

GBA titles are launched using **GBARunner2**.

Download GBARunner2:  
https://github.com/Gericom/GBARunner2/releases

Recommended file:
```
GBARunner2_arm7dldi_ds.nds
```

Place it in:
```
/_nds/TWiLightMenu/emulators/
```

---

### 3️⃣ Pico Loader + DSi NAND Dump (STRONGLY RECOMMENDED)

For best compatibility (especially **DSiWare** and TWL features):

- Install **pico-loader**
- Provide a valid **DSi NAND dump**

Used for:
- DSiWare launching
- TWL-mode services
- Improved compatibility on DSi / 3DS

Without these:
- Standard `.nds` homebrew still works
- DSiWare and advanced TWL features will not

---

## 🗂 Example SD Card Layout

```
/launcher.nds
/_pico/settings.json
/_nds/TWiLightMenu/emulators/
/_nds/pico-launcher/handoff/lastrom_snes.txt
/roms/nes/
/roms/snes/
/roms/gb/
/roms/gba/
```

⚠️ Handoff files **must exist** (can be empty):
```
/_nds/pico-launcher/handoff/lastrom_snes.txt
```

---

## ⚙️ Setup & Configuration

- FAT (flashcart) and SD (DSi) configurations are provided separately
- **Do not mix FAT and SD configs**
- File associations are system-specific and intentional

---

## 🛠 Compiling

### Requirements
- WSL (recommended) or MSYS2
- BlocksDS toolchain

### Build
```bash
make
```

Output:
```
LAUNCHER.nds
```

---

## 🧪 Tested On

- Ace3DSX flashcart
- melonDS *(emulator limitations expected)*
- Real Nintendo DS / DSi hardware

---

## 🚀 Future Plans

- Optional YAML configuration support
- Emulator-specific autoboot patches
- Improved fallback handling
- Expanded documentation

---

## 🙏 Credits

- Pico Launcher / Pico Loader developers
- TWiLight Menu++ & DS-Homebrew team
- GBARunner2 contributors
- blocksds / libnds contributors

---

## 📄 License

This project is licensed under the **zlib license**.  
See `LICENSE.txt` for details.
