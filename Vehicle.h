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
#ifndef _MAC_VEHICLE_H
#define _MAC_VEHICLE_H 1

#include "Vector2D.h"

namespace mac{

	/**
	 * Vector2D float class
	 */
	class Vehicle {
		
		public:

			/**
			 * Maximum (and minium) steering angle in degrees
			 */
			float maxSteer = 25;

			/**
			 * Constructor
			 */
			Vehicle();
			Vehicle( float x, float y );

			/**
			 * Vehicle position x
			 */
			float x;

			/**
			 * Vehicle position y
			 */
			float y;

			/**
			 * Current vehicle bearing
			 */
			Vector2F* bearing;

			/**
			 * Friction
			 */
			float f = 0;

			/**
			 * Steering angle
			 */
			float s = 0;

			/**
			 * Update the car position and stats
			 */
			void update( float dt );

			/**
			 * Set the speed (magnitude) portion of the bearing
			 * @param s The speed in pps
			 */
			void setSpeed( float s );

			/**
			 * Set the vehicle heading in degrees
			 * @param a Heading in degrees
			 */
			void setHeading( float a );

			/**
			 * Set steering wheel angle
			 * @param s Steering angle in degrees
			 */
			void steer( float s );
	};



} // ns:mac

#endif
