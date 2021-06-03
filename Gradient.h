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

#include "IDrawable.h"
#include "Bitmap.h"
#include "Vector2D.h"

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
	class GradientStop {
		public:
			GradientStop( color888 color, float alpha, float position );

			void set( color888 color, float alpha, float position );

			void reset( float pos, float gStep );

			void step();

			void calc();

			color888 color = 0;
			float alpha = 1.0;
			float position = 0;
			float distance = 0;
			float dStep = 0;

			GradientStop* next = 0;
			GradientStop* prev = 0;

			float r;
			float g;
			float b;
			float a;
			float dr = 0.0;
			float dg = 0.0;
			float db = 0.0;
			float da = 0.0;
	}; */
	class GradientStop {
		public:
			color888 color = 0;
			float alpha = 1.0;
			float position = 0.0;
			float distance = 0.0;
			float dStep = 0.0;
			float r = 0.0;
			float g = 0.0;
			float b = 0.0;
			float a = 0.0;
			float dr = 0.0;
			float dg = 0.0;
			float db = 0.0;
			float da = 0.0;
	};

	/**
	 * A gradient
	 */
	class Gradient: public IDrawable {
		
		public:
			/**
			 * Memory pool of recycled objects
			 */
			static Gradient* pool;

			/**
			 * Create a new object or take one from the pool
			 * @return The new or recycled object
			 */
			static Gradient* Create( uint8_t numStops );

			/**
			 * Return this object to the pool
			 */
			void recycle();

			/**
			 * Reset the object back to default settings
			 */
			void reset();

			/**
			 * @brief Construct a new Gradient object
			 */
			Gradient();

			/**
			 * @brief Destroy the Gradient object
			 */
			virtual ~Gradient();

			/**
			 * @brief Specify the start and end points
			 */
			void position( float x, float y, float x2, float y2 );

			/**
			 * @brief Set a gradient stop
			 * @param index The stop index
			 * @param color The color of the stop
			 * @param alpha The alpha at the stop
			 * @param position The position along the gradient (0.0 - 1.0)
			 * @return self, for chaining
			 */
			Gradient* stop( uint8_t index, color888 color, float alpha, float position );

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


			void skipPixel( int16_t rx, int16_t ry );

			/**
			 * @brief The color value result of calcPixel
			 */
			color888 rc;

			/**
			 * @brief The alpha value result of calcPixel
			 */
			float ra;

		protected:

			/**
			 * Pointer to next object in memory pool of recycled objects
			 */
			Gradient* _poolNext = 0;

			/**
			 * @brief The number of stops
			 */
			uint8_t numStops = 0;

			/**
			 * @brief The stops that make up this gradient
			 */
			GradientStop* stops = 0;

			/**
			 * @brief Flag to indicate if stops need recalculating
			 */
			boolean _needsCalc = true;

			float _x = 0;
			float _y = 0;
			float _x2 = 0;
			float _y2 = 0;

			float _cos = 0;
			float _sin = 0;
			float _len = 0;

			float _x0 = 0;
			float _y0 = 0;
			float _pos0 = 0;
			float _reverse = false;
			boolean _steep = false;

			float _dx = 0;
			float _dy = 0;
			float _pos = 0;

			GradientStop* activeStop = 0;

	};
	
} // namespace

#endif