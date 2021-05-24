/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Box.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	DisplayObject* Box::pool = 0;

	/**
	 * Pool getter
	 */
	DisplayObject** Box::_getPool(){
		return &Box::pool;
	}

	/**
	 * Create a new object or take one from the pool
	 * @return The new or recycled object
	 */
	Box* Box::Create(){
		return (Box*)DisplayObject::Create<Box>();
	}

	Box* Box::Create( int16_t x, int16_t y, int16_t w, int16_t h ) {
		Box* box = (Box*)DisplayObject::Create<Box>();
		box->set( x, y, w, h );
		return box;
	}

	Box* Box::Create( ClipRect* rect ) {
		Box* box = (Box*)DisplayObject::Create<Box>();
		box->set( rect->x, rect->y, rect->width, rect->height );
		return box;
	}

	/**
	 * Reset the object back to default settings
	 */
	void Box::reset(){
		DisplayObject::reset();
	}

	/**
	 * Set the tilemap and the tileIndex that the sprite uses.
	 * @param tilemap 	The tilemap to use
	 * @param tileIndex The index of teh active tile
	 */
	void Box::set( int16_t x, int16_t y, int16_t w, int16_t h ) {
		this->x( x );
		this->y( y );
		this->width( w );
		this->height( h );
		dirty();
	}

	/**
	 * Read a pixel from the sprite and advance position
	 * @param c (out) color
	 * @param a (out) alpha
	 */
	void Box::calcPixel( int16_t rx, int16_t ry ) {
		_ra = 1;
		_rc = color;
	}
	
} // namespace