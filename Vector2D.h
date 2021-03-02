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

/**
 * Vector2D physics functions
 **/
 
#pragma once
#ifndef _MAC_VECTOR2D_H
#define _MAC_VECTOR2D_H 1

namespace mac{
	
	/**
	 * Vector2D float class
	 */
	class Vector2F {
		
		public:

			/**
			 * Constructor
			 */
			Vector2F();
			Vector2F( float x, float y );

			/**
			 * Angle in radians
			 */
			float a;

			/**
			 * Magnitude
			 */
			float m;

			/**
			 * Vector X
			 */
			float x;

			/**
			 * Vector y
			 */
			float y;

			/**
			 * Set the vector by angle and magnitude
			 */
			void set( float x, float y );

			/**
			 * Calculate the angle and magnitude based on x and y. Note that this
			 * is done automatically after any of the methods that adust x and y, and
			 * should only be called if you have manually changed x or y.
			 */
			void calc();

			/**
			 * Calculate the x and y based on angle and magnitude. Note that this
			 * is done automatically after any of the methods that adust the angle or
			 * magnitude, and should only be called if you have manually changed these.
			 */
			void calcXY();

			/**
			 * Add a vector to this one
			 */
			void add( Vector2F* v );
			Vector2F* getAdded( Vector2F* v );

			/**
			 * Subtract a vector from this one
			 */
			void subtract( Vector2F* v );
			Vector2F* getSubtracted( Vector2F* v );

			/**
			 * Normalise the vector
			 */
			void normalize();
			Vector2F* getNormalized();

			/**
			 * Rotate the vector
			 */
			void rotate( float a );
			Vector2F* getRotated( float a );

	};



} // ns:mac

#endif
