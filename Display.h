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

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Pixel scaling.
	 */
	enum PixelScale {
	    pixelScale_1x1 		= 0,
	    pixelScale_2x2 		= 1,
	    pixelScale_4x4 		= 2,
		pixelScale_8x8 		= 3
	};

	/**
	 * Holds the pixel data and state of the line buffer. There
	 * are two of these - one for the sront and one for the back.
	 */
	typedef struct {

		// The pixel data as 32bit words. There is a storage overhead of 1 byte
		// per pixel, but this is made up for by the faster math this allows.
		uint32_t* pixels;

		// The y coord of the linebuffer
		uint16_t y;

		// The start x coord (normally 0)
		uint16_t x;

		// The end x coord (normally width-1)
		uint16_t x2;

	} LineBufferData;

	/**
	 * Display base class.
	 * This minimal version of the display class only includes enough code to
	 * set up the display and to draw pixels to it. Do not use this class directly.
	 * Use one of the derived classes specifically for your display. Currently:
	 *		DisplayILI9341 (4-wire SPI)
	 **/
	class Display {
		public:

			/**
			 * Destroy the Display object
			 */
			virtual ~Display( void ){}

			/**
			 * Initialise the display. usually called immediately after the constructor.
			 **/
			virtual void init( void ) = 0;

			/**
			 * Set the region of the display to draw to. This resets the line buffer to
			 * start of the region. The backbuffer is not affected until the next flip.
			 * @param x 	The x coordinate of the region
			 * @param y 	The y coordinate of the region
			 * @param w		The width of the region
			 * @param h 	The height of the region
			 */
			virtual void set( uint16_t x, uint16_t y, uint16_t w, uint16_t h );

			/**
			 * Set the region of the display to draw to. This resets the line buffer to
			 * start of the region. The backbuffer is not affected until the next flip.
			 * @param rect 	A rect describing the region
			 */
			virtual void set( ClipRect* rect );

			/**
			 * Resets the region without changing it. This resets the line buffer to
			 * start of the region. The backbuffer is not affected until the next flip.
			 */
			virtual void reset();

			/**
			 * Flip the front and back buffers, and trigger the drawing of the back buffer
			 * to the display. Moves the front buffer to the next line.
			 */
			virtual void flip();
			
			/**
			 * Width of the display in pixels (unscaled). Read-only.
			 **/
			int16_t width;
			
			/**
			 * Height of the display in pixels (unscaled). Read-only.
			 **/
			int16_t height;

			/**
			 * Rect describing size of the display (scaled to pixelscale). Read-only.
			 */
			ClipRect rect;

			/**
			 * Rect describing the drawing region (scaled to pixelscale). Modified by set
			 */
			ClipRect region;

			/**
			 * Fill the current line with a color
			 **/
			inline void clear( color888 c ) {
				uint16_t i = 0;
				while ( i < width ) data[frontIndex].pixels[i++] = c;
			}
			
			/**
			 * Write a pixel to the current line. Overwrites underlying pixel (if any)
			 * @param  c 	The color (pixel) in RGB 888 format
			 * @param  x 	The X coordinate
			 */
			inline void pixel( color888 c, int16_t x ) {
				data[frontIndex].pixels[x] = c;
			}

			/**
			 * read a pixel from the current line (front buffer)
			 * @param  x 	The X coordinate
			 * @return 		The color (pixel) in RGB 888 format
			 */
			inline color888 pixel( int16_t x ) {
				return data[frontIndex].pixels[x];
			}

			/**
			 * Blend a pixel to the current line using float alpha 0.0 - 1.0
			 * @param  c 	The color (pixel) in RGB 888 format
			 * @param  a 	The alpha (0.0 - 1.0)
			 * @param  x 	The X coordinate
			 */
			inline void blend( color888 c, alpha a, int16_t x ) {
				data[frontIndex].pixels[x] = alphaBlend8888( data[frontIndex].pixels[x], c, alpha8bit( a ) );
			}

			/**
			 * Blend a pixel to the current line using integer alpha 0 - 255
			 * @param  c 	The color (pixel) in RGB 888 format
			 * @param  a 	The alpha (0 - 255)
			 * @param  x 	The X coordinate
			 */
			inline void blend( color888 c, uint8_t a, int16_t x ) {
				data[frontIndex].pixels[x] = alphaBlend8888( data[frontIndex].pixels[x], c, a );
			}

			/**
			 * The pixel format of the display
			 */
			PixelFormat pixelFormat;

			/**
			 * Pointer to the pixel data
			 */
			LineBufferData data[2];

			/**
			 * Index to the active (front) data buffer for reading and writing
			 */
			uint8_t frontIndex = 0;

			/**
			 * Index to the active data buffer for transfering to the hardware. I.e: `lineBuff->data[ lineBuff->backIndex ][ x ]`
			 */
			uint8_t backIndex = 1;

			/**
			 * Flag that can be used by code that writes the back buffer to hardware. It is set to 1 (ready to draw back-buffer) when
			 * the buffer is flipped, and can be set back to 0 (not ready) when back buffer has been drawn to the display.
			 */
			uint8_t ready = 0;
	};

} // namespace

#endif
