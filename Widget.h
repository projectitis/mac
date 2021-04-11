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
#ifndef _MAC_WIDGETH_
#define _MAC_WIDGETH_ 1

#include "Graphics.h"
#include "Style.h"
#include "Messenger.h"
#include "Tween.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * @brief Register of widget types
	 */
	enum class WidgetType {
		base,
		panelSet,
		panel,
		label,
		icon,
		guiIcon
	};
	
	/**
	 * The base class for all GUI widgets
	 */
	class Widget: public Listener {
		
		public:
			/**
			 * Constructor
			 */
			Widget( Style* aStyle = 0 );

			/**
			 * Destructor
			 */
			virtual ~Widget();

			/**
			 * Create factory method. Uses memory pool
			 * for object re-use.
			 */
			template<class T>
			static Widget* Create( Style* aStyle ){
				Widget* widget;
				if (T::pool){
					widget = T::pool;
					T::pool = widget->_poolNext;
					widget->style = aStyle;
				}
				else{
					widget = new T( aStyle );
					widget->type = T::TYPE;
				}
				return widget;
			}

			/**
			 * Return this widget to the pool
			 */
			void recycle();

			/**
			 * Reset the widget back to default settings
			 */
			virtual void reset();

			/**
			 * The widget type. Set in construtor of each subclass
			 */
			WidgetType type;

			/**
			 * ID of the widget. Implementation specific.
			 */
			uint32_t id;

			/**
			 * Styles to use for this widget
			 */
			Style* style;

			/**
			 * Listener for events
			 */
			Messenger* messenger;

			/**
			 * Tween object for animations
			 */
			Tween* tweens;

			/**
			 * Position
			 */
			int16_t x = 0;
			int16_t y = 0;

			/**
			 * Size
			 */
			float w = 1;

			/**
			 * @brief Set the Visibility of the object
			 * @param v 	The visibility
			 */
			void setVisible( boolean v );

			/**
			 * Add a display object to the top of the list
			 */
			void addChild( Widget* child );

			/**
			 * Add a display object before a sibling object
			 */ 
			void addChildBefore( Widget* child, Widget* sibling );

			/**
			 * Remove a child from the list
			 * @param 	child 		The widget to remove
			 */
			void removeChild( Widget* child );

			/**
			 * Remove a child from the list by ID
			 * @param 	id 		The ID of the widget
			 */
			void removeChild( uint32_t id );

			/**
			 * Remove all children from the list
			 * @param 	free		If true, will delete the objects from memory.
			 */
			void removeAllChildren( boolean free=true );

			/**
			 * Return the first child
			 */
			Widget* firstChild();

			/**
			 * Return the last child
			 */
			Widget* lastChild();

			/**
			 * Return a child from the list by ID
			 * @param 	id 		The ID of the widget
			 */
			Widget* getChild( uint32_t id );

			/**
			 * Return the number of children
			 */
			uint32_t numChildren();

			/**
			 * Return true if there are children
			 */
			boolean hasChildren();

			/**
			 * Add a sibling as the next one in the list
			 */
			void add( Widget* sibling );

			/**
			 * Get next sibling in the list
			 */
			Widget* next( void );

			/**
			 * Get previous sibling in the list
			 */
			Widget* prev( void );

			/**
			 * Set the position of the widget on the parent. This is based on w (fractional
			 * width). For example, if w=0.5, the widget will attempt to take up 50% of the
			 * available width. 
			 * @param x           		[description]
			 * @param y           		[description]
			 * @param availableWidth 	[description]
			 */
			virtual void position( int16_t x, uint16_t availableWidth );

			/**
			 * Update the display object.
			 * @param	dt 			Time since last update in seconds
			 * @return 	Return self for chaining
			 */
			virtual void update( float dt );

			/**
			 * Render the display object
			 * @param 	graphics 	The graphics object to use for rendering
			 * @param 	force 		If true, will render even if not dirty (force redraw)
			 * @return          Return true if current widget rendered (_dirty or forced)
			 */
			virtual boolean render( Graphics* graphics, boolean force=false );

		protected:

			/**
			 * Pointer to next widget in memory pool of recycled widgets
			 */
			Widget* _poolNext;

			/**
			 * Pool getter
			 */
			virtual Widget** _getPool();
			
			/**
			 * Flag to indicate that the object needs to be redrawn
			 */
			boolean _dirty = true;

			/**
			 * Visibility of the object. If false, object is updated but not rendered.
			 */
			boolean _visible = true;

			/**
			 * Active flag. If false, object does not accept input.
			 */
			boolean _active = true;

			/**
			 * Animation flag. If false, object does not animate.
			 */
			boolean _animate = false;

			/**
			 * List of children
			 **/
			Widget* _parent;
			Widget* _children = 0;
			Widget* _childrenTop = 0;
			Widget* _next = 0;
			Widget* _prev = 0;

	}; // class

} // namespace

#endif