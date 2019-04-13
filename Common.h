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
	 * Swap two values
	 **/
	template<typename T>
	inline void swap(T &a, T &b){
		T t = a;
		a = b;
		b = t;
	}
	
	/**
	 * Defines a color as a RGB565 16bit unsigned int
	 **/
	typedef uint16_t color;
	
	/**
	 * Defines a color as packed ARGB components
	 **/
	typedef uint32_t colorARGB;
	
	/**
	 * Defines alpha as an 8bit unsigned char [0..255]
	 **/
	typedef uint8_t alpha;
	
	/**
	 * Defines a 2-D vertex (unsigned int)
	 **/
	typedef struct VertexS {
		uint16_t x;			// X position
		uint16_t y;			// Y position
	} Vertex;
	
	/**
	 * Defines a 2-D vertex (float)
	 **/
	typedef struct VertexFS {
		float x;			// X position
		float y;			// Y position
	} VertexF;
	
	/**
	 * Defines a 2-D line segment (unsigned int)
	 **/
	typedef struct LineS {
		uint16_t p0;		// First point
		uint16_t p1;		// Second point
	} Line;
	
	/**
	 * Defines a 2-D line segment (float)
	 **/
	typedef struct LineFS {
		VertexF p0;			// First point
		VertexF p1;			// Second point
	} LineF;
	
	/**
	 * Defines a span (float)
	 * A span is a solid are defined by two vertical edges. The lines
	 * may converge at either end, but not cross. The lines must start
	 * and finish at the same y coordinate.
	 **/
	typedef struct SpanFS {
		LineF s0;			// First point
		LineF s1;			// Second point
	} SpanF;
	
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
	 * Found in a pull request for the Adafruit framebuffer library. Clever!
	 * https://github.com/tricorderproject/arducordermini/pull/1/files#diff-d22a481ade4dbb4e41acc4d7c77f683d
	 * Converts  0000000000000000rrrrrggggggbbbbb
	 *     into  00000gggggg00000rrrrr000000bbbbb
	 * with mask 00000111111000001111100000011111
	 * This is useful because it makes space for a parallel fixed-point multiply
	 * This implements the linear interpolation formula: result = bg * (1.0 - alpha) + fg * alpha
	 * This can be factorized into: result = bg + (fg - bg) * alpha
	 * alpha is in Q1.5 format, so 0.0 is represented by 0, and 1.0 is represented by 32
	 * @param	fg		Color to draw in RGB565 (16bit)
	 * @param	bg		Color to draw over in RGB565 (16bit)
	 * @param	alpha	Alpha 0 - 255
	 **/
	inline color alphaBlendRGB565(
		uint32_t fg,
		uint32_t bg,
		alpha alpha
	){
		alpha = ( alpha + 4 ) >> 3; // Reduce to 0-31
		bg = (bg | (bg << 16)) & 0b00000111111000001111100000011111;
		fg = (fg | (fg << 16)) & 0b00000111111000001111100000011111;
		uint32_t result = ((((fg - bg) * alpha) >> 5) + bg) & 0b00000111111000001111100000011111;
		return (color)((result >> 16) | result); // contract result
	}
	
	/**
	 * Convert R G B color triplet to RGB565 16bit format
	 * @param	r		Red component
	 * @param	g		Green component
	 * @param	b		Blue component
	 **/
	inline color convertRGBtoRGB565(
		uint8_t r,
		uint8_t g,
		uint8_t b
	){
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	}
	
	/**
	 * Converts RGB565 format to RGB color triplet
	 * @param	color	The RGB565 color to convert
	 * @param	r		(out) Red component
	 * @param	g		(out) Green component
	 * @param	b		(out) Blue component
	 **/
	inline void convertRGB565toRGB(
		color color,
		uint8_t &r,
		uint8_t &g,
		uint8_t &b
	){
		r = (color>>8) & 0x00F8;
		g = (color>>3) & 0x00FC;
		b = (color<<3) & 0x00F8;
	}
	
	/**
	 * Convert HSV to a packed ARGB color
	 **/
	inline colorARGB convertHSVtoARGB(
		float H,
		float S,
		float V
	){
		float		p, q, t, ff;
		int8_t		i;
		float		r,g,b;

		if (H >= 360.0) H = 0.0;
		H /= 60.0;
		i = (int8_t)H;
		ff = H - i;
		p = V * (1.0 - S);
		q = V * (1.0 - (S * ff));
		t = V * (1.0 - (S * (1.0 - ff)));

		switch(i) {
			case 0:
				r = V;
				g = t;
				b = p;
				break;
			case 1:
				r = q;
				g = V;
				b = p;
				break;
			case 2:
				r = p;
				g = V;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = V;
				break;
			case 4:
				r = t;
				g = p;
				b = V;
				break;
			case 5:
			default:
				r = V;
				g = p;
				b = q;
				break;
		}
		return ((0xFF << 24) | ((uint8_t)(r * 255.0f) << 16) | ((uint8_t)(g * 255.0f) << 8) | (uint8_t)(b * 255.0f));
	}
	
} // ns::mac

#endif