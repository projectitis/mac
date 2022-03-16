/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "../Stage.h"

 /**
  * mac (or μac) stands for "Microprocessor App Creator"
  * mac is a project that enables creating beautiful and useful apps on the
  * Teensy microprocessor, but hopefully is generic enough to be ported to other
  * microprocessor boards. The various libraries that make up mac might also
  * be useful in other projects.
  **/
namespace mac {

	DisplayObject* Stage::pool = 0;

	/**
	 * Construct a new Stage object
	 */
	Stage::Stage() {
		_dirtyBounds = new ClipRect();
		id = 0;
	}

	/**
	 * Destroy the Stage object
	 */
	Stage::~Stage() {
		delete _dirtyBounds;
	}

	/**
	 * Pool getter
	 */
	DisplayObject** Stage::_getPool() {
		return &Stage::pool;
	}

	/**
	 * Create a new object or take one from the pool
	 * @return The new or recycled object
	 */
	Stage* Stage::Create() {
		return (Stage*)DisplayObject::Create<Stage>();
	}

	/**
	 * Reset the object back to default settings
	 */
	void Stage::reset() {
		DisplayObject::reset();
	}

	/**
	 * Update the display object
	 */
	void Stage::update( float_t dt ) {
		// Update self

		// Update children
		DisplayObject::update( dt );
	}

