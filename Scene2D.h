/**
 * 2D scene/level types for "mac/μac"
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
#ifndef _MAC_SCENE2DH_
#define _MAC_SCENE2DH_ 1

#include "Bitmap.h"

/**
 * This file is part of the mac (or μac) "Microprocessor App Creator" library.
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 *
 * A 2D scene is made up of layers. A simple 2D game might only have a single layer.
 * A layer can be either a vector layer, a tilemap layer, or a sprite layer. Each
 * layer can have optional effects and paralax. This allows very complex scenes with
 * lighting and other cool effects.
 *
 * A tilemap layer is made up of a grid of tiles. The bitmaps/sprites can come from
 * any number of Tilemaps. Each grid square can have 0, 1 or more actual bitmaps
 * layered on top of each otehr with alpha and offset. This allows varied and unique
 * scenes with much fewer graphical assets.
 *
 * A vector layer is made up of vector shapes. Vectors support alpha, so these are
 * handy for creating shadows, lighting etc.
 *
 * A sprite layer is where any PC and NPC characters and other (usually animated) 
 * sprites are rendered.
 *
 * A complex paralax scene may look like this (back to front):
 * 		Vector layer with sky and basic ground				- Static
 * 		Tilemap layer with mountains and background			- Paralax -1
 * 		Tilemap layer with actual scene at character level	- Paralax 0
 * 		Sprite layer for player and NPCs					- Paralax 0
 * 		Vector layer for lights and shadows over sprites	- Paralax 0
 * 		Tilemap layer for foreground						- Paralax +1
 *
 **/
namespace mac{

	/**
	 * Holds details of a 2D scene
	 **/
	typedef struct Scene2DS {
		uint32_t layerCount;
		Scene2DLayer *layers;
	} Scene2D;

	/**
	 * Scene layer types
	 **/
	typedef enum {
		ST_UNKNOWN 			= 0,
		ST_VECTOR 			= 1,
		ST_TILEMAP 			= 2,
		ST_SPRITE 			= 3
	} Scene2DLayerType;

	/**
	 * Scene flags
	 **/
	enum {
		SF_STATIC 			= 1,
		SF_XXXX 			= 2,
		SF_XXXX 			= 4,
		SF_XXXX 			= 8,
		SF_XXXX 			= 16,
		SF_XXXX 			= 32,
		SF_XXXX 			= 64,
		SF_XXXX 			= 128
	};

	/**
	 * Holds details of a 2D scene layer
	 **/
	typedef struct Scene2DLayerS {
		Scene2DLayerType type;
		uint8_t flags;
		float paralaxFactor;
		alpha alpha;
		uint8_t *data;
	} Scene2DLayer;

	/**
	 * Holds details of a 2D tile
	 **/
	typedef struct Scene2DTileS {
		Tilemap* tileMap;	// The tilemap
		uint16_t index;		// Index into the tilemap
		uint16_t x;			// X coord on map (in tiles)
		uint16_t y;			// Y coord on map (in tiles)
		int8_t ox;			// X offset (in pixels)
		int8_t oy;			// Y offset (in pixels)
		alpha alpha;		// Alpha
	} Scene2DTile;

	

} // ns::mac

#endif