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
	 */
	class GradientStop {
		public:
			GradientStop( color888 color, float alpha, float position );

			void set( color888 color, float alpha, float position );

			void update();

			color888 color = 0;
			float alpha = 1.0;
			float position = 0;

			GradientStop* next = 0;

			float r;
			float g;
			float b;
			float a;
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
			static Gradient* Create();

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
			void set( float x, float y, float x2, float y2 );

			color888 rc(){ return _rc; }

			float ra(){ return _ra; }

			/**
			 * @brief Add a gradient stop
			 * A stop at the same position as another will replace the first
			 * @param color The color of the stop
			 * @param alpha The alpha at the stop
			 * @param position The position along the gradient (0.0 - 1.0)
			 */
			void setStop( color888 color, float alpha, float position );

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

		protected:

			/**
			 * Pointer to next object in memory pool of recycled objects
			 */
			Gradient* _poolNext = 0;

			/**
			 * @brief The color from the previous call to readPixel
			 */
			color888 _rc = 0;

			/**
			 * @brief The alpha from the previous call to readPixel or readMaskPixel
			 */
			float _ra = 1.0;

			/**
			 * @brief The stops that make up this gradient
			 */
			GradientStop* stops = 0;

			float _x = 0;
			float _y = 0;
			float _x2 = 0;
			float _y2 = 0;
			float _a = 0;
			float _m = 0;

			int16_t _updateX = 0;
			float _xStep = 0;
			float _yStep = 0;
			float _pos = 0;
			GradientStop* activeStop = 0;

	};
	
} // namespace

#endif