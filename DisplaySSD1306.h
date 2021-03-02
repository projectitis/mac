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
 * send the framebuffer to it via I2C.
 */

#pragma once
#ifndef _MAC_DISPLAYSSD1306H_
#define _MAC_DISPLAYSSD1306H_ 1

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
	 * Display base class for SSD1306 displays.
	 * A contructor overload should be implemented for each communication type. At the moment
	 * only I2C is implemented.
	 **/
	class DisplaySSD1306: public Display {
		public:
			
			/**
			 * Constructor for I2C.
			 * Note - constructor calls init, so you don't need to.
			 * @param	scl		Pin used for clock
			 * @param	sda		Pin used for data
			 * @param	px 		Scale factor from framebuffer to display. Normally 1:1 (pixelScale_1x1)
			 **/
			DisplaySSD1306(
				uint8_t scl = 19,
				uint8_t sda = 18,
				PixelScale px 	= pixelScale_1x1
			);

			/**
			 * Destructor
			 */
			~DisplaySSD1306( void );

			/**
			 * Update the framebuffer to the display
			 * @param	continuous	If true, will continuously refresh until stopRefresh is called
			 **/
			virtual void update(
				boolean continuous = false
			);
			
		protected:
			
			/**
			 * Initialise the display. Called from the constructor.
			 **/
			virtual void init( void );

			/**
			 * I2C methods
			 */
			__attribute__((always_inline)) inline void beginTransmission();
			__attribute__((always_inline)) inline void endTransmission();

			__attribute__((always_inline)) inline void writeCommand( uint8_t c );
  			__attribute__((always_inline)) inline void writeCommand( uint8_t *c, uint32_t n );

  			__attribute__((always_inline)) inline void writeData( uint8_t c );
  			__attribute__((always_inline)) inline void writeData( uint8_t *c, uint32_t n );
  			/**
  			 * Internal display methods
  			 */
  			__attribute__((always_inline)) inline void setDestinationArea( ClipRect *clipRect );
			__attribute__((always_inline)) inline void resetDestinationArea( void );
			
			/**
			 * Properties
			 **/
			uint8_t _scl, _sda;
			PixelScale _px;
			int8_t _i2caddr;
	};

} // namespace

#endif
