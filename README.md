# μac / mac

![mac logo](/logo.png?raw=true "mac")

μac, or mac, stands for "Microprocessor App Creator". mac is a project that enables creating beautiful and useful apps on the Teensy microprocessor, but hopefully is generic enough to be ported to other microprocessor boards in the future. The various individual libraries that make up mac (such as display and drawing libraries) might also be useful in other projects.

This project has a software component (the libraries) and a hardware component (a Teensy-based handheld). As well as the software, eventually the plans/schematics will be released to enable you to build your own Teensy-based handheld that will run mac.

## Current status

 - **2019-04-14** Many bitmap and vector features implemented. Working on GUI components.
 - **2018-04-24** Very much a work in progress. Only display components currently implemented.

## Prerequisites

Only tested on Teensy 3.6 but should work on T3.5.

#### Minimum requirements: 

 1. 192kB of RAM (Framebuffer used by display libraries)
 2. 512kB of Flash (Storage for music and graphics)

#### Recommended:

 - Teensy 3.6 with 256kB RAM and 1MB Flash

## Component status

mac has lofty goals. The mac project has the following planned components:

 -  Display adapters
	 - **ILI9341 4-wire SPI - complete (2018-04-24)**
	 - ILI9488 4-wire SPI with framebuffer upscaling (0%)
	 - Vertical sync/Tearing Effect (TE) support (0%)
 - Graphics
	 - **Basic framebuffer operations (copy, clear etc) - complete (2018-04-24)**
	 - Sprite blitting: RGB 565 (90%)
	 - Sprite blitting: RGBA 5658 (10%)
	 - Sprite blitting: RGBA (0%)
	 - Sprite animation (10%)
	 - Fast antialiased vector drawing: Thin lines (100%), Triangles (100%), Complex ploygons (50%), Thick lines (0%)
	 - Double-buffer support (0%)
 - XM audio player
	 - Loading and playback - (90% - errors. Only very few mods work)
	 - Loading and saving to SD card (0%)
- GUI
	- Windows, input and output components (5%)
- Input
	- Analogue joy (50%)
	- Digital joy (50%)
	- Buttons (shoulder, ABXY, misc) (10%)
	- Touch screen (0%)
	- Accellerometer (0%)
	- Various control schemes that support from minimal 2 button input to full gamut (0%)
- Output
	- Display (25%)
	- Speakers and headphone (25%)
	- Vibration (0%)
- Game creator
	- Load, edit and save game maps and backgrounds (0%)
	- Ability to create crunched, play-only game file (0%)
- Game player
	- Load and play games (0%)
	- Ability to swap between edit and play (0%)
	- Save progress to SD (0%)
	- Wifi multi-player (0%)
- Other hardware
	- PSP battery power supply (25%)
	- CNC aluminium shell with wooden accents (0%)

## Licence

Both the software and hardware components of this project are released under the **[MIT licence](https://en.wikipedia.org/wiki/MIT_License)**, except for the various parts by other authors that may have their own licences (for example, libxm is under the WTFPL).

## Acknowledgements

Code from many authors has contributed to mac. This project would not be possible without them. These include:

#### Display

* **[Paul Stoffregen](mailto:)** - [ILI9341_t3](https://github.com/PaulStoffregen/ILI9341_t3)
* **[KurtE](mailto:)** - [ILI9341_t3n](https://github.com/KurtE/ILI9341_t3n)
* **[Frank Boesing](mailto:)** - [ILI9341_t3DMA](https://github.com/FrankBoesing/ILI9341_t3DMA)

#### Graphics and animation

* **Maxim "mcseem" Shemanarev** - [Anti-grain geometry](https://en.wikipedia.org/wiki/Anti-Grain_Geometry)
* **[Warren Moore](wm@warrenmoore.net)** - [AHEasing](https://github.com/warrenm/AHEasing)
* **[Rosetta Code](https://rosettacode.org/wiki/Rosetta_Code)** - [Wu's algorithm](https://rosettacode.org/wiki/Xiaolin_Wu%27s_line_algorithm)

#### Audio

* **[Romain "Artefact2" Dalmaso](mailto:artefact2@gmail.com)** - [libxm](https://github.com/Artefact2/libxm)

#### Support

* **[Teensy support forums](https://forum.pjrc.com/)**
* **[stackedit Markdown editor for this readme!](https://stackedit.io/)**

#### Hardware

* **[Teensy microprocessor boards](https://www.pjrc.com/teensy/)**
* **[BuyDisplay](https://www.buydisplay.com)** - [ILI9341 TFT module](https://www.buydisplay.com/default/3-2-inch-capacitive-touchscreen-240x320-tft-lcd-module-display) - [ILI9488 TFT module](https://www.buydisplay.com/default/lcd-3-5-inch-320x480-tft-display-module-optl-touch-screen-w-breakout-board)