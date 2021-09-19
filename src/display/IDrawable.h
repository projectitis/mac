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
#ifndef _MAC_IDRAWABLEH_
#define _MAC_IDRAWABLEH_ 1

#include "Common.h"
#include "geom/ClipRect.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * An interface for all drawable objects
	 */
	class IDrawable {
		public:
			/**
			 * @brief Begin the render sweep for the current frame
			 * The render sweep for each frame is:
			 * 		beginRender
			 * 			prepareLine y
			 * 				readPixel|readMaskPixel|skipPixel x ... x2
			 *			...
			 * 			preparLine y2
			 * 				readPixel|readMaskPixel|skipPixel x ... x2
			 * 		endRender
			 * @param updateArea The area of the display being updated
			 */
			virtual void beginRender( ClipRect* updateArea ){}

			/**
			 * @Brief Initialise the next line to be read for rendering
			 * 
			 * Identified the y coordinate in local space at which the next line of pixels
			 * will be read (@see readPixel). In the same sweep, ry will always be 1 greater
			 * than ry on the last call to prepareLine
			 * 
			 * @param ry The current Y position (line) in local coordinates
			 */
			virtual void beginLine( int16_t ry ){}

			/**
			 * @brief Calculate the pixel to be rendered at the current position
			 * 
			 * The coordinates, rx and ry, are in local coordinates. rx will always be n + 1
			 * higher than rx the previous call to calcPixel, calcMaskPixel or skipPixel for
			 * a given line (prepareLine is called at the start of a line). ry will be
			 * identical.
			 * 
			 * The renderer expects the  results of calcPixel to be in _rc (the color, 24bit RGB)
			 * and _ra (alpha 0.0 - 1.0).
			 * 
			 * @param rx The current X position in local coordinates
			 * @param ry The current Y position in local coordinates
			 */
			virtual void calcPixel( int16_t rx, int16_t ry ){}

			/**
			 * @brief Calculate the pixel to be rendered at the current position as a mask
			 * Any display object can be used as a mask. In this case calcMaskPixel is called instead of
			 * calcPixel. The function only needs to calculate alpha, as the color value is ignored
			 * when used as a mask. Although it is possible to simply call calcPixel, an implementaton
			 * may be able to be done more efficiently if it is known that only alpha will be used.
			 * 
			 * @param rx The current X position in local coordinates
			 * @param ry The current Y position in local coordinates
			 */
			virtual void calcMaskPixel( int16_t rx, int16_t ry ){}

			/**
			 * @brief Skip the pixel at the current position
			 * If the object is not being drawn (for example, it is masked by a higher object) then
			 * skipPixel will be called instead of calcPixel. Some implementations may require some
			 * processing, even if the pixel is not being rendered.
			 * 
			 * @param rx The current X position in local coordinates
			 * @param ry The current Y position in local coordinates
			 */
			virtual void skipPixel( int16_t rx, int16_t ry ){}

			/**
			 * @brief End the render sweep for the current frame
			 * @see beginRender
			 */
			virtual void endRender(){}

	}; // class

} // namespace

#endif