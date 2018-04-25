/**
 * Common types for "mac/μac"
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
#ifndef _MAC_COMMONH_
#define _MAC_COMMONH_ 1

#include <Arduino.h>

#define CONV_RGB565(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))
#define CONV_RGB888(c) do { r = ((c)>>8) & 0x00F8; g = ((c)>>3) & 0x00FC; b = ((c)<<3) & 0x00F8; }while(0)

namespace mac{
	/**
	 * Defines a color as a RGB565 16bit unsigned int
	 **/
	typedef uint16_t color;
	
	/**
	 * Defines alpha as an 8bit unsigned char [0..255]
	 **/
	typedef uint8_t alpha;
	
	/**
	 * Defines a 2-D vector (unsigned int)
	 **/
	typedef struct VectorS {
		uint16_t x;			// X position
		uint16_t y;			// Y position
	} Vector;
	
	/**
	 * Defines a 2-D vector (float)
	 **/
	typedef struct VectorFS {
		float x;			// X position
		float y;			// Y position
	} VectorF;
	
	/**
	 * Defines a clipping area
	 **/
	typedef struct ClipRectS {
		uint16_t x;			// X position of top-left corner
		uint16_t y;			// Y position of top-left corner
		uint16_t w;			// Width of area
		uint16_t h;			// Height of area
	} ClipRect;
	
	/**
	 * Defines an area of a buffer
	 **/
	typedef struct BufferRectS:ClipRect {
		uint16_t* buffer;	// The address of the framebuffer
		uint16_t* start;	// The start address of first pixel in the clip area
		uint16_t stride;	// Stride from start of one line to the next (always equal to width of framebuffer)
	} BufferRect;
	
	/**
	 * Colors
	 **/
	enum {
		RGB565_TRANSPARENT 	= 0xf81f,
		RGB565_BLACK 		= 0x0000,
		RGB565_WHITE 		= 0xffff
	};
	
	/**
	 * Pixel formats
	 **/
	enum {
		PF_UNKNOWN 			= 0,	// Unknown
		PF_RGB565 			= 1,	// RGB565
		PF_RGBA5658 		= 2,	// RGB565 with additional 8-bit alpha (24bits total)
		PF_RGB				= 3,	// RGB 888 (24-bit color, no alpha)
		PF_RGBA				= 4		// RGBA 8888 (32-bit color including alpha)
	};
	
	/**
	 * Holds details of a tile map (data in flash)
	 **/
	typedef struct TileMapS {
		uint32_t tileWidth;
		uint32_t tileHeight;
		uint32_t tileCount;
		uint32_t tileStride;
		uint8_t pixelFormat;
		uint32_t dataSize;
		union {
			uint8_t* data8;
			uint16_t* data16;
		} __attribute__ ((aligned (4)));
	} TileMap;
	
	/**
	 * Fast but reasonably inaccurate blending of two RGB565 pixels.
	 * Thanks to biziclop on stack overflow!
	 * https://stackoverflow.com/a/19068028/6036640
	 * @param	fg		The foreground color (the one we are drawing)
	 * @param	bg		The background color
	 * @param	alpha	The alpha transparency level from 0 to 255
	 **/
	color alphaBlendRGB565_approx(
		color fg,
		color bg,
		alpha alpha
	);
	
	/**
	 * Found in a pull request for the Adafruit framebuffer library. Clever!
	 * https://github.com/tricorderproject/arducordermini/pull/1/files#diff-d22a481ade4dbb4e41acc4d7c77f683d
	 **/
	color alphaBlendRGB565(
		uint32_t fg,
		uint32_t bg,
		alpha alpha
	);
	
	/**
	 * Convert R G B color triplet to RGB565 16bit format
	 * @param	r		Red component
	 * @param	g		Green component
	 * @param	b		Blue component
	 **/
	color convertRGBtoRGB565( uint8_t r, uint8_t g, uint8_t b );
	
	/**
	 * Converts RGB565 format to RGB color triplet
	 * @param	color	The RGB565 color to convert
	 * @param	r		(out) Red component
	 * @param	g		(out) Green component
	 * @param	b		(out) Blue component
	 **/
	void convertRGB565toRGB(color color, uint8_t &r, uint8_t &g, uint8_t &b);
	
} // ns::mac

#endif