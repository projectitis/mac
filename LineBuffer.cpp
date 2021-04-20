/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "LineBuffer.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Constructor
	 */
	LineBuffer::LineBuffer( PixelFormat pixelFormat, uint16_t w ){
		width = w;
		switch (pixelFormat){
			case PF_INDEXED: {
				data[0].pixels.data8 = new uint8_t[width];
				data[1].pixels.data8 = new uint8_t[width];
				break; // XXX: Implement indexed color
			}
			case PF_565: {
				data[0].pixels.data16 = new uint16_t[width];
				data[1].pixels.data16 = new uint16_t[width];
				break;
			}
			case PF_888:{
				data[0].pixels.data32 = new uint32_t[width];
				data[1].pixels.data32 = new uint32_t[width];
				break;
			}
			case PF_MONO: {
				data[0].pixels.data8 = new uint8_t[(uint32_t)((width+7)/8)];
				data[1].pixels.data8 = new uint8_t[(uint32_t)((width+7)/8)];
				break;
			}
			default: // XXX: Handle this error
				break;
		}
		this->pixelFormat = pixelFormat;
		data[0].y = 0;
		data[0].x0 = 0;
		data[0].x1 = width - 1;
		data[1].y = 0;
		data[1].x0 = 0;
		data[1].x1 = width - 1;
	}

	/**
	 * Destructor
	 */
	LineBuffer::~LineBuffer(){
		switch (pixelFormat){
			case PF_INDEXED:{
				delete[] data[0].pixels.data8;
				delete[] data[1].pixels.data8;
				break;
			}
			case PF_565: {
				delete[] data[0].pixels.data16;
				delete[] data[1].pixels.data16;
				break;
			}
			case PF_888: {
				delete[] data[0].pixels.data32;
				delete[] data[1].pixels.data32;
				break;
			}
			case PF_MONO: {
				delete[] data[0].pixels.data8;
				delete[] data[1].pixels.data8;
				break;
			}
			default: 
				break;
		}
	}

	/**
	 * Fill the framebuffer with a color
	 **/
	void LineBuffer::clear( color888 c ){
		switch (pixelFormat){
			case PF_565: {
				clear16( convert888to565( c ) );
				break;
			}
			case PF_888: {
				clear32( c );
				break;
			}
			case PF_INDEXED: {
				// XXX: TODO - paletted color
				clear8( c & 255 );
				break;
			}
			case PF_MONO: {
				clearMono( c & 0b1 );
				break;
			}
			default:
				break;
		}
	}

	/**
	 * Write a pixel to the framebuffer. Checks the pixel format and uses the
	 * correct method. This method is slow. If you are setting a lot of pixels, use
	 * the pixel-format specific method, or hit the framebuffer directly.
	 * @param  c 	The color (pixel) in RGB 888 format
	 * @param  x 	The X coordinate
	 */
	void LineBuffer::pixel( color888 c, int16_t x ){
		if (x < 0) return;
		if (x >= width ) return;
		switch (pixelFormat){
			case PF_565: {
				writePixel( convert888to565( c ), x );
				break;
			}
			case PF_888: {
				writePixel( c, x );
				break;
			}
			case PF_INDEXED: {
				// XXX: Pick color index from table that is closest to specified
				break;
			}
			case PF_MONO: {
				writePixelMono( c, x );
				break;
			}
			default:
				break;
		}
	}

	/**
	 * Blend a pixel to the framebuffer. Checks the pixel format and uses the
	 * correct method. This method is slow. If you are blending a lot of pixels, use
	 * the pixel-format specific method, or hit the framebuffer directly.
	 * @param  c 	The color (pixel) in RGB 888 format
	 * @param  a 	The alpha (0.0 - 1.0)
	 * @param  x 	The X coordinate
	 */
	void LineBuffer::blend( color888 c, alpha a, int16_t x ){
		if (x < 0) return;
		if (x >= width ) return;
		switch (pixelFormat){
			case PF_565: {
				blendPixel( convert888to565( c ), alpha5bit(a), x );
				break;
			}
			case PF_888: {
				blendPixel( c, alpha8bit(a), x );
				break;
			}
			case PF_INDEXED: {
				// XXX: Pick color index from table that is closest to specified
				break;
			}
			case PF_MONO: {
				blendPixelMono( c, alpha8bit(a), x );
				break;
			}
			default:
				break;
		}
	}

	/**
	 * read a pixel from the framebuffer in RGB 888 format. Checks the pixel format
	 * and uses the correct method. This method is slow. If you are reading a lot of
	 * pixels, use the pixel-format specific method, or hit the framebuffer directly.
	 * @param  x 	The X coordinate
	 * @return 		The color (pixel) in RGB 888 format
	 */
	color888 LineBuffer::pixel( int16_t x ){
		if (x < 0) return 0;
		if (x >= width ) return 0;
		switch (pixelFormat){
			case PF_565: {
				color565 c;
				readPixel( x, c );
				return convert565to888( c );
			}
			case PF_888: {
				color888 c;
				readPixel( x, c );
				return c;
			}
			case PF_INDEXED: {
				// XXX: Pick color from table by index
				return 0;
			}
			case PF_MONO: {
				uint8_t c;
				readPixelMono( x, c );
				return (c==0)?RGB888_Black:RGB888_White;
			}
			default:
				return 0;
				break;
		}
	}
	
} // namespace