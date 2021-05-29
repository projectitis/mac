/**
 * Display library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 *
 * MIT LICENCE
 * -----------
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * This minimal version only includes enough code to set up the display and to
 * send the framebuffer to it via SPI and (optionally) DMA.
 *
 * ACKNOWLEDGEMENTS
 * This ILI9341 SPI-based display code has drawn on the contributions of many smart people.
 * Many thanks to:
 *		Paul Stoffregen / ILI9341_t3 - https://github.com/PaulStoffregen/ILI9341_t3
 *		Frank Bösing / ILI9341_t3DMA - https://github.com/FrankBoesing/ILI9341_t3DMA
 *		KurtE / ILI9341_t3n - https://github.com/KurtE/ILI9341_t3n
 *
 * TEENSY VERSION SUPPORT
 * The amount of memory will dictate how you use the framebuffer. The framebuffer supports
 * up-scaling (PixelScale) whereby the framebuffer can be smaller, but each pixel is up-scaled
 * onto the display. It appears blockier because you have less pixels, but the framebuffer
 * memory requirement is smaller. For example, PixelScale of 4 (pixelScale_4x4) means each pixel in the
 * framebuffer is mapped to 4x4 pixels on the display. 
 * PixelScale	pixelScale_1x1	Requires 154k RAM (320x240 x 16bit)		Teensy 3.5, 3.6
 * PixelScale	pixelScale_2x2	Requires 39k RAM (160x120 x 16bit)		Teensy 3.1, 3.2
 * PixelScale	pixelScale_4x4	Requires 10k RAM (80x60 x 16bit)		
 *
 * XXX: DMA SUPPORT - NOT YET IMPLEMENTED
 * To compile with or without DMA, see the GRAPHICS_USE_DMA define in Display.h
 * 
 * XXX: LINE BUFFER SUPPORT - PLANNED, NOT YET IMPLEMENTED
 * The mac graphics libraries are being rewritten to use a line (double) buffer instead of a frame buffer.
 * This means that the memory requirements will be much lower. For example, a full framebuffer at 1x1 pixel
 * scale requires 154k of RAM (at 16bpp) and a double-buffer would require 307k. A double line buffer requires
 * 3.2k (1% of the size). 
 * A line buffer for a 320x240 16bpp display is made up of:
 * 		front buffer: 24bpp x 320 pixels + 16bpp x 320 pixels
 * 		back buffer:  24bpp x 320 pixels + 16bpp x 320 pixels
 * 
 */

#pragma once
#ifndef _MAC_DISPLAYST7735H_
#define _MAC_DISPLAYST7735H_ 1

#include "Display.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Display base class for ST7735 displays.
	 * A contructor overload should be implemented for each communication type. At the moment
	 * only 3-wire SPI is implemented.
	 **/
	class DisplayST7735: public Display {
		public:
			
			/**
			 * Constructor for 4-wire SPI.
			 * Note - constructor calls init, so you don't need to.
			 * @param	cs		Pin used for Chip Select
			 * @param	dc		Pin used for Data Control (sometimes A0)
			 * @param	rst		Pin used for Reset (optional. 255=unused)
			 * @param	mosi	Pin used for MOSI communication (data out from master, sometimes SDA)
			 * @param	sclk	Pin used for clock (sck)
			 * @param	bklt	Pin used for backlight (optional. 255=unused)
			 * @param	px 		Scale factor from framebuffer to display. Normally 1:1 (pixelScale_1x1)
			 **/
			DisplayST7735(
				DisplaySize sz,	// displaySize_128x128
				uint8_t cs,		// 10
				uint8_t dc,		// 15
				uint8_t rst		= 4,
				uint8_t mosi	= 11,
				uint8_t sclk	= 13,
				uint8_t bklt	= 6,
				PixelScale px 	= pixelScale_1x1
			);

			/**
			 * Destructor
			 */
			~DisplayST7735( void );

			/**
			 * Flip the front and back buffers, and trigger the drawing of the back buffer
			 * to the display. Moves the front buffer to the next line.
			 */
			void flip();
			
			/**
			 * Initialise the display. Called from the constructor.
			 **/
			void init( void );
			
		protected:

			/**
			 * @brief Pixel offset
			 */
			int8_t _xoffset = 0;
			int8_t _yoffset = 0;
			
			/**
			 * @brief Get the Init Commands list
			 * Override this in subclasses to porvide custom init commands
			 * @return const uint8_t* 
			 */
			virtual const uint8_t* getInitCommands();
			
			/**
			 * Display controls
			 **/
			__attribute__((always_inline)) inline void setDestinationLine( LineBufferData *data );

	};

} // namespace

#endif
