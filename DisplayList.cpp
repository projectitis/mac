/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "DisplayList.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Compare two display objects by position
	 * @param a First display object
	 * @param b Second display object
	 * @return int8_t >0 if a is first, <0 if b is first, 0 if equal 
	 */
	int compareDisplayObjectByPosition( DisplayObject* a, DisplayObject* b ){
		if ( (int)a->globalRect->y < (int)b->globalRect->y ) return 1;	// Y is lower
		if ( (int)a->globalRect->y > (int)b->globalRect->y ) return -1;	// y is higher
		return (int)b->globalRect->x - (int)a->globalRect->x;
	}

	/**
	 * Compare two display objects by depth
	 * @param a First display object
	 * @param b Second display object
	 * @return int8_t >0 if a is first, <0 if b is first, 0 if equal 
	 */
	int compareDisplayObjectByDepth( DisplayObject* a, DisplayObject* b ){
		return (int)b->depth - (int)a->depth;
	}

	/**
	 * Construct a new Display List:: Display List object
	 * @param object The DisplayObject to attach
	 */
	DisplayList::DisplayList( DisplayObject* object ) {
		this->object = object;
	}

	/**
	 * Inserts an object into the list into sorted order by depth
	 * Assumes object is visible and on the display.
	 * @param object The object to insert
	 */
	void DisplayList::insertByDepth( DisplayObject* object ) {
		this->_insert( object, &compareDisplayObjectByDepth );
	}

	/**
	 * Inserts an object into the list into sorted order by position
	 * Assumes object is visible and on the display.
	 * @param object The object to insert
	 */
	void DisplayList::insertByPosition( DisplayObject* object ) {
		this->_insert( object, &compareDisplayObjectByPosition );
	}

	/**
	 * Remove a display object from the list
	 * @param object The display object to remove
	 * @return The removed node
	 */
	DisplayList* DisplayList::remove( DisplayObject *object ) {
		if (this->object == object){
			if (prev()){
				prev()->next( _next );
			}
			if (next()){
				next()->prev( _prev );
			}
			return this;
		}
		else if (next()) return ((DisplayList*)next())->remove( object );
		return 0;
	}

	/**
	 * The internal function that inserts the object into the list using the
	 * specified compare function for sorting.
	 * @param object The object to insert
	 * @param compare The compare function
	 */
	void DisplayList::_insert( DisplayObject* object, displayObjectCompareFunc compare ) {
		
		DisplayList* item;

		// If no next item, add as next item
		if (!_next){
			item = new DisplayList( object );
			after( item );
			return;
		}

		// See where to insert
		int d = compare( object, ((DisplayList*)next())->object );
		// If lower, move down the list
		if (d < 0) {
			((DisplayList*)next())->_insert( object, compare );
			return;
		}
		// If higher than next item, insert before next item (after this one)
		else {
			// Insert before this item
			item = new DisplayList( object );
			after( item );
			return;
		}

	}
	
} // namespace