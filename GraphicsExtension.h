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
 
#ifndef _MAC_GRAPHICSEXTNH_
#define _MAC_GRAPHICSEXTNH_ 1

#include "Common.h"

/**
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * A base class for extending the Graphics object with additional features.
	 */
	class GraphicsExtension {
		public:
			/**
			 * Called by Graphics to initialise the extension
			 **/
			void init( BufferRect* framebuffer ){
				_framebuffer = framebuffer;
			}
			
		protected:
			
			/**
			 * The display instance for the hardware
			 **/
			BufferRect* _framebuffer;

			/**
			 * Blend a pixel
			 * Same as Graphics::pixel but works with premultiplied values to optise inner drawing loops.
			 * See Common.h alphaBlendRGB565 for explanation
			 * @param	colorExpanded	Color in format 00000gggggg00000rrrrr000000bbbbb
			 * @param	alphaReduced	Alpha in range 0-31
			 **/
			inline void _pixelBlendExp(
				uint32_t x,
				uint32_t y,
				uint32_t colorExpanded,
				uint8_t alphaReduced
			){
				uint32_t fbo = _framebuffer->stride*y + x;
				uint32_t bg = _framebuffer->buffer[fbo];
				bg = (bg | (bg << 16)) & 0b00000111111000001111100000011111;
				uint32_t result = ((((colorExpanded - bg) * alphaReduced) >> 5) + bg) & 0b00000111111000001111100000011111;
				_framebuffer->buffer[fbo] = (color)((result >> 16) | result);
			}
			
	};
	
} // namespace

#endif