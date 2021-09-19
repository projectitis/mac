/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 *
 * MIT LICENCE
 * -----------
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */	

#pragma once
#ifndef _MAC_SPRITEH_
#define _MAC_SPRITEH_ 1

#include "display/DisplayObject.h"
#include "graphics/Bitmap.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Blend modes supported by Sprite
	 */
	enum class BlendMode{
		normal,
		stamp
	};

	/**
	 * Transformations supported by Sprite
	 */
	enum class Transform{
		normal,
		flipH,
		flipV,
		flipHV,
		rotate180
	};

	/**
	 * A sprite
	 */
	class Sprite: public DisplayObject {
		
		public:
			/**
			 * Constructor
			 */
			//Sprite();

			/**
			 * Memory pool of recycled objects
			 */
			static DisplayObject* pool;

			/**
			 * Create a new object or take one from the pool
			 * @return The new or recycled object
			 */
			static Sprite* Create();
			static Sprite* Create( const TilemapData* tilemapData, uint16_t tileIndex = 0 );

			/**
			 * Type identifier for this object
			 **/
			static const DisplayObjectType TYPE = DisplayObjectType::sprite;

			/**
			 * Reset the object back to default settings
			 */
			void reset() override;

			/**
			 * Update the display object.
			 * @param	dt 			Time since last update in seconds
			 */
			void update( float_t dt ) override;

			/**
			 * The tilemap that contains the sprite bitmap data.
			 * Do not change directly. Use set() to change this.
			 */
			const TilemapData* tilemapData;

			/**
			 * The index of the tile within the tilemap.
			 */
			uint16_t tileIndex; 

			/**
			 * transform for sprite
			 */
			Transform transform = Transform::normal;

			/**
			 * Blend mode for sprite
			 */
			BlendMode blendMode = BlendMode::normal;

			/**
			 * Color (used for some blend modes)
			 */
			color888 color;

			/**
			 * Set the tilemap and the tileIndex that teh sprite uses.
			 * @param tilemap 	The tilemap to use
			 * @param tileIndex The index of teh active tile
			 */
			virtual void set( const TilemapData* tilemapData, uint16_t tileIndex = 0 );

			/**
			 * prepare to render the next line
			 * @param ry The y position in local coordinates
			 */
			virtual void beginLine( int16_t ry );

			/**
			 * Read a pixel from the sprite and advance position
			 * @param rx The x position in local coordinates
			 * @param ry The y position in local coordinates
			 */
			virtual void calcPixel( int16_t rx, int16_t ry );

		protected:
			
			/**
			 * Pool getter
			 */
			DisplayObject** _getPool() override;

			/**
			 * Pixel accessor for correct tilemap pixel format
			 */
			access8888 _getPixelAs8888;

			/**
			 * Current offset into the pixel data
			 */
			int32_t _dataOffset;

			/**
			 * Current setp with each drawn pixel
			 */
			int32_t _dataStep;

	};
	
} // namespace

#endif