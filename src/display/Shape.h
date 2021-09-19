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
#ifndef _MAC_SHAPEH_
#define _MAC_SHAPEH_ 1

#include "display/DisplayObject.h"
#include "geom/Point.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * A shape (polygon)
	 */
	class Shape: public DisplayObject {
		
		public:
			/**
			 * Memory pool of recycled objects
			 */
			static DisplayObject* pool;

			/**
			 * Create a new object or take one from the pool
			 * @return The new or recycled object
			 */
			static Shape* Create();
			static Shape* Create( Point** points, uint16_t len );

			/**
			 * Type identifier for this object
			 **/
			static const DisplayObjectType TYPE = DisplayObjectType::shape;

			/**
			 * Reset the object back to default settings
			 */
			void reset();

			/**
			 * @brief Provide the points that define teh shape
			 * If the first and last points are different, the are automatically joined
			 * 
			 * @param points An array of points
			 * @param len The number of points
			 */
			void set( Point** points, uint16_t len );

			/**
			 * Update the display object.
			 * @param	dt 			Time since last update in seconds
			 */
			void update( float_t dt );

			/**
			 * Color
			 */
			color888 color;

			/**
			 * @brief Set the global position of the display object
			 * 
			 * @param x The global X position
			 * @param y The global Y position
			 */
			void globalPos( float_t x, float_t y );

			/**
			 * prepare to render the next line
			 * @param ry The y position in local coordinates
			 */
			void beginLine( int16_t ry );

			/**
			 * Read a pixel from the sprite and advance position
			 * @param rx The x position in local coordinates
			 * @param ry The y position in local coordinates
			 */
			void calcPixel( int16_t rx, int16_t ry );

		protected:
			
			/**
			 * Pool getter
			 */
			DisplayObject** _getPool() override;

			/**
			 * @brief The points that make up the shape
			 */
			Point** _points = 0;

			/**
			 * @brief The number of points
			 */
			uint16_t _pointsLen = 0;

	};
	
} // namespace

#endif