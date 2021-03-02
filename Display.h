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
 
#pragma once
#ifndef _MAC_DISPLAYH_
#define _MAC_DISPLAYH_ 1

#include "Common.h"
#include "FrameBuffer.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Make use of DMA for framebuffer transfer to the display. 0 or 1
	 **/
	#define DISPLAY_USE_DMA 1

	/**
	 * Pixel scaling.
	 */
	enum PixelScale {
	    pixelScale_1x1 		= 1,
	    pixelScale_2x2 		= 2,
	    pixelScale_4x4 		= 4
	};

	/**
	 * Display base class.
	 * This minimal version of the display class only includes enough code to
	 * set up the display and to send a framebuffer to it via SPI and (optionally) DMA.
	 * Do not use this class directly. Use one of the derived classes specifically
	 * for your display. Currently:
	 *		DisplayILI9341 (4-wire SPI)
	 **/
	class Display {
		public:
			
			/**
			 * Initialise the display. usually called immediately after the constructor.
			 **/
			virtual void init( void );
			
			/**
			 * Update the framebuffer to the display
			 * @param	continuous	If true, will continuously refresh until stopRefresh is called
			 **/
			virtual void update(
				boolean continuous = false
			);

			/**
			 * Update an area of the framebuffer to the display
			 * @param	rect		The portion of the buffer to refresh
			 **/
			virtual void updateRect(
				ClipRect* rect
			);
			
			/**
			 * Width of the display in pixels
			 **/
			int16_t width;
			
			/**
			 * Height of the display in pixels
			 **/
			int16_t height;

			/**
			 * The pixel format of the display
			 */
			PixelFormat pixelFormat;

			/**
			 * The framebuffer
			 **/
			FrameBuffer* framebuffer;
	};

} // namespace

#endif
