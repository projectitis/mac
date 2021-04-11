/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "GUIIcon.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	Widget* GUIIcon::pool = 0;

	/**
	 * Constructor
	 */
	GUIIcon::GUIIcon( Style* aStyle ) : Icon( aStyle ){}

	/**
	 * Pool getter
	 */
	Widget** GUIIcon::_getPool(){
		return &GUIIcon::pool;
	}

	/**
	 * Create a new widget or take one from the pool
	 * @return The new or recycled widget
	 */
	GUIIcon* GUIIcon::Create( Style* aStyle ){
		return (GUIIcon*)Widget::Create<GUIIcon>(aStyle);
	}

	/**
	 * Render the display object. Usually you would call update which internally calls render.
	 * @param  	graphics 	The graphics object to use for rendering
	 * @param  	force 		If true, will render even if not dirty (force redraw)
	 * @return          Return true if current widget rendered (_dirty or forced)
	 */
	boolean GUIIcon::render( Graphics* graphics, boolean force ){
		// Render children
		boolean childRendered = Widget::render( graphics, force );
		// Render self
		if (_dirty || childRendered || force){

			// if there is no tilemap, use the default
			if (!_iconTilemap) _iconTilemap = style->icons;

			// Skip to the tile
			uint32_t dataIndex = 0;
			uint8_t i = 0;
			while (i++ < _iconIndex) dataIndex += guiIconPartData[dataIndex]*4 + 1;

			// Draw the parts!
			uint8_t partCount = guiIconPartData[dataIndex++];
			uint8_t partIndex, r_x, r_y, r_w, r_h, ox, oy, r;
			uint16_t detailDataIndex;
			ClipRect* rect = new ClipRect();
			while (partCount) {
				partIndex = guiIconPartData[dataIndex++];
//Serial.printf("Part %d\n", partIndex);
				ox = guiIconPartData[dataIndex++];
				oy = guiIconPartData[dataIndex++];
				r = guiIconPartData[dataIndex++];
				// This is a detail (a portion of a tile)
				if (partIndex >= 200) {	
					// Get detail info
					detailDataIndex = (partIndex - 200) * 5;
					partIndex = guiIconDetailData[detailDataIndex++];
					r_x = guiIconDetailData[detailDataIndex++];
					r_y = guiIconDetailData[detailDataIndex++];
					r_w = guiIconDetailData[detailDataIndex++];
					r_h = guiIconDetailData[detailDataIndex];
					rect->setPosAndSize( r_x, r_y, r_w, r_h );
//Serial.printf("  Detail %d,%d,%d,%d\n", rect->x, rect->y, rect->width, rect->height );
					graphics->bitmap->stamp( _iconColor, _iconTilemap, (uint32_t)partIndex, rect, (uint16_t)(x + ox), (uint16_t)(y + oy), _iconAlpha, (BitmapRotation)r );
				}
				// This is a full tile
				else {
//Serial.printf("  Tile\n");
					graphics->bitmap->stamp( _iconColor, _iconTilemap, (uint32_t)partIndex, (uint16_t)(x + ox), (uint16_t)(y + oy), _iconAlpha, (BitmapRotation)r );
				}
				partCount--;
			}
			delete rect;

			_dirty = false;
			return true;
		}
		else{
			return false;
		}
	}
	
} // namespace