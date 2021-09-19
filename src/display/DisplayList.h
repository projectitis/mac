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
#ifndef _MAC_DISPLAYLISTH_
#define _MAC_DISPLAYLISTH_ 1

#include "utils/LinkedList.h"
#include "display/DisplayObject.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * The compare function type
	 */
	typedef int (*displayObjectCompareFunc)( DisplayObject*, DisplayObject* );
	
	/**
	 * Display list that stores a list of display objects in render order.
	 */
	class DisplayList: public LinkedList {
		
		public:

			/**
			 * Memory pool of recycled objects
			 */
			static DisplayList* pool;

			/**
			 * Create factory method. Uses memory pool
			 * for object re-use.
			 */
			static DisplayList* Create( DisplayObject* object ){
				DisplayList* obj;
				if (DisplayList::pool){
					obj = DisplayList::pool;
					DisplayList::pool = obj->_poolNext;
				}
				else{
					obj = new DisplayList();
				}
				obj->object = object;
				return obj;
			}

			/**
			 * Return this object to the pool
			 */
			void recycle();

			/**
			 * The display object at this position
			 */
			DisplayObject* object;

			/**
			 * Return the next item
			 */
			DisplayList* next();
			
			/**
			 * Inserts an object into the list into sorted order by depth
			 * Assumes object is visible and on the display.
			 * @param object The object to insert
			 */
			void insertByDepth( DisplayObject* object );

			/**
			 * Inserts an object into the list into sorted order by position.
			 * Assumes object is visible and on the display.
			 * @param object The object to insert
			 */
			void insertByPosition( DisplayObject* object );

			/**
			 * Remove a display object from the list
			 * @param object The display object to remove
			 * @return The removed node
			 */
			DisplayList* remove( DisplayObject* object );

			/**
			 * Remove the current node from teh display list
			 * @return The removed node (self)
			 */
			DisplayList* remove();

		protected:

			/**
			 * Pointer to next object in memory pool of recycled objects
			 */
			DisplayList* _poolNext;

			/**
			 * The internal function that inserts the object into the list using the
			 * specified compare function for sorting.
			 * @param object The object to insert
			 * @param compare The compare function
			 */
			void _insert( DisplayObject* object, displayObjectCompareFunc compare );
	};
	
} // namespace

#endif