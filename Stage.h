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
#ifndef _MAC_STAGEH_
#define _MAC_STAGEH_ 1

#include "DisplayList.h"
#include "Display.h"

// Include all display object types
#include "DisplayObject.h"
#include "Sprite.h"
#include "Text.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * The main stage object
	 */
	class Stage: public DisplayObject {
		
		public:
			/**
			 * Construct a new Stage object
			 */
			Stage();

			/**
			 * Destroy the Stage objectD
			 */
			~Stage();

			/**
			 * Memory pool of recycled objects
			 */
			static DisplayObject* pool;

			/**
			 * Create a new object or take one from the pool
			 * @return The new or recycled object
			 */
			static Stage* Create();

			/**
			 * Type identifier for this object
			 **/
			static const DisplayObjectType TYPE = DisplayObjectType::stage;

			/**
			 * Reset the object back to default settings
			 */
			void reset() override;

			/**
			 * Update all objects on the stage. This will advance animations, messages etc but
			 * will not update the objects to the display.
			 * @param	dt 			Time since last update in seconds
			 */
			void update( float dt ) override;

			/**
			 * Render the objects to the display 
			 */
			void render( Display* display );

			/**
			 * Set the background color
			 * @param bgColor The background color
			 */
			void backgroundColor( color888 bgColor );

			/**
			 * Get the background color
			 * @return color The background color
			 */
			color888 backgroundColor();

		protected:
			
			/**
			 * Pool getter
			 */
			DisplayObject** _getPool() override;

			/**
			 * Stage color
			 */
			color888 _backgroundColor = 0;

			/**
			 * Area of the display that is dirty
			 */
			ClipRect* _dirtyRect;

			/**
			 * Area of the display that is clean (just written)
			 */
			ClipRect* _cleanRect;

			/**
			 * Temporary rect that describes are of display being updated
			 */
			ClipRect* _updateRect;

			/**
			 * Temporary rect to hold bounds of the display
			 */
			ClipRect* _displayRect;

			/**
			 * Increments as the display list is traversed. Used to calculate
			 * relative depth of all children
			 */
			uint32_t _displayListDepth;

			/**
			 * Ordered list of all visible display objects in the order that they
			 * are added to the render list.
			 */
			DisplayList* _displayList;

			/**
			 * Ordered list of visible display objects to be rendered
			 * on the current scan line.
			 */
			DisplayList* _renderList;

			/**
			 * Step recursively through all children. Calculate the relative depth of
			 * children, and insert into the display list.
			 * @param children The children to add
			 */
			void _traverse( DisplayObject* child, float px, float py );

			/**
			 * Clear a DisplayList
			 * @param list The list to clear
			 */
			void _clearList( DisplayList* list );

	};
	
} // namespace

#endif