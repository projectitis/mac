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
#ifndef _MAC_GRADIENTH_
#define _MAC_GRADIENTH_ 1

#include "display/IDrawable.h"
#include "graphics/Bitmap.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * @brief A gradient stop
	 * XXX: Pool/recycle
	 *
	 **/
	class GradientStop {
		public:
			/**
			 * Memory pool of recycled objects
			 */
			static GradientStop* pool;

			/**
			 * Create a new object or take one from the pool
			 * @return The new or recycled object
			 */
			static GradientStop* Create();

			/**
			 * Return this object to the pool
			 */
			void recycle();

			/**
			 * Reset the object back to default settings
			 */
			void reset();

			/**
			 * @brief update the stop
			 */
			void update( float_t pos );

			color888 color = 0;
			float_t alpha = 1.0;
			float_t position = 0.0;
			float_t distance = 0.0;
			float_t step = 0.0;
			float_t r = 0.0;
			float_t g = 0.0;
			float_t b = 0.0;
			float_t a = 0.0;
			float_t dr = 0.0;
			float_t dg = 0.0;
			float_t db = 0.0;
			float_t da = 0.0;

		protected:

			/**
			 * Pointer to next object in memory pool of recycled objects
			 */
			GradientStop* _poolNext = 0;
	};

	/**
	 * A gradient
	 */
	class Gradient: public IDrawable {
		
		public:

			/**
			 * @brief Construct a new Gradient object
			 * 
			 * @param numStops The number of stops. Must be at least 2
			 */
			Gradient( uint8_t numStops = 2 );

			/**
			 * @brief Destroy the Gradient object
			 */
			virtual ~Gradient();

			/**
			 * @brief Removes all stops and sets default position
			 */
			void reset();

			/**
			 * @brief Specify the start and end points
			 */
			void position( float_t x, float_t y, float_t x2, float_t y2 );

			/**
			 * @brief Set a gradient stop
			 * @param index The stop index
			 * @param color The color of the stop
			 * @param alpha The alpha at the stop
			 * @param position The position along the gradient (0.0 - 1.0)
			 * @return self, for chaining
			 */
			Gradient* stop( uint8_t index, color888 color, float_t alpha, float_t position );

			/**
			 * @brief Reverse the stops
			 *  
			 */
			void reverse();

			/**
			 * Read a pixel
			 * @param rx The x position in local coordinates
			 * @param ry The y position in local coordinates
			 */
			virtual void calcPixel( int16_t rx, int16_t ry ){
				rc = 0;
				ra = 1.0;
			}

			/**
			 * @brief The color value result of calcPixel
			 */
			color888 rc;

			/**
			 * @brief The alpha value result of calcPixel
			 */
			float_t ra;

		protected:

			/**
			 * @brief The number of stops
			 */
			uint8_t _numStops = 0;

			/**
			 * @brief The active stop (while rendering)
			 */
			int _activeStop = 0;

			/**
			 * @brief The stops that make up this gradient
			 */
			GradientStop** _stops = 0;

			/**
			 * @brief Flag to indicate if stops need recalculating
			 */
			bool _needsCalc = true;

			/**
			 * @brief Flag to indicate whether the stops are reversed
			 */
			bool _reverse = false;

			float_t _x = 0;
			float_t _y = 0;
			float_t _x2 = 0;
			float_t _y2 = 0;

			float_t _y0 = 0;

			float_t _pos0 = 0;
			float_t _pos = 0;
			float_t _dx = 0;
			float_t _dy = 0;
	};
	
} // namespace

#endif