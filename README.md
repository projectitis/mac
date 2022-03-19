# μac / mac

![mac logo](/logo.png?raw=true "mac")

Microprocessor App Creator (μac, or mac) is an SDK for rapid development of graphical apps on the Teensy microprocessor, but hopefully is generic enough to be ported to other microprocessor boards in the future. The various individual libraries that make up mac (such as display and drawing libraries) might also be useful in other projects.

This project has a software component (the libraries) and a future hardware component (a Teensy-based handheld). As with the software, the plans/schematics will be released to enable you to build your own Teensy-based handheld that will run applications developed using mac.

## DEVELOPER NOTES 2022-03-19
### TODO next
- Radial gradient
- Sprite stamping
- Sprite animation
- Sprite rotation
- Box class enhancements (border)
- Simple vector rendering
- Attempt to measure RAM usage
- ESP32 support

## Change history
 - **2022-03-19** Added masks (and inverse masks)
 - **2022-03-18** Refactored memory pooling and linked lists to use templates classes
 - **2022-03-18** Added ability to set any size buffer (from single line to full frame)
 - **2022-03-17** Filters (tint, HSV, colormask, lighten/darken) and variable height framebuffer
 - **2022-03-16** Working linear gradient.
 - **2022-03-10** Refactored line buffer and display implementation.
 - **2021-09-19** Reorganised codebase into subfolders.
 - **2021-05-25** Completely refactored to stage and display list approach (like Flash).
 - **2021-04-27** Completely rewritten to convert framebuffer to linebuffer.
 - **2019-04-14** Many bitmap and vector features implemented. Working on GUI components.
 - **2018-04-24** Very much a work in progress. Only display components currently implemented.

## Prerequisites

Only tested on Teensy 3.6 but should work on T3.5 and other lower-memory teensy devices.

#### Minimum requirements:

 1. 16kB RAM
 2. 40kB of Flash (Storage for music and graphics etc)

The library uses a linebuffer (double-buffered) for rendering graphics to the screen. This requires 6 * display_width bytes of RAM. E.g. A 320 width displays scaled to 4x4 pixels (resulting in 80 pixels) requires a 480 byte linebuffer.

#### Recommended:

 - Teensy 3.6 with 256kB RAM and 1MB Flash

## Component status

mac has lofty goals. The mac project has the following planned components:

-  Display adapters
	 - **ILI9341 4-wire SPI (100%)**
	 - **Support for many pixel formats from 24bit color down to 1-bit mono (100%)**
	 - **Line buffer implementation for very low memory consumption (100%)**
	 - **Pixel scaling (1x1, 2x2, 4x4 and 8x8 pixels) (100%)**
	 - Double-buffered DMA support for faster rendering (20%)
	 - Vertical sync/Tearing Effect (TE) support (0%)
- Graphics
    - **Stage and display list based graphics engine**
    - **Tilemap-based sprites (100%)**
	- **Linear gradients at any angle with up to 255 stops (100%)**
	- Sprite animation (10%)
	- Fast antialiased vector drawing: Thin lines, Triangles, Complex polygons, Thick lines
- XM audio player
	 - Loading and playback - (90% - errors. Only very few mods work)
	 - Loading and saving to SD card (0%)
- GUI
	- Windows, input and output components (5%)
	- Textarea (50%)
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