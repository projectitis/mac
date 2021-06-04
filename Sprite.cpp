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

	Sprite* Sprite::Create( const TilemapData* tilemapData, uint16_t tileIndex ){
		Sprite* sprite = (Sprite*)DisplayObject::Create<Sprite>();
		sprite->set( tilemapData, tileIndex );
		return sprite;
	}

	/**
	 * Reset the object back to default settings
	 */
	void Sprite::reset(){
		DisplayObject::reset();
	}

	/**
	 * Set the tilemapData and the tileIndex that the sprite uses.
	 * @param tilemapData 	The tilemap data to use
	 * @param tileIndex 	The index of teh active tile
	 */
	void Sprite::set( const TilemapData* tilemapData, uint16_t tileIndex ) {
		this->tilemapData = tilemapData;
		this->tileIndex = tileIndex;
		width( tilemapData->tileWidth );
		height( tilemapData->tileHeight );
		_getPixelAs8888 = getAccessor8888( tilemapData->pixelFormat );
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
	 * Prepare to render the next line
	 * @param ry The y position in local coordinates
	 */
	void Sprite::beginLine( int16_t ry ) {
		switch (transform) {
			case Transform::flipH: {
				_dataOffset = (tileIndex * tilemapData->tileHeight + ry + 1) * tilemapData->tileWidth - renderBounds->x - 1;
				_dataStep = -1;
				break;
			}
			case Transform::flipV: {
				_dataOffset = ((tileIndex + 1) * tilemapData->tileHeight - ry - 1) * tilemapData->tileWidth + renderBounds->x;
				_dataStep = 1;
				break;
			}
			case Transform::flipHV:
			case Transform::rotate180: {
				_dataOffset = ((tileIndex + 1) * tilemapData->tileHeight - ry) * tilemapData->tileWidth - renderBounds->x - 1;
				_dataStep = -1;
				break;
			}
			default: {
				_dataOffset = (tileIndex * tilemapData->tileHeight + ry) * tilemapData->tileWidth + renderBounds->x;
				_dataStep = 1;
			}
		}
	}

	/**
	 * Read a pixel from the sprite and advance position
	 * @param c (out) color
	 * @param a (out) alpha
	 */
	void Sprite::calcPixel( int16_t rx, int16_t ry ) {
		_getPixelAs8888( (uint8_t*)tilemapData->data, _dataOffset, _rc );
		_dataOffset += _dataStep;
		switch (blendMode) {
			case BlendMode::stamp: {
				_ra = (_rc & 255) / 255.0;
				_rc = color;
				break;
			}
			default: {
				_ra = (_rc >> 24) / 255.0; // TODO: pass as uint8_t?
			}
		}
	}
	
} // namespace