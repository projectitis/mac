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
#ifndef _MAC_STYLEH_
#define _MAC_STYLEH_ 1

#include "Common.h"
#include "fonts/OpenSans11.h"
#include "fonts/OpenSansExtraBold11.h"
#include "resources/gui_controls.h"
#include "resources/gui_icons.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Styles for GUI
	 */
	class Style {
		
		public:
			/**
			 * Screen color
			 */
			color888 screenColor 					= 0x151617;

			/**
			 * Panel colors
			 */
			color888 panelBackgroundColor[2] 		= { 0x1c1d1e, 0x222324 };
			color888 panelBorderColor[2] 			= { 0x121314, 0x18191a };
			color888 panelHighlightColor[2] 		= { 0x202122, 0x262728 };

			/**
			 * Fonts
			 */
			packedbdf_t* font 						= (packedbdf_t*)&OpenSans11;
			packedbdf_t* labelFont 					= (packedbdf_t*)&OpenSans11;

			/**
			 * Text colors
			 */
			color888 textColor 						= 0xecf0f1;
			color888 textInactiveColor 				= 0x95a5a6;

			color888 labelColor 					= 0x7f8c8d;
			color888 labelInactiveColor 			= 0x454646;

			/**
			 * Feature colours
			 */
			color888 featureColor[7] 				= { 0xf2c500, 0xe87e04, 0xe94b35, 0x00bd9c, 0x1fce6d, 0x2c97de, 0x9c56b8 };
			color888 featureInactiveColor[7] 		= { 0xf59d00, 0xd55400, 0xc23824, 0x00a185, 0x1aaf5d, 0x227fbb, 0x8f3faf };

			/**
			 * Icon bitmaps
			 */
			const Tilemap* controls 				= &guiControls;
			const Tilemap* icons 					= &guiIcons;

			/**
			 * Icon colors
			 */
			color888 iconColor						= 0xecf0f1;
			color888 badgeTextColor 				= 0xecf0f1;
			color888 badgeColor 					= 0xe94b35;
	};
	
} // namespace

#endif