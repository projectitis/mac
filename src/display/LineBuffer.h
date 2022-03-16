#pragma once
#ifndef _MAC_CONTEXTH_
#define _MAC_CONTEXTH_ 1

#include "Common.h"
#include "display/Display.h"
#include "geom/ClipRect.h"
#include "graphics/Bitmap.h"

namespace mac {

	/**
	 * Holds the pixel data and state of the line buffer. There
	 * are two of these - one for the front and one for the back.
	 */
	typedef struct {

		// The pixel data as 32bit words. There is a storage overhead of 1 byte
		// per pixel, but this is made up for by the faster math this allows.
		uint32_t* pixels;

		// The y coord of the linebuffer
		uint16_t y;

		// The start x coord (normally 0)
		uint16_t x;

		// The end x coord (normally width-1)
		uint16_t x2;

	} LineBufferData;

	/**
	 * Line Buffer
	 * A line-by-line frame buffer implementation
	 **/
	class LineBuffer {
	public:
		/**
		 * @brief Construct a new Line Buffer object
		 *
		 * @param display
		 */
		LineBuffer( Display* display );

		/**
		 * @brief Destroy the Line Buffer object
		 */
		~LineBuffer();

		/**
		 * @brief Set the area of the buffer to draw to
		 * Sets the buffer region
		 * @param rect The region of the display to draw to
		 */
		void setRegion( ClipRect* rect );

		/**
		 * Resets the region without changing it. This resets the line buffer to
		 * start of the region. The backbuffer is not affected until the next flip.
		 */
		void resetRegion();

		/**
		 * Flip the front and back buffers, and trigger the drawing of the back buffer
		 * to the display. Moves the front buffer to the next line.
		 */
		void flip();

		/**
		 * Fill the current line with a color. Respects region
		 **/
		inline void clear( color888 c ) {
			uint16_t i = _region.x;
			while ( i < _region.x2 ) _data[_frontIndex].pixels[i++] = c;
		}

		/**
		 * Write a pixel to the current line. Overwrites underlying pixel (if any)
		 * @param  c 	The color (pixel) in RGB 888 format
		 * @param  x 	The X coordinate
		 */
		inline void pixel( color888 c, int16_t x ) {
			_data[_frontIndex].pixels[x] = c;
		}

		/**
		 * read a pixel from the current line (front buffer)
		 * @param  x 	The X coordinate
		 * @return 		The color (pixel) in RGB 888 format
		 */
		inline color888 pixel( int16_t x ) {
			return _data[_frontIndex].pixels[x];
		}

		/**
		 * Blend a pixel to the current line using float_t alpha 0.0 - 1.0
		 * @param  c 	The color (pixel) in RGB 888 format
		 * @param  a 	The alpha (0.0 - 1.0)
		 * @param  x 	The X coordinate
		 */
		inline void blend( color888 c, alpha_t a, int16_t x ) {
			_data[_frontIndex].pixels[x] = blend888( _data[_frontIndex].pixels[x], c, alpha8bit( a ) );
		}

		/**
		 * Blend a pixel to the current line using integer alpha 0 - 255
		 * @param  c 	The color (pixel) in RGB 888 format
		 * @param  a 	The alpha (0 - 255)
		 * @param  x 	The X coordinate
		 */
		inline void blend( color888 c, uint8_t a, int16_t x ) {
			_data[_frontIndex].pixels[x] = blend888( _data[_frontIndex].pixels[x], c, a );
		}

		/**
		 * @brief Maximum buffer size as dictated by display
		 *
		 */
		ClipRect rect;

	protected:

		/**
		 * Pointer to the pixel data
		 */
		LineBufferData _data[2];

		/**
		 * Index to the active (front) data buffer for reading and writing
		 */
		uint8_t _frontIndex = 0;

		/**
		 * Index to the active data buffer for transfering to the hardware. I.e: `lineBuff->data[ lineBuff->backIndex ][ x ]`
		 */
		uint8_t _backIndex = 1;

		/**
		 * Reference to the display
		 */
		Display* _display;

		/**
		 * Rect describing the drawing region (scaled to pixelscale)
		 */
		ClipRect _region;
	};

} // namespace

#endif
