/**
 * Graphics library for "mac/μac"
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
 
#ifndef _MAC_GRAPHICSBITMAPEXTNH_
#define _MAC_GRAPHICSBITMAPEXTNH_ 1

#include "GraphicsExtension.h"

/**
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Adds bitmap drawing functions to the Graphics library
	 */
	class GraphicsBitmapExtn: public GraphicsExtension {
		public:

			/***
			 *** Tilemap (various pixel formats)
			 ***/

			/**
			 * Blit a tile from the tilemap with on/off transparency, and apply
			 * constant alpha to the blit.
			 * @param 	tileMap A tilemap containing the tile data
			 * @param 	index   The index of the tile in the map
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	alpha	The alpha value (0-255)
			 */
			void blit(
				TileMap tileMap,
				uint32_t index,
				int16_t dx,
				int16_t dy,
				alpha alpha = 255
			);
			
			/***
			 *** RGB 565 (16 bit color)
			 ***/
			
			/**
			 * Blit an RGB565 bitmap from memory with no alpha
			 * @param	source	The start address of the source bitmap in memory
			 * @param	sw		The width, in pixels, of the source bitmap
			 * @param	sh		The height, in pixels, of the source bitmap 
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 **/
			void blitRGB565(
				const uint16_t* source,
				uint16_t sw,
				uint16_t sh,
				int16_t dx,
				int16_t dy
			);
			
			/**
			 * Blit an RGB565 bitmap from memory with on/off transparency
			 * @param	source	The start address of the source bitmap in memory
			 * @param	sw		The width, in pixels, of the source bitmap
			 * @param	sh		The height, in pixels, of the source bitmap 
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	transparentColor		The color to use as transparent
			 **/
			void blitRGB565_T(
				const uint16_t* source,
				uint16_t sw,
				uint16_t sh,
				int16_t dx,
				int16_t dy,
				color transparentColor = RGB565_TRANSPARENT
			);
			
			/**
			 * Blit an RGB565 bitmap from memory with on/off transparency, and apply
			 * constant alpha to the blit.
			 * @param	source	The start address of the source bitmap in memory
			 * @param	sw		The width, in pixels, of the source bitmap
			 * @param	sh		The height, in pixels, of the source bitmap 
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	transparentColor		The color to use as on/off alpha
			 * @param	alpha	The alpha value (0-255)
			 **/
			void blitRGB565_A(
				const uint16_t* source,
				uint16_t sw,
				uint16_t sh,
				int16_t dx,
				int16_t dy,
				alpha alpha,
				color transparentColor = RGB565_TRANSPARENT
			);
			
			/***
			 *** RGBA 5658 (16 bit color, 8 bits alpha)
			 ***/
			
			/**
			 * Blit an RGBA5658 bitmap from memory (RGB565 plus alpha, 24bit packed format)
			 * @param	source	The start address of the source bitmap in memory
			 * @param	sw		The width, in pixels, of the source bitmap
			 * @param	sh		The height, in pixels, of the source bitmap 
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 **/
			void blitRGBA5658(
				uint8_t* source,
				uint16_t sw,
				uint16_t sh,
				int16_t dx,
				int16_t dy
			);
			
			/***
			 *** RGBA 8888
			 ***/
			
			/**
			 * Blit an RGBA 32bit bitmap from memory (8bits per channel plus alpha)
			 * @param	source	The start address of the source bitmap in memory
			 * @param	sw		The width, in pixels, of the source bitmap
			 * @param	sh		The height, in pixels, of the source bitmap 
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 **/
			void blitRGBA(
				uint8_t* source,
				uint16_t sw,
				uint16_t sh,
				int16_t dx,
				int16_t dy
			);
			
			/***
			 *** RGB 888
			 ***/
			
			/**
			 * Blit an RGB 24bit bitmap from memory (8bits per channel)
			 * @param	source	The start address of the source bitmap in memory
			 * @param	sw		The width, in pixels, of the source bitmap
			 * @param	sh		The height, in pixels, of the source bitmap 
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 **/
			void blitRGB(
				uint8_t* source,
				uint16_t sw,
				uint16_t sh,
				int16_t dx,
				int16_t dy
			);
		
		protected:
			
			/**
			 * Temporary values while calculating clipping
			 **/
			int16_t _mdx;
			int16_t _mdy;
	};
	
} // namespace

#endif