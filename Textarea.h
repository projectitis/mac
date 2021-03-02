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
#ifndef _MAC_TEXTAREAH_
#define _MAC_TEXTAREAH_ 1

#include "Widget.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Text area for rendering a large block of multi-line, scrollable text
	 */
	class Textarea: public Widget {
		
		public:
			/**
			 * Create a new Textarea or take one from the pool
			 * @return The new or recycled text area
			 */
			static Textarea* create();

			/**
			 * Return this widget to the pool
			 * @param widgt [description]
			 */
			void recycle();

			/**
			 * Reset the widget back to default settings
			 */
			void reset();

			/**
			 * Set the position and size of the text area
			 * @param x 	X coord of top left corner
			 * @param y 	Y coord of top left corner
			 * @param w 	Width of area, including padding and chrome
			 * @param h 	Height of area, including padding and chrome
			 */
			void setSize( int32_t x, int32_t y, uint32_t w, uint32_t h );

			/**
			 * Set the line height of the text area
			 * @param lh 	The line height. Usually between 1 - 1.5
			 */
			void setLineHeight( float lh = 1.5 );

			/**
			 * Set the font to a mac::packedbdf_t
			 * @param f 	The font to use
			 */
			void setFont( packedbdf_t *f );

			/**
			 * Set active font color
			 * @param color  	RGR888 24-bit color
			 */
			void setColor( color888 color );

			/**
			 * Set active font alpha
			 * @param alpha  	Alpha 0 - 1
			 */
			void setAlpha( alpha alpha = 1 );

		protected:
			/**
			 * Memory pool of recycled widgets
			 */
			static Textarea* _pool;
			Textarea* _poolNext;

			/**
			 * Properties
			 */
			ClipRect 	_rect;
			packedbdf_t* _font;
			float 		_lineHeight;
			color888 	_color;
			alpha 		_alpha;
			textAlign 	_align;
	};
	
} // namespace

#endif