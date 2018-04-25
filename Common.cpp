/**
 * Common types, functions and helpers for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Common.h"

namespace mac{
	
	/**
	 * Found in a pull request for the Adafruit framebuffer library. Clever!
	 * https://github.com/tricorderproject/arducordermini/pull/1/files#diff-d22a481ade4dbb4e41acc4d7c77f683d
	 **/
	color alphaBlendRGB565(
		uint32_t fg,
		uint32_t bg,
		alpha alpha
	){
		// Alpha converted from [0..255] to [0..31]
		alpha = ( alpha + 4 ) >> 3;
		
		// Converts  0000000000000000rrrrrggggggbbbbb
		//     into  00000gggggg00000rrrrr000000bbbbb
		// with mask 00000111111000001111100000011111
		// This is useful because it makes space for a parallel fixed-point multiply
		bg = (bg | (bg << 16)) & 0b00000111111000001111100000011111;
		fg = (fg | (fg << 16)) & 0b00000111111000001111100000011111;
		
		// This implements the linear interpolation formula: result = bg * (1.0 - alpha) + fg * alpha
		// This can be factorized into: result = bg + (fg - bg) * alpha
		// alpha is in Q1.5 format, so 0.0 is represented by 0, and 1.0 is represented by 32
		//uint32_t result = (fg - bg) * alpha; // parallel fixed-point multiply of all components
		//result >>= 5;
		//result += bg;
		//result &= 0b00000111111000001111100000011111; // mask out fractional parts
		//return (color)((result >> 16) | result);
		
		uint32_t result = ((((fg - bg) * alpha) >> 5) + bg) & 0b00000111111000001111100000011111;
		return (color)((result >> 16) | result); // contract result
	}
	
	/**
	 * Convert R G B color triplet to RGB565 16bit format
	 * @param	r		Red component
	 * @param	g		Green component
	 * @param	b		Blue component
	 **/
	color convertRGBtoRGB565( uint8_t r, uint8_t g, uint8_t b ) {
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	}
	
	/**
	 * Converts RGB565 format to RGB color triplet
	 * @param	color	The RGB565 color to convert
	 * @param	r		(out) Red component
	 * @param	g		(out) Green component
	 * @param	b		(out) Blue component
	 **/
	void convertRGB565toRGB(color color, uint8_t &r, uint8_t &g, uint8_t &b) {
		r = (color>>8) & 0x00F8;
		g = (color>>3) & 0x00FC;
		b = (color<<3) & 0x00F8;
	}
	
}