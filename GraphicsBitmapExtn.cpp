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
		uint16_t dx,
		uint16_t dy
	){
		if ((sw==0) || (sh==0)) return;
		
		// Get destination buffer rect
		uint16_t* dst_p = _framebuffer->buffer + dy * _framebuffer->w + dx;
		uint16_t* src_p = (uint16_t*)source;
		
		// Copy line by line
		while (sh>0){
			memcpy( dst_p, src_p, sw*2 );
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
		uint16_t dx,
		uint16_t dy,
		color transparentColor
	){
		if ((sw==0) || (sh==0)) return;
		
		// Get destination buffer rect
		uint16_t* dst_hp = _framebuffer->buffer + dy * _framebuffer->w + dx;
		uint16_t* dst_p = dst_hp;
		uint16_t* src_p = (uint16_t*)source;
		uint16_t sx = sw;
		
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
			sx = sw;
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
		uint16_t dx,
		uint16_t dy,
		alpha alpha,
		color transparentColor
	){
		if ((sw==0) || (sh==0)) return;
		
		// Get destination buffer rect
		uint16_t* dst_hp = _framebuffer->buffer + dy * _framebuffer->w + dx;
		uint16_t* dst_p = dst_hp;
		uint16_t* src_p = (uint16_t*)source;
		uint16_t sx = sw;
		
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
			sx = sw;
			sh--;
		}
	}
	void GraphicsBitmapExtn::blitRGB565_A_approx(
		const uint16_t* source,
		uint16_t sw,
		uint16_t sh,
		uint16_t dx,
		uint16_t dy,
		alpha alpha,
		color transparentColor
	){
		if ((sw==0) || (sh==0)) return;
		
		// Get destination buffer rect
		uint16_t* dst_hp = _framebuffer->buffer + dy * _framebuffer->w + dx;
		uint16_t* dst_p = dst_hp;
		uint16_t* src_p = (uint16_t*)source;
		uint16_t sx = sw;
		
		// Copy pixel by pixel!
		while (sh>0){
			while (sx>0){
				if ((*src_p) != transparentColor) *dst_p = alphaBlendRGB565_approx( *src_p, *dst_p, alpha );
				dst_p++;
				src_p++;
				sx--;
			}
			dst_hp += _framebuffer->w;
			dst_p = dst_hp;
			sx = sw;
			sh--;
		}
	}
	
}