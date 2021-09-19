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
#ifndef _MAC_RECTANGLEH_
#define _MAC_RECTANGLEH_ 1

#include "display/DisplayObject.h"
#include "display/Gradient.h"
#include "display/Borders.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * A rectangle
	 */
	class Box: public DisplayObject {
		
		public:
			/**
			 * Memory pool of recycled objects
			 */
			static DisplayObject* pool;

			/**
			 * Create a new object or take one from the pool
			 * @return The new or recycled object
			 */
			static Box* Create();
			static Box* Create( int16_t x, int16_t y, int16_t w, int16_t h );
			static Box* Create( ClipRect* rect );

			/**
			 * Type identifier for this object
			 **/
			static const DisplayObjectType TYPE = DisplayObjectType::box;

			/**
			 * @brief Construct a new Box object
			 */
			Box();

			/**
			 * @brief Destroy the Box object
			 */
			~Box();

			/**
			 * Reset the object back to default settings
			 */
			void reset();

			/**
			 * @brief Provide the points that define the shape
			 * If the first and last points are different, the are automatically joined
			 * 
			 * @param points An array of points
			 * @param len The number of points
			 */
			void set( int16_t x, int16_t y, int16_t w, int16_t h );

			/**
			 * Color. Note that if a gradient is set, it will be used instead
			 */
			color888 color;

			/**
			 * Gradient. If set, will be used instead of color
			 */
			Gradient* gradient;

			/**
			 * @brief Borders of the box
			 */
			Borders* borders;

			/**
			 * @brief Begin the render sweep for the current frame
			 * @param updateArea The area of the display being updated
			 */
			void beginRender( ClipRect* updateArea );

			/**
			 * prepare to render the next line
			 * @param ry The y position in local coordinates
			 */
			void beginLine( int16_t ry );

			/**
			 * Read a pixel
			 * @param rx The x position in local coordinates
			 * @param ry The y position in local coordinates
			 */
			void calcPixel( int16_t rx, int16_t ry );

			/**
			 * Skip a pixel
			 * @param rx The x position in local coordinates
			 * @param ry The y position in local coordinates
			 */
			void skipPixel( int16_t rx, int16_t ry );

		protected:
			
			/**
			 * Pool getter
			 */
			DisplayObject** _getPool() override;

	};
	
} // namespace

#endif