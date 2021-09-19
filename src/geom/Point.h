/**
 * Vector functions for "mac/μac"
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
#ifndef _MAC_POINT_H
#define _MAC_POINT_H 1

namespace mac{
	
	/**
	 * Point float_t class
	 */
	class Point {
		
		public:

			/**
			 * @brief Construct a new Point object
			 * 
			 * @param x The x coordinate
			 * @param y The y coordinate
			 */
			Point( float_t x=0, float_t y=0 ) {
				this->x = x;
				this->y = y;
			}

			/**
			 * X coordinate
			 */
			float_t x;

			/**
			 * Y coordinate
			 */
			float_t y;

			/**
			 * @brief Set both x and y at once
			 * 
			 * @param x The X coordinate
			 * @param y The Y coordinate
			 */
			void set( float_t x, float_t y ) {
				this->x = x;
				this->y = y;
			}

			/**
			 * @brief Copy the values from another point to this one
			 * 
			 * @param from The point to copy from
			 */
			void copy( Point* from ) {
				x = from->x;
				y = from->y;
			}

			/**
			 * @brief Create a new Point with the same values as this one
			 * 
			 * @return Point* The new point
			 */
			Point* clone() {
				Point* p = new Point();
				p->copy( this );
				return p;
			}

	};



} // ns:mac

#endif
