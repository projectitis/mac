/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Shape.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	DisplayObject* Shape::pool = 0;

	/**
	 * Pool getter
	 */
	DisplayObject** Shape::_getPool(){
		return &Shape::pool;
	}

	/**
	 * Create a new object or take one from the pool
	 * @return The new or recycled object
	 */
	Shape* Shape::Create(){
		return (Shape*)DisplayObject::Create<Shape>();
	}

	Shape* Shape::Create( Point** points, uint16_t len ) {
		Shape* shape = (Shape*)DisplayObject::Create<Shape>();
		shape->set( points, len );
		return shape;
	}

	/**
	 * Reset the object back to default settings
	 */
	void Shape::reset(){
		_points = 0;
		_pointsLen = 0;

		DisplayObject::reset();
	}

	/**
	 * Set the tilemap and the tileIndex that the sprite uses.
	 * @param tilemap 	The tilemap to use
	 * @param tileIndex The index of teh active tile
	 */
	void Shape::set( Point** points, uint16_t len ) {
		
		dirty();
	}

	/**
	 * Update the display object
	 */
	void Shape::update( float dt ){
		// Update self
		
		// Update children
		DisplayObject::update( dt );
	}

	/**
	 * @brief Set the global position of the display object
	 * 
	 * @param x The global X position
	 * @param y The global Y position
	 */
	void Shape::globalPos( float x, float y ) {
		_localBounds->clear();
		uint16_t l = _pointsLen;
		Point* p;
		while (l--) {
			p = _points[ l ];
			_localBounds->x = min( _localBounds->x, p->x );
			_localBounds->x2 = max( _localBounds->x2, p->x );
			_localBounds->y = min( _localBounds->y, p->y );
			_localBounds->y2 = max( _localBounds->y2, p->y );
		}
		DisplayObject::globalPos( x, y );
	}

	/**
	 * Prepare to render the next line
	 * @param ry The y position in local coordinates
	 */
	void Shape::beginLine( int16_t ry ) {
		
	}

	/**
	 * Read a pixel from the sprite and advance position
	 * @param c (out) color
	 * @param a (out) alpha
	 */
	void Shape::calcPixel( int16_t rx, int16_t ry ) {
		_ra = 1;
		_rc = color;
	}
	
} // namespace