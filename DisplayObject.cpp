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
	DisplayObject::DisplayObject(){
		globalBounds = new ClipRect();
		_localBounds = new ClipRect();
		cleanBounds = new ClipRect();
		renderBounds = new ClipRect();
	}

	/**
	 * Destructor
	 **/
	DisplayObject::~DisplayObject(){
		reset();
		delete renderBounds;
		delete cleanBounds;
		delete _localBounds;
		delete globalBounds;
	}

	/**
	 * Pool getter
	 */
	DisplayObject** DisplayObject::_getPool(){
		return 0;
	}

	/**
	 * Return this object to the pool
	 */
	void DisplayObject::recycle(){
#ifdef MAC_OBJECT_REUSE
		this->reset();
		DisplayObject** pool = _getPool();
		if (pool==0){
			delete this;
		}
		else{
			this->_poolNext = *pool;
			*pool = this;
		}
#else
		delete this;
#endif
	}

	/**
	 * Reset the object back to default settings
	 */
	void DisplayObject::reset(){
		removeAllChildren();
		id = 0;
		_ox = 0.0;
		_oy = 0.0;
		_dirty = true;
		_visible = true;
		_active = true;
		width(0);
		height(0);
		globalBounds->clear();
		_localBounds->clear();
		cleanBounds->clear();
		renderBounds->clear();
	}

	void DisplayObject::visible( boolean v ) {
		_visible = v;
		_dirty = true;
	}

	/**
	 * Get the visibility of an object
	 * @return boolean The visibility
	 */
	boolean DisplayObject::visible(){
		return _visible;
	}

	/**
	 * @brief Return parent
	 * @return DisplayObject* The parent display object, or null
	 */
	DisplayObject* DisplayObject::parent() {
		return _parent;
	}

	/**
	 * Return true if there is a parent
	 */
	boolean DisplayObject::hasParent() {
		return (_parent != 0);
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
				displayObject->_parent = 0;
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
				displayObject->_parent = 0;
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
	 * @brief Set the origin x coordinate within the object
	 * @param value The origin x coordinate
	 */
	void DisplayObject::originX( float value ) {
		_ox = -value;
		dirty();
	}

	/**
	 * @return float The origin x coordinate
	 */
	float DisplayObject::originX() {
		return -_ox;
	}

	/**
	 * @brief Set the origin y coordinate within the object
	 * @param value The origin y coordinate
	 */
	void DisplayObject::originY( float value ) {
		_oy = -value;
		dirty();
	}

	/**
	 * @return float The origin y coordinate
	 */
	float DisplayObject::originY() {
		return -_oy;
	}

	/**
	 * @brief Set the x coordinate
	 * @param value The x coordinate
	 */
	void DisplayObject::x( float value ) {
		_localBounds->setPos( value, _localBounds->y );
		dirty();
	}

	/**
	 * @return float The x coordinate
	 */
	float DisplayObject::x() {
		return _localBounds->x;
	}

	/**
	 * @brief Set the y coordinate
	 * @param value The y coordinate
	 */
	void DisplayObject::y( float value ) {
		_localBounds->setPos( _localBounds->x, value );
		dirty();
	}

	/**
	 * @return float The y coordinate
	 */
	float DisplayObject::y() {
		return _localBounds->y;
	}

	/**
	 * @brief Set the width
	 * @param value The new width
	 */
	void DisplayObject::width( float value ) {
		_localBounds->setWidth( (value > 0)?value:0 );
		dirty();
	}

	/**
	 * @return float The width
	 */
	float DisplayObject::width() {
		return _localBounds->width;
	}

	/**
	 * @brief Set the height
	 * @param value The new height
	 */
	void DisplayObject::height( float value ) {
		_localBounds->setHeight( (value > 0)?value:0 );
		dirty();
	}

	/**
	 * @return float The height
	 */
	float DisplayObject::height() {
		return _localBounds->height;
	}

	/**
	 * Set self to dirty
	 */
	void DisplayObject::dirty() {
		if (_visible) _dirty = true;
	}

	/**
	 * Check if object is dirty
	 */
	boolean DisplayObject::isDirty() {
		return _dirty;
	}

	/**
	 * @brief Set the global position of the display object
	 * 
	 * @param x The global X position
	 * @param y The global Y position
	 */
	void DisplayObject::globalPos( float x, float y ) {
		globalBounds->setPosAndSize(
			x + _ox + _localBounds->x,
			y + _oy + _localBounds->y,
			_localBounds->width,
			_localBounds->height
		);
	}

	/**
	 * @brief Convert a global X coordinate to a local coord
	 * 
	 * @param x The global X coordinate
	 * @return float The local X coordinate
	 */
	float DisplayObject::globalToLocalX( float x ) {
		return x - (float)(globalBounds->x);
	}

	/**
	 * @brief Convert a global Y coordinate to a local coord
	 * 
	 * @param x The global Y coordinate
	 * @return float The local Y coordinate
	 */
	float DisplayObject::globalToLocalY( float y ) {
		return y - (float)(globalBounds->y);
	}

	/**
	 * @brief Convert a rect in global coordinates to local coordinates
	 * 
	 * @param rect (out) The rext with global coordinate
	 */
	void DisplayObject::globalToLocal( ClipRect* rect ) {
		rect->translate( -globalBounds->x, -globalBounds->y );
	}

	/**
	 * @brief Begin the render sweep for the current frame
	* @param updateArea The area of the display being updated
	*/
	void DisplayObject::beginRender( ClipRect* updateArea ) {
		_dirty = false;
		renderBounds->set( updateArea );
		renderBounds->clip( globalBounds );
		globalToLocal( renderBounds );
	}
	
} // namespace