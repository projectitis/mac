/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Widget.h"

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
	Widget::Widget( Style* aStyle ){
		this->style = aStyle;
	}

	/**
	 * Destructor
	 **/
	Widget::~Widget(){
		reset();
		this->style = 0;
	}

	/**
	 * Pool getter
	 */
	Widget** Widget::_getPool(){
		return 0;
	}

	/**
	 * Return this widget to the pool
	 */
	void Widget::recycle(){
		this->reset();
		Widget** pool = _getPool();
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
	void Widget::reset(){
		removeAllChildren();
		this->id = 0;
		this->x = 0;
		this->y = 0;
		this->w = 1;
		this->_dirty = true;
		this->_visible = true;
		this->_active = true;
	}

	/**
	 * Add a child to the list
	 */
	void Widget::addChild( Widget* child ){
		child->_parent = this;
		child->_next = 0;
		if (!_children){
			_children = child;
			_childrenTop = _children;
		}
		else _childrenTop->add( child );
	}

	/**
	 * Remove a child from the list
	 * @param 	child 		The widget to remove
	 */
	void Widget::removeChild( Widget* child ){
		if (!_children) return;
		Widget* widget = _children;
		while (widget){
			if (widget == child){
				if (widget->_next){
					widget->_next->_prev = widget->_prev;
				}
				if (widget->_prev){
					widget->_prev->_next = widget->_next;
				}
				if (_children==widget){
					_children = widget->_next;
				}
				if (_childrenTop==widget){
					_childrenTop = widget->_prev;
				}
				break;
			}
			widget = widget->next();
		}
	}

	/**
	 * Remove a child from the list by ID
	 * @param 	id 		The ID of the widget to remove
	 */
	void Widget::removeChild( uint32_t id ){
		if (!_children) return;
		Widget* widget = _children;
		while (widget){
			if (widget->id == id){
				if (widget->_next){
					widget->_next->_prev = widget->_prev;
				}
				if (widget->_prev){
					widget->_prev->_next = widget->_next;
				}
				if (_children==widget){
					_children = widget->_next;
				}
				if (_childrenTop==widget){
					_childrenTop = widget->_prev;
				}
				break;
			}
			widget = widget->next();
		}
	}

	/**
	 * Remove all children from the list
	 */
	void Widget::removeAllChildren( boolean free ){
		if (!_children) return;
		Widget* widget = _children;
		Widget* nextWidget;
		while (widget){
			nextWidget = widget->next();
			if (free) delete widget;
			else{
				widget->_parent = 0;
				widget->_next = 0;
				widget->_prev = 0;
			}
			widget = nextWidget;
		}
		_children = 0;
		_childrenTop = 0;
	}

	/**
	 * Return the first child
	 */
	Widget* Widget::firstChild(){
		return _children;
	}

	/**
	 * Return a child from the list by ID
	 * @param 	id 		The ID of the widget
	 */
	Widget* Widget::getChild( uint32_t id ){
		if (!_children) return 0;
		Widget* widget = _children;
		while (widget){
			if (widget->id == id) return widget;
			widget = widget->next();
		}
		return 0;
	}

	/**
	 * Return the number of children
	 */
	uint32_t Widget::numChildren(){
		uint32_t count = 0;
		Widget* widget = _children;
		while (widget){
			count++;
			widget = widget->next();
		}
		return count;
	}

	/**
	 * Return true if there are children
	 */
	boolean Widget::hasChildren(){
		return (boolean)_children;
	}

	/**
	 * Get next sibling in the list
	 */
	Widget* Widget::next( void ){
		return _next;
	}

	/**
	 * Get previous sibling in the list
	 */
	Widget* Widget::prev( void ){
		return _prev;
	}

	/**
	 * Add or insert a sibling after this one
	 */
	void Widget::add( Widget* sibling ){
		if (_next){
			_next->_prev = sibling;
			sibling->_next = _next;
		}
		else{
			sibling->_next = 0;
			_childrenTop = sibling;
		}
		_next = sibling;
		sibling->_parent = _parent;
	}

	/**
	 * Update and render the display object
	 */
	void Widget::update( float dt ){
		if (!_children) return;
		Widget* widget = _children;
		while (widget){
			widget->update( dt );
			widget = widget->next();
		}
	}

	void Widget::position( int16_t x, uint16_t availableWidth ){
		
	}

	/**
	 * Render the children. Return true if any children were rendered. This may mean
	 * that the parent will need to render also.
	 * @param  	graphics 	The graphics object to use for rendering
	 * @param	force		Force the widget to redraw even if it isn't dirty
	 * @return          Return true if current widget rendered (_dirty or forced)
	 */
	boolean Widget::render( Graphics* graphics, boolean force ){
		if (!_children) return false;
		boolean childRendered = false;
		Widget* widget = _children;
		while (widget){
			childRendered |= widget->render( graphics, force );
			widget = widget->next();
		}
		return childRendered;
	}
	
} // namespace