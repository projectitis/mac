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

#include "IDrawable.h"
#include "Messenger.h"
#include "Tween.h"
#include "Color8888.h"

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
		text,
		shape,
		box,
	};
	
	/**
	 * The base class for all display objects (@see Stage)
	 */
	class DisplayObject: public Listener, IDrawable {
		friend class Stage;
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
			 * @brief Rect describing the bounds in global space (relative to stage)
			 * This is calculated automatically from the origin and the local bounds during the rendering
			 * sweep.
			 */
			ClipRect* globalBounds;

			/**
			 * @brief Rect describing the part of this object being rendered in the current cycle, in local space
			 * This is calculated when beginRender is called
			 */
			ClipRect* renderBounds;

			/**
			 * @brief Rect describing the last dirty area in global space (relative to stage)
			 */
			ClipRect* cleanBounds;

			/**
			 * Alpha of entire object
			 */
			float alpha = 1.0;

			/**
			 * Display depth
			 */
			uint32_t depth = 0;

			/**
			 * Set the Visibility of the object
			 * @param v 	The visibility
			 */
			void visible( boolean v );

			/**
			 * Get the visibility of an object
			 * @return boolean The visibility
			 */
			boolean visible();

			/**
			 * @brief Return parent
			 * @return DisplayObject* The parent display object, or null
			 */
			DisplayObject* parent();

			/**
			 * Return true if there is a parent
			 */
			boolean hasParent();

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
			 * @brief Set the origin x coordinate within the object
			 * @param value The origin x coordinate
			 */
			virtual void originX( float value );

			/**
			 * @return float The origin x coordinate
			 */
			virtual float originX();

			/**
			 * @brief Set the origin y coordinate within the object
			 * @param value The origin y coordinate
			 */
			virtual void originY( float value );

			/**
			 * @return float The origin y coordinate
			 */
			virtual float originY();

			/**
			 * @brief Set the x coordinate
			 * @param value The new width
			 */
			virtual void x( float value );

			/**
			 * @return float The x coordinate
			 */
			virtual float x();

			/**
			 * @brief Set the y coordinate
			 * @param value The new width
			 */
			virtual void y( float value );

			/**
			 * @return float The y coordinate
			 */
			virtual float y();

			/**
			 * @brief Set the width
			 * @param value The new width
			 */
			virtual void width( float value );

			/**
			 * @return float The width
			 */
			virtual float width();

			/**
			 * @brief Set the height
			 * @param value The new height
			 */
			virtual void height( float value );

			/**
			 * @return float The height
			 */
			virtual float height();

			/**
			 * Update the display object.
			 * @param	dt 			Time since last update in seconds
			 * @return 	Return self for chaining
			 */
			virtual void update( float dt );

			/**
			 * @brief Set the global position of the display object
			 * 
			 * @param x The global X position
			 * @param y The global Y position
			 */
			virtual void globalPos( float x, float y );

			/**
			 * @brief Convert a global X coordinate to a local coord
			 * 
			 * @param x The global X coordinate
			 * @return float The local X coordinate
			 */
			float globalToLocalX( float x );

			/**
			 * @brief Convert a global Y coordinate to a local coord
			 * 
			 * @param x The global Y coordinate
			 * @return float The local Y coordinate
			 */
			float globalToLocalY( float y );

			/**
			 * @brief Convert a rect in global coordinates to local coordinates
			 * 
			 * @param rect (out) The rext with global coordinate
			 */
			void globalToLocal( ClipRect* rect );

			/**
			 * Set dirty
			 */
			void dirty();

			/**
			 * Check if object is dirty
			 */
			boolean isDirty();

			/**
			 * @brief Begin the render sweep for the current frame
			 * The render sweep for each frame is:
			 * 		beginRender
			 * 			prepareLine y
			 * 				readPixel|readMaskPixel|skipPixel x ... x2
			 *			...
			 * 			preparLine y2
			 * 				readPixel|readMaskPixel|skipPixel x ... x2
			 * 		endRender
			 * @param updateArea The area of the display being updated
			 */
			void beginRender( ClipRect* updateArea );

			/**
			 * @Brief Initialise the next line to be read for rendering
			 * 
			 * Identified the y coordinate in local space at which the next line of pixels
			 * will be read (@see readPixel). In the same sweep, ry will always be 1 greater
			 * than ry on the last call to prepareLine
			 * 
			 * @param ry The current Y position (line) in local coordinates
			 */
			void beginLine( int16_t ry ){}

			/**
			 * @brief Calculate the pixel to be rendered at the current position
			 * 
			 * The coordinates, rx and ry, are in local coordinates. rx will always be n + 1
			 * higher than rx the previous call to calcPixel, calcMaskPixel or skipPixel for
			 * a given line (prepareLine is called at the start of a line). ry will be
			 * identical.
			 * 
			 * The renderer expects the  results of calcPixel to be in _rc (the color, 24bit RGB)
			 * and _ra (alpha 0.0 - 1.0).
			 * 
			 * @param rx The current X position in local coordinates
			 * @param ry The current Y position in local coordinates
			 */
			void calcPixel( int16_t rx, int16_t ry ){
				_rc = 0;
				_ra = 0.0;
			}

			/**
			 * @brief Calculate the pixel to be rendered at the current position as a mask
			 * Any display object can be used as a mask. In this case calcMaskPixel is called instead of
			 * calcPixel. The function only needs to calculate alpha, as the color value is ignored
			 * when used as a mask. Although it is possible to simply call calcPixel, an implementaton
			 * may be able to be done more efficiently if it is known that only alpha will be used.
			 * 
			 * @param rx The current X position in local coordinates
			 * @param ry The current Y position in local coordinates
			 */
			void calcMaskPixel( int16_t rx, int16_t ry ){
				_rc = 0;
				_ra = 1.0;
			}

			/**
			 * @brief Skip the pixel at the current position
			 * If the object is not being drawn (for example, it is masked by a higher object) then
			 * skipPixel will be called instead of calcPixel. Some implementations may require some
			 * processing, even if the pixel is not being rendered.
			 * 
			 * @param rx The current X position in local coordinates
			 * @param ry The current Y position in local coordinates
			 */
			void skipPixel( int16_t rx, int16_t ry ){}

			/**
			 * @brief End the render sweep for the current frame
			 * @see beginRender
			 */
			void endRender(){}

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
			 * X offset of origin
			 */
			float _ox = 0;

			/**
			 * Y offset of origin
			 */
			float _oy = 0;

			/**
			 * @brief Rect describing the bounds in local space
			 * 
			 * The width and height must contain the pixels completely. The x and y values are the
			 * position of the object (relative to the parent).
			 */
			ClipRect* _localBounds;

			/**
			 * @brief The color from the previous call to readPixel
			 */
			color888 _rc;

			/**
			 * @brief The alpha from the previous call to readPixel or readMaskPixel
			 */
			float _ra;

			/**
			 * List of children
			 **/
			DisplayObject* _parent = 0;
			DisplayObject* _children = 0;
			DisplayObject* _childrenTop = 0;
			DisplayObject* _next = 0;
			DisplayObject* _prev = 0;

	}; // class

} // namespace

#endif