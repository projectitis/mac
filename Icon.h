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
#ifndef _MAC_ICONH_
#define _MAC_ICONH_ 1

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
	 * An icon class
	 * This class is for user-defined icons. For the standard GUI icons, @see GUIIcon
	 */
	class Icon: public Widget {
		
		public:
			/**
			 * Constructor
			 */
			Icon( Style* aStyle );

			/**
			 * Memory pool of recycled icons
			 */
			static Widget* pool;

			/**
			 * Create a new icon or take one from the pool
			 * @return The new or recycled widget
			 */
			static Icon* Create( Style* style );

			/**
			 * Type identifier for this widget
			 **/
			static const WidgetType TYPE = WidgetType::icon;

			/**
			 * Reset the widget back to default settings
			 */
			void reset() override;

			/**
			 * Update the display object.
			 * @param	dt 			Time since last update in seconds
			 */
			void update( float dt ) override;

			/**
			 * Render the display object
			 * @param 	graphics 	The graphics object to use for rendering
			 * @param 	force		If true, will force render even if widget is not dirty
			 * @return          Return true if current widget rendered (_dirty or forced)
			 */
			boolean render( Graphics* graphics, boolean force ) override;

			/**
			 * @brief Set icon index
			 * 
			 * @param tilemap	The tilemap that contains the icons
			 * @param index 	The index of the icon in the tilemap
			 */
			void setIcon( Tilemap* tilemap, uint32_t index );

			/**
			 * @brief Set icon index
			 * 
			 * @param tilemap	The tilemap that contains the icons
			 * @param index 	The index of the icon in the tilemap
			 * @param color 	The color of the icon
			 */
			void setIcon( Tilemap* tilemap, uint32_t index, color888 color );

			/**
			 * @brief Set icon index
			 * 
			 * @param index 	The index of the icon in the tilemap
			 */
			void setIcon( uint32_t index );

			/**
			 * @brief Set icon index and color
			 * 
			 * @param index 	The index of the icon in the tilemap
			 * @param color 	The color of the icon
			 */
			void setIcon( uint32_t index, color888 color );

			/**
			 * Set a badge
			 * @param  count   		The number in the badge (1-99). If 0, the badge will be hidden
			 */
			void setBadge( uint8_t count );

			/**
			 * Set a badge
			 * @param  count   		The number in the badge (1-99). If 0, the badge will be hidden
			 * @param  badgeColor 	The color of the badge background circle
			 * @param  textColor 	The color of the badge text (count)
			 */
			void setBadge( uint8_t count, color888 badgeColor, color888 textColor );

			/**
			 * @brief Set icon color
			 * 
			 * @param color 	The color of the icon
			 */
			void setColor( color888 color );

		protected:
			
			/**
			 * Pool getter
			 */
			Widget** _getPool() override;

			/**
			 * Icon details
			 */
			const Tilemap* _iconTilemap = NULL;
			uint32_t _iconIndex = 0;
			uint8_t _badgeCount;
			BitmapRotation _iconRotation = BitmapRotation::none;

			/**
			 * Colors and alpha
			 */
			color888 _iconColor = 0xffffff;
			alpha _iconAlpha = 1.0;
			color888 _badgeColor = 0xff9900;
			color888 _textColor = 0xffffff;
	};
	
} // namespace

#endif