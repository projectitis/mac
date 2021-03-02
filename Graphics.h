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
#ifndef _MAC_GRAPHICSH_
#define _MAC_GRAPHICSH_ 1

#include "FrameBuffer.h"
#include "Display.h"
#include "GraphicsBitmapExtn.h"
#include "GraphicsVectorExtn.h"
#include "GraphicsTextExtn.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Graphics extensions
	 */
	enum GraphicsExtensions {
	    extension_bitmap 	= 1,
	    extension_vector 	= 2,
	    extension_text 		= 4
	};
	
	/**
	 * A device- and display- independent graphics library. All the methods in
	 * this library work directly on an in-memory framebuffer. Drawing is (mostly) done using
	 * standard 24-bit RGB (8-bits per channel) and 0.0 - 1.0 floating point alpha. The
	 * library handles conversion to the display's pixel format.
	 */
	class Graphics {
		
		public:
			/**
			 * Constructor. Must pass in a display adapter for the hardware display being used.
			 * @param	display			A Display adapter instance for the hardware display being used
			 **/
			Graphics( Display* display );

			/**
			 * Constructor. Must pass in a display adapter for the hardware display being used.
			 * Can optionally load extensions that are required. This may save some memory on
			 * less capable hardware.
			 * @usage new Graphics( display, extension_bitmap | extension_vector );
			 * @param	display			A Display adapter instance for the hardware display being used
			 * @param	extensionFlags 	The extensions to load (@see mac::GraphicsExtensions enum type)
			 **/
			Graphics( Display* display, int extensionFlags );
			
			/**
			 * Destructor
			 **/
			~Graphics();

			/**
			 * The display instance for the hardware
			 **/
			Display* display;

			/**
			 * A reference to the frame buffer as a shortcut (actually display->framebuffer)
			 **/
			FrameBuffer* framebuffer;
			
			/**
			 * Update the framebuffer to the display
			 **/
			void update();

			/**
			 * Clear the display to a solid color
			 * @param color The color
			 */
			void clear(
				color888 color
			);
			
			/**
			 * Blend a pixel to the display
			 * @param x     Display X coordinate
			 * @param y     Display Y coordinate
			 * @param color The color
			 * @param alpha The alpha (0.0 - 1.0)
			 */
			void pixel(
				int16_t x,
				int16_t y,
				color888 color,
				alpha alpha = 1
			);

			/**
			 * get a pixel from the display
			 * @param x     Display X coordinate
			 * @param y     Display Y coordinate
			 * @return		The pixel color
			 */
			color888 pixel(
				int16_t x,
				int16_t y
			);

			/**
			 * Draw a horizontal line.
			 * @param x     	Start x coord
			 * @param y     	Start y coord
			 * @param length 	Length of line
			 * @param color 	Color of line
			 * @param alpha 	Alpha of the line (0.0 - 1.0)
			 */
			void lineH(
				int16_t x,
				int16_t y,
				int16_t length,
				color888 color,
				alpha alpha = 1
			);

			/**
			 * Draw a horizontal line with gradient
			 * @param x     	Start x coord
			 * @param y     	Start y coord
			 * @param length 	Length of line
			 * @param color0 	Left color
			 * @param alpha0 	Left alpha (0.0 - 1.0)
			 * @param color1 	Right color
			 * @param alpha1 	Right alpha (0.0 - 1.0)
			 */
			void lineHG(
				int16_t x,
				int16_t y,
				int16_t length,
				color888 color0,
				alpha alpha0,
				color888 color1,
				alpha alpha1
			);

			/**
			 * Draw a vertical line.
			 * @param x     	Start x coord
			 * @param y     	Start y coord
			 * @param length 	Length of line
			 * @param color 	Color of line
			 * @param alpha 	Alpha of the line (0.0 - 1.0)
			 */
			void lineV(
				int16_t x,
				int16_t y,
				int16_t length,
				color888 color,
				alpha alpha = 1
			);

			/**
			 * Draw a vertical line with gradient
			 * @param x     	Start x coord
			 * @param y     	Start y coord
			 * @param length 	Length of line
			 * @param color0 	Top color
			 * @param alpha0 	Top alpha (0.0 - 1.0)
			 * @param color1 	Bottom color
			 * @param alpha1 	Bottom alpha (0.0 - 1.0)
			 */
			void lineVG(
				int16_t x,
				int16_t y,
				int16_t length,
				color888 color0,
				alpha alpha0,
				color888 color1,
				alpha alpha1
			);

			/**
			 * Draw a basic rectangle with alpha. If you require sub-pixel accuracy,
			 * use the vector extension (graphics->vector->rectangle).
			 * @param x     	Start x coord
			 * @param y     	Start y coord
			 * @param width  	The width of the rectangle
			 * @param height 	The height of the rectangle
			 * @param color 	Color of rectangle
			 * @param alpha 	Alpha of the rectangle (0.0 - 1.0)
			 */
			void rectangle(
				int16_t x,
				int16_t y,
				int16_t width,
				int16_t height,
				color888 color,
				alpha alpha = 1
			);

			/**
			 * Draw a basic rectangle with horizontal gradient.
			 * @param x     	Start x coord
			 * @param y     	Start y coord
			 * @param width  	The width of the rectangle
			 * @param height 	The height of the rectangle
			 * @param color0 	Left color of rectangle
			 * @param alpha0 	Left alpha of the rectangle (0.0 - 1.0)
			 * @param color1 	Right color of rectangle
			 * @param alpha1 	Right alpha of the rectangle (0.0 - 1.0)
			 **/
			void rectangleHG(
				int16_t x,
				int16_t y,
				int16_t width,
				int16_t height,
				color888 color0,
				alpha alpha0,
				color888 color1,
				alpha alpha1
			);

			/**
			 * Draw a basic rectangle with vertical gradient.
			 * @param x     	Start x coord
			 * @param y     	Start y coord
			 * @param width  	The width of the rectangle
			 * @param height 	The height of the rectangle
			 * @param color0 	Top color of rectangle
			 * @param alpha0 	Top alpha of the rectangle (0.0 - 1.0)
			 * @param color1 	Bottom color of rectangle
			 * @param alpha1 	Bottom alpha of the rectangle (0.0 - 1.0)
			 **/
			void rectangleVG(
				int16_t x,
				int16_t y,
				int16_t width,
				int16_t height,
				color888 color0,
				alpha alpha0,
				color888 color1,
				alpha alpha1
			);
			
			/***
			 *** EXTENSIONS
			 ***/
			
			/**
			 * Bitmap graphic support
			 **/
			GraphicsBitmapExtn* bitmap;
			
			/**
			 * Vector graphic support
			 **/
			GraphicsVectorExtn* vector;

			/**
			 * Text support
			 **/
			GraphicsTextExtn* text;

		protected:

			/**
			 * Initialise graphics. Called from constructor(s).
			 * @param	display			A Display instance for the hardware display being used
			 * @param	extensionFlags 	The extensions to load (@see mac::GraphicsExtensions enum type)
			 **/
			void _init( Display* display, int extensionFlags );

			/**
			 * Internal method to draw H or V line in 16-bit pixel format
			 * @param x     	Start x coord
			 * @param y     	Start y coord
			 * @param step		Framebuffer step to next pixel
			 * @param length    Number of pixels
			 * @param color 	Color of line
			 * @param alpha 	Alpha of the line (0.0 - 1.0)
			 */
			void _line16(
				uint16_t x,
				uint16_t y,
				uint16_t step,
				uint16_t length,
				color888 color,
				alpha alpha = 1
			);

			/**
			 * Internal method to draw H or V line in 32-bit pixel format
			 * @param x     	Start x coord
			 * @param y     	Start y coord
			 * @param step		Framebuffer step to next pixel
			 * @param length    Number of pixels
			 * @param color 	Color of line
			 * @param alpha 	Alpha of the line (0.0 - 1.0)
			 */
			void _line32(
				uint16_t x,
				uint16_t y,
				uint16_t step,
				uint16_t length,
				color888 color,
				alpha alpha = 1
			);
			
	};
	
} // namespace

#endif