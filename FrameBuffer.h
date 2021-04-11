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
#ifndef _MAC_FRAMEBUFFERH_
#define _MAC_FRAMEBUFFERH_ 1

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
	 * Framebuffer class that can handle different pixel formats (8, 16 and 32 bit pixels)
	 */
	class FrameBuffer {
		
		public:
			/**
			 * Constructor
			 */
			FrameBuffer( PixelFormat pixelFormat, uint16_t w, uint16_t h );

			/**
			 * Destructor
			 */
			~FrameBuffer();

			/**
			 * Pointer to the pixel data
			 */
			union Data{
				uint8_t* data8;
				uint16_t* data16;
				uint32_t* data32;
			} data;

			/**
			 * Pixel format of buffer
			 */
			PixelFormat pixelFormat;

			/**
			 * Width and height of buffer
			 */
			uint16_t width;
			uint16_t height;

			/**
			 * Count of pixels in buffer
			 */
			uint32_t count;

			/**
			 * Calculate the index into the framebuffer for a given coordinate. No bounds checking
			 * @param  x The X coordinate
			 * @param  y The Y coordinate
			 * @return   The index
			 */
			inline uint32_t index( int16_t x, int16_t y ){
				return width*y+x;
			}

			/**
			 * Calculate the index into the framebuffer for a given coordinate. Bounds checked
			 * @param  x The X coordinate
			 * @param  y The Y coordinate
			 * @return   The index
			 */
			inline uint32_t indexClip( int16_t x, int16_t y ){
				x = min(max(x,0), width-1);
				y = min(max(y,0), height-1 );
				return width*y+x;
			}

			/**
			 *  ####
			 * ##  ##
			 *  ####
			 * ##  ##
			 *  ####
			 */
			
			/**
			 * Write an 8bit pixel to the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void writePixel( uint8_t c, uint32_t i ){
				data.data8[i] = c;
			}

			/**
			 * Write an 8bit pixel to the framebuffer by index, with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void writePixelClip( uint8_t c, uint32_t i ){
				if (i>=count) return;
				data.data8[i] = c;
			}

			/**
			 * Write an 8bit pixel to the framebuffer by coordinate (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void writePixel( uint8_t c, int16_t x, int16_t y ){
				data.data8[y*width+x] = c;
			}

			/**
			 * Write an 8bit pixel to the framebuffer by coordinate with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void writePixelClip( uint8_t c, int16_t x, int16_t y ){
				if ((x<0) || (x>=width)) return;
				if ((y<0) || (y>=height)) return;
				data.data8[y*width+x] = c;
			}

			/**
			 * Read an 8bit pixel from the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void readPixel( uint32_t i, uint8_t& c ){
				c = data.data8[i];
			}

			/**
			 * Read an 8bit pixel from the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void readPixelClip( uint32_t i, uint8_t& c ){
				if (i>=count) c = 0;
				else c = data.data8[i];
			}

			/**
			 * Read an 8bit pixel from the framebuffer by coordinate (no bounds checking)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 * @param  c 	(out) The color (pixel)
			 */
			inline void readPixel( int16_t x, int16_t y, uint8_t& c ){
				c = data.data8[y*width+x];
			}

			/**
			 * Read an 8bit pixel from the framebuffer by coordinate with bounds checking
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 * @param  c 	The color (pixel)
			 */
			inline void readPixelClip( int16_t x, int16_t y, uint8_t& c ){
				if ((x<0) || (x>=width)) c = 0;
				else if ((y<0) || (y>=height)) c = 0;
				else c = data.data8[y*width+x];
			}

			/**
			 * Fill the framebuffer with an 8bit color
			 **/
			inline void clear8( uint8_t c ){
				uint32_t i=count;
				while (i) data.data8[--i] = c;
			}

			/**
			 *  ##   ####
			 * ###  ##
			 *  ##  #####
			 *  ##  ##  ## 
			 * ####  ####
			 */

			/**
			 * Write a 16bit pixel to the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void writePixel( uint16_t c, uint32_t i ){
				data.data16[i] = c;
			}

			/**
			 * Bleand a 16bit pixel to the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (5 bits, 0-31)
			 * @param  i 	The index into the framebuffer
			 */
			inline void blendPixel( uint16_t c, uint8_t a, uint32_t i ){
				data.data16[i] = alphaBlend5565( c, data.data16[i], a );
			}

			/**
			 * Write a 16bit pixel to the framebuffer by index, with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void writePixelClip( uint16_t c, uint32_t i ){
				if (i>=count) return;
				data.data16[i] = c;
			}

			/**
			 * Bleand a 16bit pixel to the framebuffer by index with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (5 bits, 0-31)
			 * @param  i 	The index into the framebuffer
			 */
			inline void blendPixelClip( uint16_t c, uint8_t a, uint32_t i ){
				if (i>=count) return;
				data.data16[i] = alphaBlend5565( c, data.data16[i], a );
			}

			/**
			 * Write a 16bit pixel to the framebuffer by coordinate (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void writePixel( uint16_t c, int16_t x, int16_t y ){
				data.data16[y*width+x] = c;
			}

			/**
			 * Blend a 16bit pixel to the framebuffer by coordinate (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (5 bits, 0-31)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void blendPixel( uint16_t c, uint8_t a, int16_t x, int16_t y ){
				uint32_t i = y*width+x;
				data.data16[i] = alphaBlend5565( c, data.data16[i], a );
			}

			/**
			 * Write a 16bit pixel to the framebuffer by coordinate with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void writePixelClip( uint16_t c, int16_t x, int16_t y ){
				if ((x<0) || (x>=width)) return;
				if ((y<0) || (y>=height)) return;
				data.data16[y*width+x] = c;
			}

			/**
			 * Blend a 16bit pixel to the framebuffer by coordinate with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (5 bits, 0-31)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void blendPixelClip( uint16_t c, uint8_t a, int16_t x, int16_t y ){
				if ((x<0) || (x>=width)) return;
				if ((y<0) || (y>=height)) return;
				uint32_t i = y*width+x;
				data.data16[i] = alphaBlend5565( c, data.data16[i], a );
			}

			/**
			 * read a 16bit pixel from the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void readPixel( uint32_t i, uint16_t& c ){
				c = data.data16[i];
			}

			/**
			 * read a 16bit pixel from the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void readPixelClip( uint32_t i, uint16_t& c ){
				if (i>=count) c = 0;
				else c = data.data16[i];
			}

			/**
			 * Read a 16bit pixel from the framebuffer by coordinate (no bounds checking)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 * @param  c 	(out) The color (pixel)
			 */
			inline void readPixel( int16_t x, int16_t y, uint16_t& c ){
				c = data.data16[y*width+x];
			}

			/**
			 * Read a 16bit pixel from the framebuffer by coordinate with bounds checking
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 * @param  c 	The color (pixel)
			 */
			inline void readPixelClip( int16_t x, int16_t y, uint16_t& c ){
				if ((x<0) || (x>=width)) c = 0;
				else if ((y<0) || (y>=height)) c = 0;
				else c = data.data16[y*width+x];
			}

			/**
			 * Fill the framebuffer with a 16bit color
			 **/
			inline void clear16( uint16_t c ){
				uint32_t i=count;
				while (i) data.data16[--i] = c;
			}

			/**
			 *  ####    ####
			 * ##  ##  ##  ##
			 *    ##      ##
			 * ##  ##   ##
			 *  ####   ######
			 */

			/**
			 * Write a 32bit pixel to the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void writePixel( uint32_t c, uint32_t i ){
				data.data32[i] = c;
			}

			/**
			 * Blend a 32bit pixel to the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  i 	The index into the framebuffer
			 */
			inline void blendPixel( uint32_t c, uint8_t a, uint32_t i ){
				data.data32[i] = alphaBlend8888( c, data.data32[i], i );
			}

			/**
			 * Write a 32bit pixel to the framebuffer by index, with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void writePixelClip( uint32_t c, uint32_t i ){
				if (i>=count) return;
				data.data32[i] = c;
			}

			/**
			 * Blend a 32bit pixel to the framebuffer by index, with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  i 	The index into the framebuffer
			 */
			inline void blendPixelClip( uint32_t c, uint8_t a, uint32_t i ){
				if (i>=count) return;
				data.data32[i] = alphaBlend8888( c, data.data32[i], i );
			}

			/**
			 * Write a 32bit pixel to the framebuffer by coordinate (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void writePixel( uint32_t c, int16_t x, int16_t y ){
				data.data32[y*width+x] = c;
			}

			/**
			 * Write a 32bit pixel to the framebuffer by coordinate (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void blendPixel( uint32_t c, uint8_t a, int16_t x, int16_t y ){
				uint32_t i = y*width+x;
				data.data32[i] = alphaBlend8888( c, data.data32[i], i );
			}

			/**
			 * Write a 32bit pixel to the framebuffer by coordinate with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void writePixelClip( uint32_t c, int16_t x, int16_t y ){
				if ((x<0) || (x>=width)) return;
				if ((y<0) || (y>=height)) return;
				data.data32[y*width+x] = c;
			}

			/**
			 * Blend a 32bit pixel to the framebuffer by coordinate with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void blendPixelClip( uint32_t c, uint8_t a, int16_t x, int16_t y ){
				if ((x<0) || (x>=width)) return;
				if ((y<0) || (y>=height)) return;
				uint32_t i = y*width+x;
				data.data32[i] = alphaBlend8888( c, data.data32[i], i );
			}

			/**
			 * read a 32bit pixel from the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void readPixel( uint32_t i, uint32_t& c ){
				c = data.data32[i];
			}

			/**
			 * read a 32bit pixel from the framebuffer by index with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void readPixelClip( uint32_t i, uint32_t& c ){
				if (i>=count) c = 0;
				else c = data.data32[i];
			}

			/**
			 * Read a 32bit pixel from the framebuffer by coordinate (no bounds checking)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 * @param  c 	(out) The color (pixel)
			 */
			inline void readPixel( int16_t x, int16_t y, uint32_t& c ){
				c = data.data32[y*width+x];
			}

			/**
			 * Read a 32bit pixel from the framebuffer by coordinate with bounds checking
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 * @param  c 	The color (pixel)
			 */
			inline void readPixelClip( int16_t x, int16_t y, uint32_t& c ){
				if ((x<0) || (x>=width)) c = 0;
				else if ((y<0) || (y>=height)) c = 0;
				else c = data.data32[y*width+x];
			}

			/**
			 * Fill the framebuffer with a 32bit color
			 **/
			inline void clear32( uint32_t c ){
				uint32_t i=count;
				while (i) data.data32[--i] = c;
			}

			/**
			 *  ####     ##   ##
			 * ##  ##    ##  ###
			 * ##  ##   ##    ##
			 * ##  ##  ##     ##
			 *  ####  ##    ######
			 */
			
			/**
			 * Write a 1bit pixel to the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void writePixelMono( uint8_t c, uint32_t i ){
				uint8_t m = 128 >> (i & 0b11);		// Which bit (mask)
				uint16_t b = i >> 3;				// Which byte
				if (c) data.data8[b] |= m;
				else data.data8[b] &= ~m;
			}

			/**
			 * Blend a 1bit pixel to the framebuffer by index (no bounds checking). This will
			 * write the pixel if it is ON and alpha is 0.5 or greater. Otherwise the pixel is
			 * not drawn.
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  i 	The index into the framebuffer
			 */
			inline void blendPixelMono( uint8_t c, alpha a, uint32_t i ){
				if (!c || (a<0.5f)) return;
				uint8_t m = 128 >> (i & 0b11);		// Which bit (mask)
				uint16_t b = i >> 3;				// Which byte
				data.data8[b] |= m;
			}

			/**
			 * Write a 1bit pixel to the framebuffer by index, with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void writePixelMonoClip( uint8_t c, uint32_t i ){
				if (i>=count) return;
				writePixelMono( c, i );
			}

			/**
			 * Blend a 1bit pixel to the framebuffer by index, with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  i 	The index into the framebuffer
			 */
			inline void blendPixelMonoClip( uint8_t c, uint8_t a, uint32_t i ){
				if (i>=count) return;
				blendPixelMono( c, a, i );
			}

			/**
			 * Write a 1bit pixel to the framebuffer by coordinate (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void writePixelMono( uint8_t c, int16_t x, int16_t y ){
				writePixelMono(c, y*width+x);
			}

			/**
			 * Write a 1bit pixel to the framebuffer by coordinate (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  a 	The Alpha (8 bits, 0-255)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void blendPixelMono( uint8_t c, uint8_t a, int16_t x, int16_t y ){
				blendPixelMono( c, a, y*width+x );
			}

			/**
			 * Write a 1bit pixel to the framebuffer by coordinate with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void writePixelMonoClip( uint8_t c, int16_t x, int16_t y ){
				if ((x<0) || (x>=width)) return;
				if ((y<0) || (y>=height)) return;
				writePixelMono(c, y*width+x);
			}

			/**
			 * Blend a 1bit pixel to the framebuffer by coordinate with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			inline void blendPixelMonoClip( uint8_t c, uint8_t a, int16_t x, int16_t y ){
				if ((x<0) || (x>=width)) return;
				if ((y<0) || (y>=height)) return;
				blendPixelMono( c, a, y*width+x );
			}

			/**
			 * read a 1bit pixel from the framebuffer by index (no bounds checking)
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void readPixelMono( uint32_t i, uint8_t& c ){
				uint8_t m = 128 >> (i & 0b11);		// Which bit (mask)
				uint16_t b = i >> 3;				// Which byte
				c = (data.data8[b] >> m) & 0b1;
			}

			/**
			 * read a 1bit pixel from the framebuffer by index with bounds checking
			 * @param  c 	The color (pixel)
			 * @param  i 	The index into the framebuffer
			 */
			inline void readPixelMonoClip( uint32_t i, uint8_t& c ){
				if (i>=count) c = 0;
				else readPixelMono( i, c );
			}

			/**
			 * Read a 1bit pixel from the framebuffer by coordinate (no bounds checking)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 * @param  c 	(out) The color (pixel)
			 */
			inline void readPixelMono( int16_t x, int16_t y, uint8_t& c ){
				readPixelMono( y*width+x, c );
			}

			/**
			 * Read a 1bit pixel from the framebuffer by coordinate with bounds checking
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 * @param  c 	The color (pixel)
			 */
			inline void readPixelMonoClip( int16_t x, int16_t y, uint8_t& c ){
				if ((x<0) || (x>=width)) c = 0;
				else if ((y<0) || (y>=height)) c = 0;
				else readPixelMono( y*width+x, c );
			}

			/**
			 * Fill the framebuffer with a 1bit color
			 **/
			inline void clearMono( uint8_t c ){
				uint32_t i = (uint32_t)((count+7)/8);
				if (c>0) c = 255;
				while (i) data.data8[--i] = c;
			}

			/**
			 *  ####   ##  ##
			 * ##  ##  ##  ##
			 * ######  ##  ##
			 * ##  ##  ##  ##
			 * ##  ##  ##  ##
			 */
			
			/**
			 * Prepare a color for fast blending by pre-calculating a few things. Don't generally use
			 * this unless you know what it's for. See GraphicsVectorExtn for an example.
			 * @param  c The color in RGB888 24-bit format 
			 * @param  a The alpha (0.0 - 1.0)
			 * @return   The prepared color
			 */
			PreparedColor* prepareColor( color888 c, alpha a );

			/**
			 * Fill the framebuffer with a color
			 **/
			void clear( color888 c );
			
			/**
			 * Write a pixel to the framebuffer. Checks the pixel format and uses the
			 * correct method. This method is slow. If you are setting a lot of pixels, use
			 * the pixel-format specific method, or hit the framebuffer directly.
			 * @param  c 	The color (pixel) in RGB 888 format
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			void pixel( color888 c, int16_t x, int16_t y );

			/**
			 * Blend a pixel to the framebuffer. Checks the pixel format and uses the
			 * correct method. This method is slow. If you are blending a lot of pixels, use
			 * the pixel-format specific method, or hit the framebuffer directly.
			 * @param  c 	The color (pixel) in RGB 888 format
			 * @param  a 	The alpha (0.0 - 1.0)
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 */
			void blend( color888 c, alpha a, int16_t x, int16_t y );

			/**
			 * read a pixel from the framebuffer in RGB 888 format. Checks the pixel format
			 * and uses the correct method. This method is slow. If you are reading a lot of
			 * pixels, use the pixel-format specific method, or hit the framebuffer directly.
			 * @param  x 	The X coordinate
			 * @param  y 	The Y coordinate
			 * @return 		The color (pixel) in RGB 888 format
			 */
			color888 pixel( int16_t x, int16_t y );

			/**
			 * read a pixel from the framebuffer in RGB 888 format. Checks the pixel format
			 * and uses the correct method. This method is slow. If you are reading a lot of
			 * pixels, use the pixel-format specific method, or hit the framebuffer directly.
			 * @param  i 	Index into the framebuffer
			 * @return 		The color (pixel) in RGB 888 format
			 */
			color888 pixel( uint32_t i );
			
			/**
			 * Scroll/move an area of the framebuffer. Note: After the scroll operation,
			 * the area vacated by the scroll is technically undefined. It will usually
			 * still contain the pixels prior to the scroll.
			 * @param rect The area to scroll
			 * @param dx   The amount to scroll horizontally
			 * @param dy   The amount to scroll vertically
			 */
			void scroll( ClipRect rect, int16_t dx, int16_t dy );

			/**
			 * Write the framebuffer to a File object as a BMP
			 * @param  file 	The file to write to
			 * @param  area 	An area of the framebuffer to output
			 * @return      	True if successful
			 */
			boolean bmp( File& file, ClipRect* area );
			boolean bmp( File& file );

			/**
			 * Validate entire dirty buffer
			 */
			void validate();

			/**
			 * Check if entire buffer is invalidated
			 */
			boolean invalidated();

			/**
			 * Return value of buffer line
			 * @param  v Vertical row
			 * @return   The value representing the row
			 */
			uint32_t invalideBuffer( uint8_t v );

			/**
			 * Invalidate entire dirty buffer
			 */
			void invalidate();

			/**
			 * Invalidate a pixel in a dirty buffer
			 * @param x The x-coord of the pixel
			 * @param y The y-coord of the pixel
			 */
			void invalidate( int16_t x, int16_t y );

			/**
			 * Invalidate a pixel in a dirty buffer
			 * @param i 	Index into the framebuffer
			 */
			void invalidate( uint32_t i );

			/**
			 * Invalidate a rectangular are in the dirty buffer
			 * @param x 	The x-coord of the top-left pixel
			 * @param y 	The y-coord of the top-left pixel
			 * @param w 	The width of the rect
			 * @param h 	The height of the rect
			 */
			void invalidateRect( int16_t x, int16_t y, int16_t w, int16_t h );

			/**
			 * Invalidate a rectangular are in the dirty buffer
			 * @param dx 	The x-coord of the top-left pixel
			 **/
			void invalidateRect( ClipRect* rect );

		protected:

			void _scroll8( ClipRect rect, int16_t dx, int16_t dy );
			void _scroll16( ClipRect rect, int16_t dx, int16_t dy );
			void _scroll32( ClipRect rect, int16_t dx, int16_t dy );
			void _scroll1( ClipRect rect, int16_t dx, int16_t dy );

			/**
			 * Invalidate buffer. Supports screens up to 512x512. Divides FB into 16 x 16px tiles
			 */
			uint32_t* _invalidateBuffer;
			boolean _invalidated;
	};
	
} // namespace

#endif