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
		sprite->size( tilemap->tileWidth, tilemap->tileHeight );
		sprite->_getPixelAs8888 = getAccessor8888( tilemap->pixelFormat );
		return sprite;
	}

	/**
	 * Reset the object back to default settings
	 */
	void Sprite::reset(){
		DisplayObject::reset();
	}

	/**
	 * Set the tilemap and the tileIndex that the sprite uses.
	 * @param tilemap 	The tilemap to use
	 * @param tileIndex The index of teh active tile
	 */
	void Sprite::set( Tilemap* tilemap, uint16_t tileIndex ) {
		tilemap = tilemap;
		tileIndex = tileIndex;
		size( tilemap->tileWidth, tilemap->tileHeight );
		_getPixelAs8888 = getAccessor8888( tilemap->pixelFormat );
		dirty();
	}

	/**
	 * Update the display object
	 */
	void Sprite::update( float dt ){
		// Update self
		
		// Update children
		DisplayObject::update( dt );
	}

	/**
	 * Set the position at which to read the next pixel
	 * @param x The global x coordinate
	 * @param y The global y coordinate
	 */
	void Sprite::readPosition( int16_t gx, int16_t gy ){
Serial.printf("Sprite::readPosition %d %d", gx, gy);
		DisplayObject::readPosition( gx, gy );
		_dataOffset = (tileIndex * tilemap->tileHeight + ry) * tilemap->tileWidth + rx;
Serial.printf(" with offset %d\n", _dataOffset);
	}

	/**
	 * Read a pixel from the sprite and advance position
	 * @param c (out) color
	 * @param a (out) alpha
	 */
	void Sprite::readPixel( color888 &c, float &a ) {
		_getPixelAs8888( (uint8_t*)tilemap->data, _dataOffset, c );
		_dataOffset++;
		switch (blendMode) {
			case BlendMode::stamp: {
				a = (c & 255) / 255.0;
				c = color;
				break;
			}
			default: {
				a = (c >> 24) / 255.0; // TODO: pass as uint8_t?
			}
		}
	}
	
} // namespace