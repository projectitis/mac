/**
 * Graphics library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "GraphicsBitmapExtn.h"

/**
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Blit a tile from the tilemap with on/off transparency, and apply
	 * constant alpha to the blit.
	 * @param 	tileMap A tilemap containing the tile data
	 * @param 	index   The index of the tile in the map
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	alpha	The alpha value (0-255)
	 */
	void GraphicsBitmapExtn::blit(
		TileMap tileMap,
		uint32_t index,
		int16_t dx,
		int16_t dy,
		alpha alpha
	){
		// XXX: Support other pixel formats
		// XXX: Support user-defined transparent color? Maybe as property on tileMap?
		if (tileMap.pixelFormat == PF_RGB565){
			if (alpha == 255){
				blitRGB565_T(
					tileMap.data16 + tileMap.tileStride*index,
					tileMap.tileWidth,
					tileMap.tileHeight,
					dx,
					dy,
					RGB565_TRANSPARENT
				);
			}
			else{
				blitRGB565_A(
					tileMap.data16 + tileMap.tileStride*index,
					tileMap.tileWidth,
					tileMap.tileHeight,
					dx,
					dy,
					alpha,
					RGB565_TRANSPARENT
				);
			}
		}
	}

	/**
	 * Blit an RGB565 bitmap from memory with no alpha
	 * @param	source	The start address of the source bitmap in memory
	 * @param	sw		The width, in pixels, of the source bitmap
	 * @param	sh		The height, in pixels, of the source bitmap 
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 **/
	void GraphicsBitmapExtn::blitRGB565(
		const uint16_t* source,
		uint16_t sw,
		uint16_t sh,
		int16_t dx,
		int16_t dy
	){
		// Some early bounds checking
		if ((sw==0) || (sh==0)) return;
		if ((dx>=_framebuffer->w) || (dy>=_framebuffer->h)) return;

		// Get source address (p) and step (s)
		uint16_t* src_p = (uint16_t*)source;
		uint16_t src_s = sw;

		// Calculate real values with clipping
		if (dy<0){
			src_p += (-dy * sw); 
			sh += dy;
			dy = 0;
		}
		if (dx<0){
			src_p -= dx;
			src_s += dx;
			dx = 0;
		}
		_mdy = (dy+sh) - _framebuffer->h;
		if (_mdy > 0){
			sh -= _mdy;
		}
		_mdx = (dx+sw) - _framebuffer->w;
		if (_mdx > 0){
			src_s -= _mdx;
		}

		// Get destination address
		uint16_t* dst_p = _framebuffer->buffer + dy * _framebuffer->w + dx;
		
		// Copy line by line
		while (sh>0){
			memcpy( dst_p, src_p, src_s*2 );
			dst_p += _framebuffer->w;
			src_p += sw;
			sh--;
		}
	}
	
	/**
	 * Blit an RGB565 bitmap from memory with on/off alpha
	 * @param	source	The start address of the source bitmap in memory
	 * @param	sw		The width, in pixels, of the source bitmap
	 * @param	sh		The height, in pixels, of the source bitmap 
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	transparentColor		The color to use as on/off transparency
	 **/
	void GraphicsBitmapExtn::blitRGB565_T(
		const uint16_t* source,
		uint16_t sw,
		uint16_t sh,
		int16_t dx,
		int16_t dy,
		color transparentColor
	){
		// Some early bounds checking
		if ((sw==0) || (sh==0)) return;
		if ((dx>=_framebuffer->w) || (dy>=_framebuffer->h)) return;
		
		// Get source address (p) and step (s)
		uint16_t* src_hp = (uint16_t*)source;
		uint16_t src_s = sw;

		// Calculate real values with clipping
		if (dy<0){
			src_hp += (-dy * sw); 
			sh += dy;
			dy = 0;
		}
		if (dx<0){
			src_hp -= dx;
			src_s += dx;
			dx = 0;
		}
		_mdy = (dy+sh) - _framebuffer->h;
		if (_mdy > 0){
			sh -= _mdy;	
		}
		_mdx = (dx+sw) - _framebuffer->w;
		if (_mdx > 0){
			src_s -= _mdx;		
		}
		uint16_t* src_p = src_hp;

		// Get destination address
		uint16_t* dst_hp = _framebuffer->buffer + dy * _framebuffer->w + dx;
		uint16_t* dst_p = dst_hp;
		uint16_t sx = src_s;
		
		// Copy pixel by pixel!
		while (sh>0){
			while (sx>0){
				if ((*src_p) != transparentColor) *dst_p = *src_p;
				dst_p++;
				src_p++;
				sx--;
			}
			dst_hp += _framebuffer->w;
			dst_p = dst_hp;
			src_hp += sw;
			src_p = src_hp;
			sx = src_s;
			sh--;
		}
	}
	
	/**
	 * Blit an RGB565 bitmap from memory with on/off alpha, and apply constant
	 * transparency to the blit.
	 * @param	source	The start address of the source bitmap in memory
	 * @param	sw		The width, in pixels, of the source bitmap
	 * @param	sh		The height, in pixels, of the source bitmap 
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	alpha	The alpha value (0-255)
	 * @param	transparentColor		The color to use as on/off transparency
	 **/
	void GraphicsBitmapExtn::blitRGB565_A(
		const uint16_t* source,
		uint16_t sw,
		uint16_t sh,
		int16_t dx,
		int16_t dy,
		alpha alpha,
		color transparentColor
	){
		// Some early bounds checking
		if ((sw==0) || (sh==0)) return;
		if ((dx>=_framebuffer->w) || (dy>=_framebuffer->h)) return;

		// Get source address (p) and step (s)
		uint16_t* src_hp = (uint16_t*)source;
		uint16_t src_s = sw;

		// Calculate real values with clipping
		if (dy<0){
			src_hp += (-dy * sw); 
			sh += dy;
			dy = 0;
		}
		if (dx<0){
			src_hp -= dx;
			src_s += dx;
			dx = 0;
		}
		_mdy = (dy+sh) - _framebuffer->h;
		if (_mdy > 0){
			sh -= _mdy;	
		}
		_mdx = (dx+sw) - _framebuffer->w;
		if (_mdx > 0){
			src_s -= _mdx;		
		}
		uint16_t* src_p = src_hp;

		// Get destination address
		uint16_t* dst_hp = _framebuffer->buffer + dy * _framebuffer->w + dx;
		uint16_t* dst_p = dst_hp;
		uint16_t sx = src_s;

		// Copy pixel by pixel!
		while (sh>0){
			while (sx>0){
				if ((*src_p) != transparentColor) *dst_p = alphaBlendRGB565( *src_p, *dst_p, alpha );
				dst_p++;
				src_p++;
				sx--;
			}
			dst_hp += _framebuffer->w;
			dst_p = dst_hp;
			src_hp += sw;
			src_p = src_hp;
			sx = src_s;
			sh--;
		}
	}
	
}