/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Sprite.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	DisplayObject* Sprite::pool = 0;

	/**
	 * Constructor
	 */
	//Sprite::Sprite() : DisplayObject(){}

	/**
	 * Pool getter
	 */
	DisplayObject** Sprite::_getPool(){
		return &Sprite::pool;
	}

	/**
	 * Create a new object or take one from the pool
	 * @return The new or recycled object
	 */
	Sprite* Sprite::Create(){
		return (Sprite*)DisplayObject::Create<Sprite>();
	}

	Sprite* Sprite::Create( Tilemap* tilemap, uint16_t tileIndex ){
		Sprite* sprite = (Sprite*)DisplayObject::Create<Sprite>();
		sprite->tilemap = tilemap;
		sprite->tileIndex = tileIndex;
		return sprite;
	}

	/**
	 * Reset the object back to default settings
	 */
	void Sprite::reset(){
		DisplayObject::reset();
	}

	/**
	 * Update and render the display object
	 */
	void Sprite::update( float dt ){
		// Update self
		
		// Update children
		DisplayObject::update( dt );
	}

	void Sprite::rect( ClipRect* inRect ) {
		inRect->setPosAndSize( (int16_t)x, (int16_t)y, tilemap->tileWidth, tilemap->tileHeight );
	}
	
} // namespace