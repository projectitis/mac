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

#include "display/DisplayList.h"
#include "display/LineBuffer.h"
 // Display object types
#include "display/DisplayObject.h"
#include "display/Sprite.h"
#include "display/Text.h"
#include "display/Shape.h"
#include "display/Box.h"
// Gradients
#include "display/LinearGradient.h"
// Filters
#include "display/filters/TintFilter.h"
#include "display/filters/ColorMaskFilter.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac {

	/**
	 * The main stage object
	 */
	class Stage : public DisplayObject {

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
		void update( float_t dt ) override;

		/**
		 * Render the objects to the display
		 */
		void render( LineBuffer* buffer );

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

		/**
		 * @brief Enable visual debugging of update area
		 */
		boolean debug = false;

		/**
		 * @brief Color for visual debugging
		 */
		color888 debugColor = ARGB8888_IndianRed;

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
		ClipRect* _dirtyBounds;

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
		 * The line buffer
		 */
		LineBuffer* _lineBuffer;

		/**
		 * Step recursively through all children. Calculate the relative depth of
		 * children, and insert into the display list.
		 * @param children The children to add
		 */
		void _traverse( LineBuffer* buffer, DisplayObject* child, boolean forceDirty, float_t px, float_t py );

		/**
		 * Clear a DisplayList
		 * @param list The list to recycle
		 */
		void _recycleList( DisplayList* list );

	};

} // namespace

#endif