	/**
	 * Render all display objects
	 */
	void Stage::render( LineBuffer* buffer ) {
		//Serial.println("Stage::render");

		// Set stage to match display
		_localBounds->setSize( buffer->rect.width, buffer->rect.height );

		// Build display list. At the same time, determine the area of the display that is dirty
		// The display list is a list of all visible display objects (both with 'visibility' flag
		// set to true, and at least partially visible on the display) ordered from top to bottom
		// and left to right.
		if ( _dirty ) _dirtyBounds->set( &buffer->rect );
		else _dirtyBounds->clear();
		_displayListDepth = 0;
		_displayList = DisplayList::Create( this );
		if ( _children ) _traverse( buffer, _children, _dirty, 0, 0 );

		// Calculate the updated area of ths display
		renderBounds->set( _dirtyBounds );
		renderBounds->clip( &buffer->rect );

		// TODO: Handle this. For now do nothing
		if ( _displayListDepth == 0 ) {}
		if ( renderBounds->isEmpty() ) {
			_recycleList( _renderList );
			_recycleList( _displayList );
			_dirty = false;
			return;
		}

		/* Debug display list and render bounds
		Serial.println("DisplayList");
		DisplayList* debugNode = _displayList->next();
		while (debugNode) {
			Serial.printf(
				"  ID %d: %d,%d %dx%d (d=%d) (n=%d)\n",
				debugNode->object->id,
				debugNode->object->globalBounds->x,
				debugNode->object->globalBounds->y,
				debugNode->object->globalBounds->width,
				debugNode->object->globalBounds->height,
				debugNode->object->depth,
				debugNode->next()?debugNode->next()->object->id:0
			);
			debugNode = debugNode->next();
		}
		Serial.println("Render bounds");
		Serial.printf("  %d,%d %dx%d\n", renderBounds->x,renderBounds->y, renderBounds->width,renderBounds->height );
		*/

		// Initialise the display to draw only the dirty area
		buffer->setRegion( renderBounds );

		Filter* filter;
		DisplayList* head = _displayList->next();
		DisplayList* next;
		_renderList = DisplayList::Create( this );
		float_t localx;
		float_t localy;
		for ( uint16_t y = renderBounds->y; y <= renderBounds->y2; y++ ) {

			//Serial.printf("\nStart line y=%d\n", y);

			// For this line, grab the active display objects and add to the render list
			// in reverse order (stage at the end)
			while ( head && ( head->object->globalBounds->y <= y ) ) {
				_renderList->insertByDepth( head->object );
				head->object->beginRender( renderBounds );
				filter = head->object->filters;
				while (filter) {
					filter->beginRender( renderBounds );
					filter = filter->next();
				}
				head = head->next();
			}
			/* Debug render list
			Serial.println("RenderList\n");
			DisplayList* list = _renderList;
			while (list) {
				Serial.printf("  ID:%d at y=%d, x=%d (%dx%d) with d=%d\n", list->object->id, int16_t(list->object->y()), int16_t(list->object->x()), int16_t(list->object->width()), int16_t(list->object->height()), list->object->depth );
				list = list->next();
			} */

			// For this line, remove the display objects that are complete, and set the
			// start read position for those that aren't. Skip the first one because it's
			// the stage.
			DisplayList* node = _renderList->next();
			while ( node ) {
				// Todo precalculate BR corner (x2 and y2) 
				if ( y > node->object->globalBounds->y2 ) {
					node->object->endRender();
					filter = node->object->filters;
					while (filter) {
						filter->endRender();
						filter = filter->next();
					}
					next = node->next();
					node->remove()->recycle();
					node = next;
				}
				else {
					localy = node->object->globalToLocalY( y );
					node->object->beginLine( localy );
					filter = node->object->filters;
					while (filter) {
						filter->beginLine( localy );
						filter = filter->next();
					}
					node = node->next();
				}
			}

			// Step pixels in this line
			for ( uint16_t x = renderBounds->x; x <= renderBounds->x2; x++ ) {

				// Debug the render bounds
				if ( debug && ( ( y == renderBounds->y ) || ( y == renderBounds->y2 ) || ( x == renderBounds->x ) || ( x == renderBounds->x2 ) ) ) {
					buffer->pixel( this->debugColor, x );
					continue;
				}

				// Base color
				buffer->pixel( this->_backgroundColor, x );

				// Step through the objects in the display buffer
				node = _renderList->next();
				while ( node ) {
					if ( node->object->globalBounds->containsX( x ) ) {
						localx = node->object->globalToLocalX( x );
						localy = node->object->globalToLocalY( y );
						node->object->calcPixel( localx, localy );
						node->object->_ra *= node->object->alpha;
						if (node->object->_ra > 0) {
							filter = node->object->filters;
							while (filter) {
								filter->filterPixel( localx, localy, node->object->_ra, node->object->_rc );
								filter = filter->next();
							}
							// Draw to buffer
							if ( node->object->_ra == 1.0 ) buffer->pixel( node->object->_rc, x );
							else if ( node->object->_ra > 0 ) buffer->blend( node->object->_rc, alphaClamp( node->object->_ra * node->object->alpha ), x );
						}
						else{
							filter = node->object->filters;
							while (filter) {
								filter->skipPixel( localx, localy );
								filter = filter->next();
							}
						}
					}
					node = node->next();
				}
			}

			// Flip the buffer (auto-advances to next line)
			buffer->flip();
		}
		_recycleList( _renderList );
		_renderList = 0;
		_recycleList( _displayList );
		_displayList = 0;
		_dirty = false;
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
	 * @param buffer The pixel buffer
	 * @param children The children to add
	 * @param px The global x coordinate of the parent
	 * @param py The global y coordinate of the parent
	 */
	void Stage::_traverse( LineBuffer* buffer, DisplayObject* child, boolean forceDirty, float_t px, float_t py ) {
		// Step all children
		while ( child ) {

			// If not visible and not dirty, skip to next child
			if ( ( !child->visible() || ( child->alpha <= 0 ) ) && !child->isDirty() ) {
				child = child->next();
				continue;
			}

			// Force dirty?
			if ( forceDirty ) child->dirty();

			// Get child to calculate global bounds
			child->globalPos( px, py );

			// If child is dirty, include it's old position
			if ( child->isDirty() && !child->cleanBounds->isEmpty() ) {
				_dirtyBounds->grow( child->cleanBounds );
			}

			// Check if on the display
			if ( child->globalBounds->overlaps( &buffer->rect ) ) {

				// Calculate depth
				child->depth = ++_displayListDepth;

				// Add to display list
				_displayList->insertByPosition( child );

				// Add current position to screen dirty area
				if ( child->isDirty() ) {
					_dirtyBounds->grow( child->globalBounds );
					child->cleanBounds->set( child->globalBounds );
				}

			}

			// Recurse
			if ( child->hasChildren() ) _traverse( buffer, child->firstChild(), child->isDirty(), child->globalBounds->x - child->_ox, child->globalBounds->y - child->_oy );

			// Next sibling
			child = child->next();
		}
	}

	/**
	 * Clear a DisplayList
	 * @param list The list to recycle
	 */
	void Stage::_recycleList( DisplayList* list ) {
		DisplayList* next = list;
		while ( list ) {
			next = list->next();
			list->recycle();
			list = next;
		}
	}

} // namespace