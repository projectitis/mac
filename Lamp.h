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
#ifndef _MAC_LAMPH_
#define _MAC_LAMPH_ 1

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
	 * A lamp or LED to indicate status
	 */
	class Lamp: public Widget {
		
		public:
			/**
			 * Constructor
			 */
			Lamp( Style* aStyle );
			
			/**
			 * Memory pool of recycled widgets
			 */
			static Widget* pool;

			/**
			 * Create a new widget or take one from the pool
			 * @return The new or recycled widget
			 */
			static Lamp* Create( Style* style );

			/**
			 * Type identifier for this widget
			 **/
			static const uint32_t TYPE = 3;

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
			 * Update the display object.
			 * @param	dt 			Time since last update in seconds
			 * @param	graphics 	The graphics object to use for rendering
			 * @param  	style 		The style object that holds graphical styles for GUI
			 */
			void update( float dt, Graphics* graphics, Style* style ) override;

			/**
			 * Render the display object.
			 * @param 	graphics 	The graphics object to use for rendering
			 * @param  	style 		The style object that holds graphical styles for GUI
			 * @param  	force 		If true, will render even if not dirty (force redraw)
	 		 * @return          Return true if current widget rendered (_dirty or forced)
			 */
			boolean render( Graphics* graphics, Style* style, boolean force ) override;

		protected:

			/**
			 * Pool getter
			 */
			Widget** _getPool() override;

	};
	
} // namespace

#endif