/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "LinkedList.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Return the next item
	 */
	LinkedList* LinkedList::next(){
		return _next;
	}

	/**
	 * Return the previous item
	 */
	LinkedList* LinkedList::prev(){
		return _prev;
	}
	
	/**
	 * Add an item to the end of the linked list. Item can be a single item or
	 * the start of a list.
	 */
	void LinkedList::add( LinkedList* item ){
		if (_next) _next->add( item );
		else{
			item->_prev = this;
			_next = item;
		}
	}

	/**
	 * Insert an item as the previous item in the list. Only works with single item.
	 */
	void LinkedList::before( LinkedList* item ){
		item->_next = this;
		item->_prev = 0;
		if (_prev){
			_prev->_next = item;
			item->_prev = _prev;
		}
		_prev = item;
	}

	/**
	 * Insert an item as the next item in the list. Only works with single item.
	 */
	void LinkedList::after( LinkedList* item ){
		item->_prev = this;
		item->_next = 0;
		if (_next){
			_next->_prev = item;
			item->_next = _next;
		}
		_next = item;
	}

	/**
	 * Remove the current item from the list
	 * @return	Pointer to the next item in the list
	 */
	LinkedList* LinkedList::remove(){
		if (_prev){
			_prev->_next = _next;
		}
		if (_next){
			_next->_prev = _prev;
		}
		return _next;
	}

	/**
	 * Remove the specified item from the list
	 * @return	Pointer to the next item in the list
	 */
	LinkedList* LinkedList::remove( LinkedList* item ){
		if (this == item) return remove();
		if (_next) return _next->remove( item );
		return 0;
	}
	
} // namespace