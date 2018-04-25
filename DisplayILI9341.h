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
 * This ILI9341 SPI-based display code has had many smart people contribute to it.
 * Many thanks to:
 *		Paul Stoffregen / ILI9341_t3 - https://github.com/PaulStoffregen/ILI9341_t3
 *		Frank Bösing / ILI9341_t3DMA - https://github.com/FrankBoesing/ILI9341_t3DMA
 *		Kurt E / ILI9341_t3n - https://github.com/KurtE/ILI9341_t3n
 *
 * TEENSY VERSION SUPPORT
 * Because of the memory requirements of a framebuffer (154k) this library will only
 * work with Teensy 3.5 (192k RAM) and Teensy 3.6 (512k RAM).
 *
 * DMA SUPPORT
 * To compile with or without DMA, see the GRAPHICS_USE_DMA define in Display.h
 */
 
#ifndef _MAC_DISPLAYILI9341H_
#define _MAC_DISPLAYILI9341H_ 1

#include "Display.h"

/**
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Display base class for ILI9341 displays.
	 * A contructor overload should be implemented for each communication type. At the moment
	 * only 4-wire SPI is implemented.
	 **/
	class DisplayILI9341: public Display {
		public:
			
			/**
			 * Constructor for 4-wire SPI.
			 * Note - constructor calls init, so you don't need to.
			 * @param	cs		Pin used for Chip Select
			 * @param	dc		Pin used for Data Control
			 * @param	rst		Pin used for Reset (optional. 255=unused)
			 * @param	mosi	Pin used for MOSI communication (data out from master)
			 * @param	sclk	Pin used for clock
			 * @param	miso	Pin used for MISO communication (data out from slave)
			 * @param	bklt	Pin used for backlight (optional. 255=unused)
			 **/
			DisplayILI9341(
				uint8_t cs,		// 10
				uint8_t dc,		// 15
				uint8_t rst		= 4,
				uint8_t mosi	= 11,
				uint8_t sclk	= 13,
				uint8_t miso	= 12,
				uint8_t bklt	= 6
			);

			/**
			 * Update the framebuffer to the display
			 * @param	buffer	A pointer to the buffer
			 * @param	rect	If not NULL, the portion of the buffer to refresh
			 * @param	continuous	If true, will continuously refresh until stopRefresh is called
			 **/
			void update(
				uint16_t* buffer,
				BufferRect* rect = NULL,
				boolean continuous = false
			);
			
			/**
			 * Stop the display from continuously refreshing
			 **/
			void stopRefresh( void );
			
			/**
			 * Wait until the current refresh is complete. Not recommended if you have other
			 * (usually non-graphics) code that shouldn't be delayed!
			 **/
			void waitForRefresh( void );
			
			/**
			 * Turn the backlight on or off. If the backlight pin is set, the backlight
			 * is turned on automatically during init/construction. If you want it off, call
			 * setbacklight(0) after construction.
			 * @param	state	0 = off, 1 = on
			 **/
			void setBacklight( boolean state );
			
		protected:
			
			/**
			 * Initialise the display. Called from the constructor.
			 **/
			void init( void );
			
			/**
			 * Display controls
			 **/
			__attribute__((always_inline)) inline void setDestinationArea( ClipRect *clipRect );
			__attribute__((always_inline)) inline void resetDestinationArea( void );
			
			void waitFifoNotFull( void );
			void waitFifoEmpty( void );
			
			__attribute__((always_inline)) inline void waitTransmitComplete( void );
			__attribute__((always_inline)) inline void waitTransmitComplete( uint32_t mcr );
			
			__attribute__((always_inline)) inline void writeCommand(uint8_t c);
			__attribute__((always_inline)) inline void writeCommand_last(uint8_t c);
			
			__attribute__((always_inline)) inline void writeData8(uint8_t c);
			__attribute__((always_inline)) inline void writeData8_last(uint8_t c);
			
			__attribute__((always_inline)) inline void writeData16(uint16_t d);
			__attribute__((always_inline)) inline void writeData16_last(uint16_t d);
			
			/**
			 * Properties
			 **/
			uint8_t _rst;
			uint8_t _cs, _dc;
			uint8_t _pcs_data, _pcs_command;
			uint8_t _miso, _mosi, _sclk;
			uint8_t _bklt;
	};

} // namespace

#endif
