/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Stage.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	DisplayObject* Stage::pool = 0;

	/**
	 * Construct a new Stage object
	 */
	Stage::Stage() {
		_dirtyRect = new ClipRect();
		_cleanRect = new ClipRect();
		_updateRect = new ClipRect();
		id = 0;
	}

	/**
	 * Destroy the Stage object
	 */
	Stage::~Stage() {
		delete _dirtyRect;
		delete _cleanRect;
		delete _updateRect;
	}

	/**
	 * Pool getter
	 */
	DisplayObject** Stage::_getPool(){
		return &Stage::pool;
	}

	/**
	 * Create a new object or take one from the pool
	 * @return The new or recycled object
	 */
	Stage* Stage::Create(){
		return (Stage*)DisplayObject::Create<Stage>();
	}

	/**
	 * Reset the object back to default settings
	 */
	void Stage::reset(){
		DisplayObject::reset();
	}

	/**
	 * Update the display object
	 */
	void Stage::update( float dt ){
		// Update self
		
		// Update children
		DisplayObject::update( dt );
	}

	/**
	 * Render all display objects
	 */
	void Stage::render( Display* display ){
		// If no children, exit early
		// XXX: Still needs to render all scanlines and draw background color
Serial.println("Stage::render");
		
		// Set stage to match display
		size( display->rect.width, display->rect.height );
_cleanRect->set( &(display->rect) );

		// Build display list. At the same time, determine the area of the display that is dirty
		// The display list is a list of all visible display objects (both with 'visibility' flag
		// set to true, and at least partially visible on the display) ordered from top to bottom
		// and left to right.
		_dirtyRect->clear();
		_displayRect = &display->rect;
		_displayListDepth = 0;
		_displayList = new DisplayList( this );
		if (_children) _traverse( _children, 0, 0 );
/*
// Debug display list
Serial.println("DisplayList");
while (node) {
	Serial.printf("  ID:%d at y=%d, x=%d (%dx%d) with d=%d\n", node->object->id, int16_t(node->object->y), int16_t(node->object->x), int16_t(node->object->width()), int16_t(node->object->height()), node->object->depth );
	node = (DisplayList*)(node->next());
}
*/
		// Calculate the updated area of ths display
		_updateRect->set( _dirtyRect );
		_updateRect->grow( _cleanRect );
		_updateRect->clip( _displayRect );

		// TODO: Handle this. For now do nothing
		if (_displayListDepth==0){}
		if (_dirtyRect->isEmpty()){}

Serial.println("Update area");
Serial.printf("  %d,%d %dx%d\n", _updateRect->x,_updateRect->y, _updateRect->width,_updateRect->height );

		// Initialise the display to draw only the dirty area
		display->set( _updateRect );

		DisplayList* head = (DisplayList*)_displayList->next();
		DisplayList* next;
		color888 c;
		float a;
		_renderList = new DisplayList( this );
		for ( uint16_t y = _updateRect->y; y <= _updateRect->y2; y++ ) {

Serial.printf("\nStart line y=%d\n", y);

			// For this line, grab the active display objects and add to the render list
			while ( head && ( head->object->y <= y ) ) {
				_renderList->insertByDepth( head->object );
				head = (DisplayList*)head->next();
			}
/*
Serial.println("RenderList\n");
node = _renderList;
while (node) {
	Serial.printf("  ID:%d at y=%d, x=%d (%dx%d) with d=%d\n", node->object->id, int16_t(node->object->y), int16_t(node->object->x), int16_t(node->object->width()), int16_t(node->object->height()), node->object->depth );
	node = (DisplayList*)node->next();
}
*/
			// For this line, remove the display objects that are complete, and set the
			// start read position for those that aren't. Skip the first one because it's
			// the stage.
			DisplayList* node = (DisplayList*)_renderList->next();
			while (node) {
				// Todo precalculate BR corner (x2 and y2) 
				if ( y > node->object->globalRect->y2 ){
					next = (DisplayList*)node->next();
					delete ((LinkedList*)node)->remove();
					node = next;
				}
				else {
					node->object->readPosition( _updateRect->x, y );
					node = (DisplayList*)node->next();
				}
			}

			// Step pixels in this line
			for ( uint16_t x = _updateRect->x; x <= _updateRect->x2; x++ ) {
				
				// Base color
				display->pixel( this->_backgroundColor, x );

				// Now step through active display objects and composite pixel to line buffer
				node = (DisplayList*)_renderList->next();
				while (node) {
					if ( node->object->globalRect->containsX(x) ){
						node->object->readPixel( c, a );
						a = alphaClamp( a * node->object->alpha );
						if (a > 0) display->blend( c, a, x );
					}
					node = (DisplayList*)node->next();
				}
			}

			// Flip the buffer (auto-advances to next line)
			display->flip();
		}
		_clearList( _renderList );
		_clearList( _displayList );

		_cleanRect->set( _dirtyRect );
	}

	/**
	 * Set the background color
	 * @param bgColor The background color
	 */
	void Stage::backgroundColor( color888 bgColor ) {
		_backgroundColor = bgColor;
	}

	/**
	 * Get the background color
	 * @return color The background color
	 */
	color888 Stage::backgroundColor() {
		return _backgroundColor;
	}

	/**
	 * Step recursively through all children. Calculate the relative depth of
	 * children, and insert into the display list.
	 * @param children The children to add
	 * @param px The global x coordinate of the parent
	 * @param py The global y coordinate of the parent
	 */
	void Stage::_traverse( DisplayObject* child, float px, float py ) {
		// Step all children
		while (child) {

			// Visible?
			if (!child->visible() || (child->alpha <= 0)) {
				// TODO: If dirty, still need to include in rect?
				child = child->next();
				continue;
			}

			// Set global position and sizing
			child->globalRect->setPosAndSize(
				px + child->x,
				py + child->y,
				child->width() + 0.5,
				child->height() + 0.5
			);

			// On the display?
			if (!child->globalRect->overlaps( _displayRect )){
				child = child->next();
				continue;
			}

			// Calculate depth
			child->depth = ++_displayListDepth;

			// Add to display list
			_displayList->insertByPosition( child );

			// Calculate screen dirty area
			if (child->isDirty()) _dirtyRect->grow( child->globalRect );
			// TODO: Child to store old rect, so when dirty, dirty rect includes the old rect!

			// Recurse
			if (child->hasChildren()) _traverse( child->firstChild(), child->globalRect->x, child->globalRect->y );

			// Next sibling
			child = child->next();
		}
	}

	/**
	 * Clear a DisplayList
	 * @param list The list to clear
	 */
	void Stage::_clearList( DisplayList* list ) {
		DisplayList* next = list;
		while (next) {
			next = (DisplayList*)list->next();
			delete list;
		}
	}
	
} // namespace