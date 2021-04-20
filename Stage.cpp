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
	 * Constructor
	 */
	Stage::Stage() : DisplayObject(){
		_dirtyRect = new ClipRect();
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

		// Build display list. At the same time, determine the area of the display that is dirty
		// The display list is a list of all visible display objects (both with 'visibility' flag
		// set to true, and at least partially visible on the display) ordered from top to bottom
		// and left to right.
		_displayListNext = 0;
		_displayListPrev = 0;
		_depth = 0;
		if (_children) {
			_sortDisplayList( this, &display->rect );
		}

		// Debug display list
		Serial.println("DisplayList");
		DisplayObject* node = _displayListNext;
		while (node) {
			Serial.printf("  ID:%d at y=%d, x=%d with d=%d\n", node->id, int16_t(node->y), int16_t(node->x), int32_t(((Stage*)node)->_depth) );
			node = ((Stage*)node)->_displayListNext;
		}

		// Draw TL to BR
		// 
		// For each pixel there will be a bg color and 0 or more pixels to render above that. Ideally
		// we would draw from the top down to the first fully opaque color. The bg is fully opaque, so
		// it is the backstop.
		// 
		// At this point we have a linear list of all display items top to bottom and left to right. This
		// means that we can grab the display objects that have pixels to draw on any particular scanline
		// as we move down the display. For example, at pixel (x,y) there might be 4 display objects layered
		// on top of each other. We need to determine the draw order. This is determined by the _depth
		// property of DisplayObject, with highest being on top, and lowest (the stage with depth = 0) on
		// the bottom.
		// 
		// We need to order all active objects by _depth. Then for each display pixel, start from the first
		// layer that has an opaque pixel and composite pixels upwards.
		//

		// TODO: Build an ordered list object
		uint16_t y = 0;
		uint16_t x = 0;
		for ( y = _dirtyRect->y; y <= _dirtyRect->y2; y++ ) {
			// For this line, grab the active display objects and add to the ordered list
			foreach() list->add(); // Add new objects where y is active 
			list->update( y ); // Removes any objects where y is inactive (is now past the bottom)

			// Flip the buffer
			display->linebuffer->flip( y, _dirtyRect->x, _dirtyRect->x2 );

			for ( x = _dirtyRect->x; x <= _dirtyRect->x2; x++ ) {

				// TODO: pixel format specific function

				// For this pixel, find the first opaque (alpha==1) layer and raw that pixel
				list->seek();
				display->linebuffer->pixel( color, x );

				// Now step through active display objects and composite pixel to line buffer
				while (list->step()) {
					display->linebuffer->blend( list->colorAt( x, y ), x, alpha );
				}

			}

			// Flip the buffer
			display->linebuffer->flip( y );

		}




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
	
} // namespace