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
#ifndef _MAC_GUIICONH_
#define _MAC_GUIICONH_ 1

#include "Icon.h"
#include <map>
#include <vector>

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * The GUI icon class for predefined GUI icons. Using this class requires a tilemap with a very specific layout.
	 * For user-defined icons, @see Icon
	 */
	class GUIIcon: public Icon {
		
		public:

			/**
			 * Constructor
			 */
			GUIIcon( Style* aStyle );

			/**
			 * Memory pool of recycled icons
			 */
			static Widget* pool;

			/**
			 * Create a new GUI icon or take one from the pool
			 * @return The new or recycled widget
			 */
			static GUIIcon* Create( Style* style );

			/**
			 * Type identifier for this widget
			 **/
			static const WidgetType TYPE = WidgetType::guiIcon;

			/**
			 * Render the display object
			 * @param 	graphics 	The graphics object to use for rendering
			 * @param 	force		If true, will force render even if widget is not dirty
			 * @return          Return true if current widget rendered (_dirty or forced)
			 */
			boolean render( Graphics* graphics, boolean force ) override;

		protected:
			
			/**
			 * Pool getter
			 */
			Widget** _getPool() override;
	};
	
} // namespace

#endif