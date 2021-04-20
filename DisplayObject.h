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
#ifndef _MAC_DISPLAYOBJECTH_
#define _MAC_DISPLAYOBJECTH_ 1

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
	 * @brief Register of display object
	 */
	enum class DisplayObjectType {
		displayObject,
		stage,
		sprite,
	};
	
	/**
	 * The base class for all display objects (@see Stage)
	 */
	class DisplayObject: public Listener {
		
		public:
			/**
			 * Constructor
			 */
			DisplayObject();

			/**
			 * Destructor
			 */
			virtual ~DisplayObject();

			/**
			 * Create factory method. Uses memory pool
			 * for object re-use.
			 */
			template<class T>
			static DisplayObject* Create(){
				DisplayObject* obj;
				if (T::pool){
					obj = T::pool;
					T::pool = obj->_poolNext;
				}
				else{
					obj = new T();
					obj->type = T::TYPE;
				}
				return obj;
			}

			/**
			 * Return this object to the pool
			 */
			void recycle();

			/**
			 * Reset back to default settings
			 */
			virtual void reset();

			/**
			 * The type. Set in constructor of each subclass
			 */
			DisplayObjectType type;

			/**
			 * ID of the object. Implementation specific.
			 */
			uint32_t id;

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
			float x = 0;
			float y = 0;

			/**
			 * @brief Set the Visibility of the object
			 * @param v 	The visibility
			 */
			void setVisible( boolean v );

			/**
			 * Add a display object to the top of the list
			 */
			void addChild( DisplayObject* child );

			/**
			 * Add a display object before a sibling object
			 */ 
			void addChildBefore( DisplayObject* child, DisplayObject* sibling );

			/**
			 * Remove a child from the list
			 * @param 	child 		The object to remove
			 */
			void removeChild( DisplayObject* child );

			/**
			 * Remove a child from the list by ID
			 * @param 	id 		The ID of the object
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
			DisplayObject* firstChild();

			/**
			 * Return the last child
			 */
			DisplayObject* lastChild();

			/**
			 * Return a child from the list by ID
			 * @param 	id 		The ID of the object
			 */
			DisplayObject* getChild( uint32_t id );

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
			void add( DisplayObject* sibling );

			/**
			 * Get next sibling in the list
			 */
			DisplayObject* next( void );

			/**
			 * Get previous sibling in the list
			 */
			DisplayObject* prev( void );

			/**
			 * Set the position of the object on the parent. This is based on w (fractional
			 * width). For example, if w=0.5, the object will attempt to take up 50% of the
			 * available width. 
			 * @param x           		The x coord
			 * @param y           		The y coord
			 */
			virtual void position( float x, float y );

			/**
			 * Update the display object.
			 * @param	dt 			Time since last update in seconds
			 * @return 	Return self for chaining
			 */
			virtual void update( float dt );

			/**
			 * Set self and parent to dirty (recursive)
			 */
			void dirty();

			/**
			 * Populate the rect with the bounds of this display object
			 * @param inRect The rect to populate
			 */
			virtual void rect( ClipRect* inRect );

		protected:

			/**
			 * Pointer to next object in memory pool of recycled objects
			 */
			DisplayObject* _poolNext;

			/**
			 * Pool getter
			 */
			virtual DisplayObject** _getPool();
			
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
			 * Size
			 */
			float _w = 0;
			float _h = 0;

			/**
			 * List of children
			 **/
			DisplayObject* _parent = 0;
			DisplayObject* _children = 0;
			DisplayObject* _childrenTop = 0;
			DisplayObject* _next = 0;
			DisplayObject* _prev = 0;

			/**
			 * Display list order
			 */
			DisplayObject* _displayListNext = 0;
			DisplayObject* _displayListPrev = 0;
			uint32_t _depth = 0;

			/**
			 * Sort children into the display list (which is on the stage)
			 * @param stage The stage
			 * @return DisplayObject The next child (for chaining)
			 */
			DisplayObject* _sortDisplayList( DisplayObject* stage, ClipRect* displayRect );

			static int8_t compare( DisplayObject* a, DisplayObject* b ){
				if ( (int16_t)a->y < (int16_t)b->y ) return 1;	// Y is lower
				if ( (int16_t)a->y > (int16_t)b->y ) return -1;	// y is higher
				if ( (int16_t)a->x < (int16_t)b->x ) return 1;	// y is same, x is lower
				if ( (int16_t)a->x > (int16_t)b->x ) return -1;	// y is same, x is higher
				return 0;	// y and x are same
			}

	}; // class

} // namespace

#endif