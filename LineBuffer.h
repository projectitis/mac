/**
 * GUI library for "mac/μac"
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
#ifndef _MAC_linebuffer_H_
#define _MAC_linebuffer_H_ 1

#include "Common.h"
#include <SD.h>

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Holds the pixel data and state of the line buffer. There
	 * are two of these - one for the sront and one for the back.
	 */
	typedef struct {

		// The pixel data
		union Data{
			uint8_t* data8;
			uint16_t* data16;
			uint32_t* data32;
		} pixels;

		// The y coord of the linebuffer
		uint16_t y;

		// The start x coord (normally 0)
		uint16_t x0;

		// The end x coord (normally width-1)
		uint16_t x1;

	} LineBufferData;
	
	/**
	 * LineBuffer class (single-line frame buffer) that handles different
	 * pixel formats (8, 16 and 32 bit pixels). Double-buffered.
	 */
	class LineBuffer {
		
		public:
			/**
			 * Constructor
			 */
			LineBuffer( PixelFormat pixelFormat, uint16_t w );

			/**
			 * Destructor
			 */
			~LineBuffer();

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
			 * Flag that can be used by code that writes the back buffer to hardware. It can be set to 1 when backbuffer has been
			 * processed. Note that flipping the buffer does not check this flag and sets it to 1, so check this manually before flipping.
			 */
			uint8_t ready = 1;

			/**
			 * Pixel format of the output
			 */
			PixelFormat pixelFormat;

			/**
			 * Width of buffer
			 */
			uint16_t width;

			/**
			 *  ####
			 * ##  ##
			 *  ####
			 * ##  ##
			 *  ####
			 */
			
			/**
			 * Write an 8bit pixel to the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void writePixel( uint8_t c, uint16_t x ){
				data[frontIndex].pixels.data8[x] = c;
			}

			/**
			 * Write an 8bit pixel to the buffer with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void writePixelClip( uint8_t c, uint16_t x ){
				if (x>=width) return;
				writePixel( c, x );
			}

			/**
			 * Read an 8bit pixel from the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void readPixel( uint16_t x, uint8_t& c ){
				c = data[frontIndex].pixels.data8[x];
			}

			/**
			 * Read an 8bit pixel from the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void readPixelClip( uint16_t x, uint8_t& c ){
				if (x>=width) c = 0;
				else readPixel( x, c );
			}

			/**
			 * Fill the buffer with an 8bit color
			 **/
			inline void clear8( uint8_t c ){
				uint16_t x = width;
				while (x) data[frontIndex].pixels.data8[--x] = c;
			}

			/**
			 *  ##   ####
			 * ###  ##
			 *  ##  #####
			 *  ##  ##  ## 
			 * ####  ####
			 */

			/**
			 * Write a 16bit pixel to the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void writePixel( uint16_t c, uint16_t x ){
				data[frontIndex].pixels.data16[x] = c;
			}

			/**
			 * Bleand a 16bit pixel to the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (5 bits, 0-31)
			 * @param  x 	The x coord into the buffer
			 */
			inline void blendPixel( uint16_t c, uint8_t a, uint16_t x ){
				data[frontIndex].pixels.data16[x] = alphaBlend5565( c, data[frontIndex].pixels.data16[x], a );
			}

			/**
			 * Write a 16bit pixel to the buffer with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void writePixelClip( uint16_t c, uint16_t x ){
				if ( x >= width ) return;
				writePixel( c, x );
			}

			/**
			 * Bleand a 16bit pixel to the buffer with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (5 bits, 0-31)
			 * @param  x 	The x coord into the buffer
			 */
			inline void blendPixelClip( uint16_t c, uint8_t a, uint16_t x ){
				if ( x >= width ) return;
				blendPixel( c, a , x );
			}

			/**
			 * read a 16bit pixel from the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void readPixel( uint16_t x, uint16_t& c ){
				c = data[frontIndex].pixels.data16[x];
			}

			/**
			 * read a 16bit pixel from the buffer with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the buffer
			 */
			inline void readPixelClip( uint16_t x, uint16_t& c ){
				if ( x >= width ) c = 0;
				else readPixel( x, c );
			}

			/**
			 * Fill the buffer with a 16bit color
			 **/
			inline void clear16( uint16_t c ){
				uint16_t x = width;
				while (x) data[frontIndex].pixels.data16[--x] = c;
			}

			/**
			 *  ####    ####
			 * ##  ##  ##  ##
			 *    ##      ##
			 * ##  ##   ##
			 *  ####   ######
			 */

			/**
			 * Write a 32bit pixel to the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void writePixel( uint32_t c, uint16_t x ){
				data[frontIndex].pixels.data32[x] = c;
			}

			/**
			 * Blend a 32bit pixel to the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  x 	The index into the buffer
			 */
			inline void blendPixel( uint32_t c, uint8_t a, uint16_t x ){
				data[frontIndex].pixels.data32[x] = alphaBlend8888( c, data[frontIndex].pixels.data32[x], a );
			}

			/**
			 * Write a 32bit pixel to the buffer  with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void writePixelClip( uint32_t c, uint16_t x ){
				if ( x >= width ) return;
				writePixel( c, x );
			}

			/**
			 * Blend a 32bit pixel to the buffer with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  x 	The x coord into the buffer
			 */
			inline void blendPixelClip( uint32_t c, uint8_t a, uint16_t x ){
				if ( x >= width ) return;
				blendPixel( c, a, x );
			}

			/**
			 * read a 32bit pixel from the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void readPixel( uint16_t x, uint32_t& c ){
				c = data[frontIndex].pixels.data32[x];
			}

			/**
			 * read a 32bit pixel from the buffer with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void readPixelClip( uint16_t x, uint32_t& c ){
				if ( x >= width ) c = 0;
				else readPixel( x, c );
			}

			/**
			 * Fill the buffer with a 32bit color
			 **/
			inline void clear32( uint32_t c ){
				uint16_t x = width;
				while (x) data[frontIndex].pixels.data32[--x] = c;
			}

			/**
			 *  ####     ##   ##
			 * ##  ##    ##  ###
			 * ##  ##   ##    ##
			 * ##  ##  ##     ##
			 *  ####  ##    ######
			 */
			
			/**
			 * Write a 1bit pixel to the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void writePixelMono( uint8_t c, uint16_t x ){
				uint8_t m = 128 >> (x & 0b11);		// Which bit (mask)
				uint16_t b = x >> 3;				// Which byte
				if (c) data[frontIndex].pixels.data8[b] |= m;
				else data[frontIndex].pixels.data8[b] &= ~m;
			}

			/**
			 * Blend a 1bit pixel to the buffer by index (no bounds checking). This will
			 * write the pixel if it is ON and alpha is 0.5 or greater. Otherwise the pixel is
			 * not drawn.
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  x 	The x coord into the buffer
			 */
			inline void blendPixelMono( uint8_t c, alpha a, uint16_t x ){
				if (!c || (a<0.5f)) return;
				uint8_t m = 128 >> (x & 0b11);		// Which bit (mask)
				uint16_t b = x >> 3;				// Which byte
				data[frontIndex].pixels.data8[b] |= m;
			}

			/**
			 * Write a 1bit pixel to the buffer with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void writePixelMonoClip( uint8_t c, uint16_t x ){
				if ( x >= width ) return;
				writePixelMono( c, x );
			}

			/**
			 * Blend a 1bit pixel to the buffer with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  x 	The x coord into the buffer
			 */
			inline void blendPixelMonoClip( uint8_t c, uint8_t a, uint16_t x ){
				if ( x >= width ) return;
				blendPixelMono( c, a, x );
			}

			/**
			 * read a 1bit pixel from the buffer (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The x coord into the buffer
			 */
			inline void readPixelMono( uint16_t x, uint8_t& c ){
				uint8_t m = 128 >> (x & 0b11);		// Which bit (mask)
				uint16_t b = x >> 3;				// Which byte
				c = (data[frontIndex].pixels.data8[b] >> m) & 0b1;
			}

			/**
			 * read a 1bit pixel from the buffer with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  s 	The s coord into the buffer
			 */
			inline void readPixelMonoClip( uint16_t x, uint8_t& c ){
				if ( x >= width ) c = 0;
				else readPixelMono( x, c );
			}

			/**
			 * Fill the buffer with a 1bit color
			 **/
			inline void clearMono( uint8_t c ){
				uint16_t x = (uint32_t)((width+7)/8);
				if ( c > 0 ) c = 255;
				while (x) data[frontIndex].pixels.data8[--x] = c;
			}

			/**
			 *  ####   ##  ##
			 * ##  ##  ##  ##
			 * ######  ##  ##
			 * ##  ##  ##  ##
			 * ##  ##  ##  ##
			 */

			/**
			 * Fill the buffer with a color
			 **/
			void clear( color888 c );
			
			/**
			 * Write a pixel to the buffer. Checks the pixel format and uses the
			 * correct method. This method is slow. If you are setting a lot of pixels, use
			 * the pixel-format specific method, or hit the buffer directly.
			 * @param  c 	The color (pixel) in RGB 888 format
			 * @param  x 	The X coordinate
			 */
			void pixel( color888 c, int16_t x );

			/**
			 * Blend a pixel to the buffer. Checks the pixel format and uses the
			 * correct method. This method is slow. If you are blending a lot of pixels, use
			 * the pixel-format specific method, or hit the buffer directly.
			 * @param  c 	The color (pixel) in RGB 888 format
			 * @param  a 	The alpha (0.0 - 1.0)
			 * @param  x 	The X coordinate
			 */
			void blend( color888 c, alpha a, int16_t x );

			/**
			 * read a pixel from the buffer in RGB 888 format. Checks the pixel format
			 * and uses the correct method. This method is slow. If you are reading a lot of
			 * pixels, use the pixel-format specific method, or hit the buffer directly.
			 * @param  x 	The X coordinate
			 * @return 		The color (pixel) in RGB 888 format
			 */
			color888 pixel( int16_t x );

			/**
			 * Flip the framebuffers between front and back
			 */
			inline void flip() {
				backIndex = frontIndex;
				frontIndex ^= 1;
				ready = 1;
			}

			/**
			 * Flip the framebuffers between front and back, specifying properties of the next line
			 */
			inline void flip( uint16_t y, uint16_t x0, uint16_t x1 ) {
				backIndex = frontIndex;
				frontIndex ^= 1;
				data[frontIndex].y = y;
				data[frontIndex].x0 = x0;
				data[frontIndex].x1 = x1 || ( width - 1 );
				ready = 1;
			}

	};
	
} // namespace

#endif