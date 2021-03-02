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
#ifndef _MAC_PANELSETH_
#define _MAC_PANELSETH_ 1

#include "Panel.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * A panel set
	 * A panel set is sort of the equivalent of a window. It has a menu that allows the player
	 * to select the panels that are in the set. Only the selected panel is active.
	 */
	class PanelSet: public Widget {
		
		public:
			/**
			 * Constructor
			 */
			PanelSet( Style* aStyle );

			/**
			 * Memory pool of recycled widgets
			 */
			static Widget* pool;

			/**
			 * Create a new widget or take one from the pool
			 * @return The new or recycled widget
			 */
			static PanelSet* Create( Style* style );

			/**
			 * Type identifier for this widget
			 **/
			static const uint32_t TYPE = 1;

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
			boolean render( Graphics* graphics, boolean force=false ) override;

			/**
			 * Add a new panel and return it
			 * @param  title   	The full title of the panel (max 32 chars)
			 * @param  acronym 	The acronym to display when collapsed (max 3 chars)
			 * @return          The new panel
			 */
			Panel* addPanel( char* title, char* acronym	);

			/**
			 * Return a panel by acronym. Will return the first match
			 * @param  acronym 	The acronym of the panel
			 * @return         	The panel, or 0 if not found
			 */
			Panel* panel( const char* acronym );

		protected:
			
			/**
			 * Pool getter
			 */
			Widget** _getPool() override;

			/**
			 * Calculate an ID based on a string (uses up to 4 characters)
			 */
			uint32_t _getId( char* str );

			/**
			 * Internal
			 **/
			Panel* _activePanel;
			boolean _menuOpen;
			float _menuWidth;
			void _drawArrowLeft( Graphics* graphics, int16_t y );
			void _drawArrowRight( Graphics* graphics, int16_t y );
			void _drawBackground( Graphics* graphics );
	};
	
} // namespace

#endif