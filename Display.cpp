/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Display.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Set the region of the display to draw to. This resets the line buffer to
	 * start of the region. The backbuffer is not affected until the next flip.
	 * @param x 	The x coordinate of the region
	 * @param y 	The y coordinate of the region
	 * @param w		The width of the region
	 * @param h 	The height of the region
	 */
	void Display::set( uint16_t x, uint16_t y, uint16_t w, uint16_t h ) {
		region.setPosAndSize( x, y, w, h );
		region.clip( &rect ); // Ensure region is within display bounds
		reset();
	}

	/**
	 * Set the region of the display to draw to. This resets the line buffer to
	 * start of the region. The backbuffer is not affected until the next flip.
	 * @param rect 	A rect describing the region
	 */
	void Display::set( ClipRect* rect ) {
		region.set( rect );
		region.clip( &this->rect ); // Ensure region is within display bounds
		reset();
	}

	/**
	 * Resets the region without changing it. This resets the line buffer to
	 * start of the region. The backbuffer is not affected until the next flip.
	 */
	void Display::reset() {
		data[frontIndex].y = region.y;
		data[frontIndex].x = region.x;
		data[frontIndex].x2 = region.x2;
	}

	/**
	 * Flip the front and back buffers, and trigger the drawing of the back buffer
	 * to the display. Moves the front buffer to the next line.
	 */
	void Display::flip() {
		backIndex = frontIndex;
		frontIndex ^= 1;
		data[frontIndex].y = data[backIndex].y + 1;
		if ( data[frontIndex].y > region.y2 ) data[frontIndex].y = region.y;
		data[frontIndex].x = data[backIndex].x;
		data[frontIndex].x2 = data[backIndex].x2;
		ready = 1;
	}
	
} // namespace