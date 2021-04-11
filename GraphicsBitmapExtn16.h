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
 
#pragma once
#ifndef _MAC_GRAPHICSBITMAPEXTN16H_
#define _MAC_GRAPHICSBITMAPEXTN16H_ 1

#include "GraphicsBitmapExtn.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Adds bitmap drawing functions to the Graphics library
	 */
	class GraphicsBitmapExtn16: public GraphicsBitmapExtn {
		public:

			/**
			 * Blit a tile from the tilemap
			 * @param 	tileMap A tilemap containing the tile data
			 * @param 	index   The index of the tile in the map
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	alpha	The alpha value (0.0-1.0)
			 * @param	r		The rotation
			 */
			virtual void blit(
				const Tilemap* tilemap,
				uint32_t index,
				int16_t x,
				int16_t y,
				alpha alpha = 1,
				BitmapRotation r = BitmapRotation::none
			);

			/**
			 * Blit a bitmap
			 * @param 	bitmap 	A bitmap containing the pixel data
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	alpha	The alpha value (0.0-1.0)
			 * @param	r		The rotation
			 */
			virtual void blit(
				Bitmap bitmap,
				int16_t x,
				int16_t y,
				alpha alpha = 1,
				BitmapRotation r = BitmapRotation::none
			);

			/**
			 * Blit an area of a bitmap
			 * @param 	bitmap 	A bitmap containing the pixel data
			 * @param 	rect 	The area of the bitmap to blit
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	alpha	The alpha value (0.0-1.0)
			 * @param	r		The rotation
			 */
			virtual void blit(
				Bitmap bitmap,
				ClipRect* rect,
				int16_t x,
				int16_t y,
				alpha alpha = 1,
				BitmapRotation r = BitmapRotation::none
			);

			/**
			 * Blit a tile from the tilemap as a stamp
			 * @param 	color 	The color of the stamp
			 * @param 	tileMap A tilemap containing the tile data
			 * @param 	index   The index of the tile in the map
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	alpha	The alpha value (0.0-1.0)
			 * @param	r		The rotation
			 */
			virtual void stamp(
				color888 color,
				const Tilemap* tilemap,
				uint32_t index,
				int16_t x,
				int16_t y,
				alpha alpha = 1,
				BitmapRotation r = BitmapRotation::none
			);

			/**
			 * Blit an area of a tile from the tilemap as a stamp
			 * @param 	color 	The color of the stamp
			 * @param 	tileMap A tilemap containing the tile data
			 * @param 	index   The index of the tile in the map
			 * @param 	rect 	The area of the tile to blit
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	alpha	The alpha value (0.0-1.0)
			 * @param	r		The rotation
			 */
			virtual void stamp(
				color888 color,
				const Tilemap* tilemap,
				uint32_t index,
				ClipRect* rect,
				int16_t x,
				int16_t y,
				alpha alpha = 1,
				BitmapRotation r = BitmapRotation::none
			);

			/**
			 * Blit a bitmap
			 * @param 	color 	The color of the stamp
			 * @param 	bitmap 	A bitmap containing the pixel data
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	alpha	The alpha value (0.0-1.0)
			 * @param	r		The rotation
			 */
			virtual void stamp(
				color888 color,
				Bitmap bitmap,
				int16_t x,
				int16_t y,
				alpha alpha = 1,
				BitmapRotation r = BitmapRotation::none
			);

			/**
			 * Blit an area of a bitmap as a stamp
			 * @param 	color 	The color of the stamp
			 * @param 	bitmap 	A bitmap containing the pixel data
			 * @param 	rect 	The area of the bitmap to blit
			 * @param	dx		The destination x coord within the framebuffer
			 * @param	dy		The destination y coord within the framebuffer
			 * @param	alpha	The alpha value (0.0-1.0)
			 * @param	r		The rotation
			 */
			virtual void stamp(
				color888 color,
				Bitmap bitmap,
				ClipRect* rect,
				int16_t x,
				int16_t y,
				alpha alpha = 1,
				BitmapRotation r = BitmapRotation::none
			);

		private:

			boolean _clip( BitmapRotation r, int16_t& x, int16_t& y, uint16_t& sw, uint16_t& sh, uint32_t& si, uint32_t& di, int16_t& dp, int16_t& ds );

			void _blitA( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds );
			void _blitT( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 tc );
			void _blitG( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 tc );
			void _stampA( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 sc );
			void _stampT( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 sc, color565 tc );
			void _stampG( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 sc, color565 tc );
	};
	
} // namespace

#endif