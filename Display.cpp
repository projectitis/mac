/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Display.h"

/**
 * Define SPI parameters
 **/
#define SPICLOCK	60e6

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

	/**
	 * Turn the backlight on or off. If the backlight pin is set, the backlight
	 * is turned on automatically during init/construction. If you want it off, call
	 * setbacklight(0) after construction.
	 * @param	brightness	0.0 - 1.0 (PWM)
	 **/
	void Display::backlight( float brightness ){
		if (_bklt==255) return;
		brightness = alphaClamp( brightness );
		if (brightness == 0) digitalWrite( _bklt, LOW );
		else if (_bkltPWM) analogWrite( _bklt, (uint8_t)(255.0*brightness) );
		else digitalWrite( _bklt, HIGH );
	}
	
	/**
	 * @brief Wait until there is space on the SPI fifo
	 */
	void Display::waitFifoNotFull(void) {
		uint32_t sr;
		uint32_t tmp __attribute__((unused));
		do {
			sr = KINETISK_SPI0.SR;
			if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
		} while ((sr & (15 << 12)) > (3 << 12));
	}

	/**
	 * @brief Wait until the SPI fifo is empty
	 */
	void Display::waitFifoEmpty(void) {
		uint32_t sr;
		uint32_t tmp __attribute__((unused));
		do {
			sr = KINETISK_SPI0.SR;
			if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
		} while ((sr & 0xF0F0) > 0);             // wait both RX & TX empty
	}
	
} // namespace