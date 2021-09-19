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
#ifndef _MAC_BORDER_H
#define _MAC_BORDER_H 1

#include "graphics/Color8888.h"

namespace mac{
	
	/**
	 * Border class
	 */
	class Border {
		
		public:

			/**
			 * Constructor
			 */
			Border();
			Border( float_t size, color888 color, float_t alpha );

			/**
			 * @brief Set size, color and alpha in one go
			 * 
			 * @param size The border size
			 * @param color The border color
			 * @param alpha The alpha
			 */
			void set( float_t size, color888 color, float_t alpha );

			/**
			 * Border size
			 */
			float_t size = 0;

			/**
			 * Border color
			 */
			color888 color = 0;

			/**
			 * Alpha
			 */
			float_t alpha = 1.0;

	};



} // ns:mac

#endif
