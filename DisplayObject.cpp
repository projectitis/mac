/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "DisplayObject.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Constructor
	 */
	DisplayObject::DisplayObject(){}

	/**
	 * Destructor
	 **/
	DisplayObject::~DisplayObject(){
		reset();
	}

	/**
	 * Pool getter
	 */
	DisplayObject** DisplayObject::_getPool(){
		return 0;
	}

	/**
	 * Return this widget to the pool
	 */
	void DisplayObject::recycle(){
		this->reset();
		DisplayObject** pool = _getPool();
		if (pool==0){
			delete this;
		}
		else{
			this->_poolNext = *pool;
			*pool = this;
		}
	}

	/**
	 * Reset the widget back to default settings
	 */
	void DisplayObject::reset(){
		removeAllChildren();
		id = 0;
		x = 0.0;
		y = 0.0;
		_dirty = true;
		_visible = true;
		_active = true;
	}

	void DisplayObject::setVisible( boolean v ) {
		_visible = v;
		if (v) dirty();
	}

	/**
	 * Populate the rect with the bounds of this display object
	 * @param inRect The rect to populate
	 */
	void DisplayObject::rect( ClipRect* inRect ){
		inRect->setPosAndSize( (int16_t)x, (int16_t)y, (int16_t)_w, (int16_t)_h );
	}

	/**
	 * Add a child to the list
	 */
	void DisplayObject::addChild( DisplayObject* child ){
//Serial.println("DisplayObject::addChild");
		if (!_children){
			_children = child;
			child->_parent = this;
			child->_next = 0;
		}
		else{
			_childrenTop->add( child );
		}
		_childrenTop = child;
		dirty();
	}

	/**
	 * Remove a child from the list
	 * @param 	child 		The widget to remove
	 */
	void DisplayObject::removeChild( DisplayObject* child ){
		if (!_children) return;
		DisplayObject* displayObject = _children;
		while (displayObject){
			if (displayObject == child){
				if (displayObject->_next){
					displayObject->_next->_prev = displayObject->_prev;
				}
				if (displayObject->_prev){
					displayObject->_prev->_next = displayObject->_next;
				}
				if (_children==displayObject){
					_children = displayObject->_next;
				}
				if (_childrenTop==displayObject){
					_childrenTop = displayObject->_prev;
				}
				dirty();
				break;
			}
			displayObject = displayObject->next();
		}
	}

	/**
	 * Remove a child from the list by ID
	 * @param 	id 		The ID of the widget to remove
	 */
	void DisplayObject::removeChild( uint32_t id ){
		if (!_children) return;
		DisplayObject* displayObject = _children;
		while (displayObject){
			if (displayObject->id == id){
				if (displayObject->_next){
					displayObject->_next->_prev = displayObject->_prev;
				}
				if (displayObject->_prev){
					displayObject->_prev->_next = displayObject->_next;
				}
				if (_children==displayObject){
					_children = displayObject->_next;
				}
				if (_childrenTop==displayObject){
					_childrenTop = displayObject->_prev;
				}
				dirty();
				break;
			}
			displayObject = displayObject->next();
		}
	}

	/**
	 * Remove all children from the list
	 */
	void DisplayObject::removeAllChildren( boolean free ){
		if (!_children) return;
		DisplayObject* displayObject = _children;
		DisplayObject* nextDisplayObject;
		while (displayObject){
			nextDisplayObject = displayObject->next();
			if (free) delete displayObject;
			else{
				displayObject->_parent = 0;
				displayObject->_next = 0;
				displayObject->_prev = 0;
			}
			displayObject = nextDisplayObject;
		}
		_children = 0;
		_childrenTop = 0;
		dirty();
	}

	/**
	 * Return the first child
	 */
	DisplayObject* DisplayObject::firstChild(){
		return _children;
	}

	/**
	 * Return the first child
	 */
	DisplayObject* DisplayObject::lastChild(){
		DisplayObject* displayObject = _children;
		DisplayObject* nextDisplayObject;
		while (displayObject){
			nextDisplayObject = displayObject->next();
			if (!nextDisplayObject) return displayObject;
			displayObject = nextDisplayObject;
		}
		return 0;
	}

	/**
	 * Return a child from the list by ID
	 * @param 	id 		The ID of the widget
	 */
	DisplayObject* DisplayObject::getChild( uint32_t id ){
		if (!_children) return 0;
		DisplayObject* displayObject = _children;
		while (displayObject){
			if (displayObject->id == id) return displayObject;
			displayObject = displayObject->next();
		}
		return 0;
	}

	/**
	 * Return the number of children
	 */
	uint32_t DisplayObject::numChildren(){
		uint32_t count = 0;
		DisplayObject* displayObject = _children;
		while (displayObject){
			count++;
			displayObject = displayObject->next();
		}
		return count;
	}

	/**
	 * Return true if there are children
	 */
	boolean DisplayObject::hasChildren(){
		return (boolean)_children;
	}

	/**
	 * Get next sibling in the list
	 */
	DisplayObject* DisplayObject::next( void ){
		return _next;
	}

	/**
	 * Get previous sibling in the list
	 */
	DisplayObject* DisplayObject::prev( void ){
		return _prev;
	}

	/**
	 * Add or insert a sibling after this one
	 */
	void DisplayObject::add( DisplayObject* sibling ){
		if (_next){
			_next->_prev = sibling;
			sibling->_next = _next;
		}
		else{
			sibling->_next = 0;
		}
		_next = sibling;
		sibling->_prev = this;
		sibling->_parent = _parent;
	}

	/**
	 * Update and render the display object
	 */
	void DisplayObject::update( float dt ){
		if (!_children) return;
		DisplayObject* displayObject = _children;
		while (displayObject){
			displayObject->update( dt );
			displayObject = displayObject->next();
		}
	}

	/**
	 * @brief Set the position of the object
	 * @param x 	The x coord
	 * @param y 	The y coord
	 */
	void DisplayObject::position( float x, float y ){
		this->x = x;
		this->y = y;
		dirty();
	}

	/**
	 * Set self and parent to dirty (recursive)
	 */
	void DisplayObject::dirty() {
		_dirty = true;
		//if (_parent) _parent->dirty();
	}

	/**
	 * Sort children into the display list (which is on the stage)
	 * @param stage The stage
	 * @return DisplayObject The next child (for chaining)
	 */
	DisplayObject* DisplayObject::_sortDisplayList( DisplayObject* stage, ClipRect* displayRect ) {
//Serial.println("DisplayObject::_sortDisplayList");
		// Step through children and sort onto display list
		DisplayObject* child = _children;
		ClipRect* childRect = new ClipRect();
		while (child) {
//Serial.printf("  Child %d\n", child->id);

			// Sort onto display list
			if (child->_visible) {

				// Skip if not within display area
				child->rect( childRect );
				if (!childRect->overlaps( displayRect )){
					child = child->_next;
					continue;
				}

				// Set depth
				child->_depth = ++stage->_depth;

				// Insertion sort
				child->_displayListNext = 0;
				child->_displayListPrev = 0;
				// If child is the first item, place it at the start
				if (!stage->_displayListNext) {
//Serial.println("    First in display list");
					stage->_displayListNext = child;
				}
				// Otherwise find insertion point
				else {
					// Step along the chain until the next child is smaller
					DisplayObject* obj = stage->_displayListNext;
					while (obj) {
//Serial.printf("    Compare to display list: %d\n", obj->id);
						// If the child is higher than current node, insert before node
						if ( DisplayObject::compare( child, obj ) > 0 ) {
//Serial.println("        Higher");
							// All except the first node, link child to prev
							if (obj->_displayListPrev){
								obj->_displayListPrev->_displayListNext = child;
								child->_displayListPrev = obj->_displayListPrev;
							}
							else {
								stage->_displayListNext = child;
							}
							// Link child to next
							obj->_displayListPrev = child;
							child->_displayListNext = obj;
							// Done
							break;
						}
						// If child wasn't inserted, place it at the end
						if (!obj->_displayListNext) {
//Serial.println("        Lower, reached end");
							obj->_displayListNext = child;
							child->_displayListPrev = obj;
							break;
						}
						obj = obj->_displayListNext;
					}
				}
			} // visible

			// Update dirty region. Note - this happens even if the child is
			// not visible. It may be dirty because it's just become hidden, so the
			// display needs to be updated to reflect this.
			if (child->_dirty) {
				// XXX: DisplayObject needs a clipRect
				//_dirtyRect.add( child->rect );
			}

			// Now recurse children of the this child
			child->_sortDisplayList( stage, displayRect );

			// Move to next sibling
			child = child->_next;
		}
		delete childRect;

		// Return next sibling
		return _next;
	}
	
} // namespace