/**
 * Display library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "DisplayILI9341.h"
#include <SPI.h>
#include <DMAChannel.h>

/**
 * Define ILI9341 commands
 **/
#define ILI9341_NOP			0x00
#define ILI9341_SWRESET		0x01		// Resets the commands and parameters to their S/W Reset default values
#define ILI9341_RDDID		0x04		// Read display identification information (return 24bits)
#define ILI9341_RDDST		0x09		// Read display status 

#define ILI9341_SLPIN		0x10		// Enter sleep mode
#define ILI9341_SLPOUT		0x11		// Turns off sleep mode
#define ILI9341_PTLON		0x12		// Turns on partial mode
#define ILI9341_NORON		0x13		// Returns the display to normal mode (aprtial mode off)

#define ILI9341_RDMODE		0x0A		
#define ILI9341_RDMADCTL	0x0B
#define ILI9341_RDPIXFMT	0x0C
#define ILI9341_RDIMGFMT	0x0D
#define ILI9341_RDSELFDIAG	0x0F

#define ILI9341_INVOFF		0x20		// Recover from display inversion mode
#define ILI9341_INVON		0x21		// Enter into display inversion mode
#define ILI9341_GAMMASET	0x26		// Select the desired Gamma curve (only 01 is defined)
#define ILI9341_DISPOFF		0x28		// Display off
#define ILI9341_DISPON		0x29		// Display on

#define ILI9341_CASET		0x2A		// Column Address Set
#define ILI9341_PASET		0x2B		// Page Address Set
#define ILI9341_RAMWR		0x2C		// Memory Write
#define ILI9341_RGBSET		0x2D		// Color Set
#define ILI9341_RAMRD		0x2E		// Memory Read

#define ILI9341_PTLAR		0x30
#define ILI9341_MADCTL		0x36
#define ILI9341_VSCRSADD	0x37
#define ILI9341_PIXFMT		0x3A

#define ILI9341_FRMCTR1		0xB1
#define ILI9341_FRMCTR2		0xB2
#define ILI9341_FRMCTR3		0xB3
#define ILI9341_INVCTR		0xB4
#define ILI9341_DFUNCTR		0xB6

#define ILI9341_PWCTR1		0xC0		// Power control
#define ILI9341_PWCTR2		0xC1
#define ILI9341_PWCTR3		0xC2
#define ILI9341_PWCTR4		0xC3
#define ILI9341_PWCTR5		0xC4
#define ILI9341_VMCTR1		0xC5		// Set the VCOM voltage
#define ILI9341_VMCTR2		0xC7

#define ILI9341_RDID1		0xDA
#define ILI9341_RDID2		0xDB
#define ILI9341_RDID3 		0xDC
#define ILI9341_RDID4		0xDD

#define ILI9341_GMCTRP1		0xE0		// Positive Gamma correction
#define ILI9341_GMCTRN1		0xE1		// Negative Gamma correction

#define ILI9341_IFCTL		0xF6		// Interface Control

/**
 * Define SPI parameters
 **/
#define MADCTL_MY	0x80
#define MADCTL_MX	0x40
#define MADCTL_MV	0x20
#define MADCTL_ML	0x10
#define MADCTL_RGB	0x00
#define MADCTL_BGR	0x08
#define MADCTL_MH	0x04

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
	 * Constructor. Note - constructor calls init
	 * @param	cs		Pin used for Chip Select
	 * @param	dc		Pin used for Data Control
	 * @param	rst		Pin used for Reset (optional. 255=unused)
	 * @param	mosi	Pin used for MOSI communication (data out from master)
	 * @param	sclk	Pin used for clock
	 * @param	miso	Pin used for MISO communication (data out from slave)
	 * @param	width	TFT pixel width
	 * @param	Height	TFT pixel height
	 **/
	DisplayILI9341::DisplayILI9341(
		uint8_t cs,
		uint8_t dc,
		uint8_t rst,
		uint8_t mosi,
		uint8_t sclk,
		uint8_t miso,
		uint8_t bklt,
		PixelScale px
	){
		_cs   = cs;
		_dc   = dc;
		_rst  = rst;
		_mosi = mosi;
		_sclk = sclk;
		_miso = miso;
		_bklt = bklt;
		width    = 320;
		height   = 240;
		pixelFormat = PF_565;
		_px = px;
		_ipx = 1.0/(float)_px;
		rect.setSize( (uint16_t)(width * _ipx), (uint16_t)(height * _ipx) );
		
		init();
	}
	
	/**
	 * Initialise the display. Called from the constructor.
	 * More info here: https://gist.github.com/postmodern/ed6e670999f456ad9f13
	 **/
	static const uint8_t init_commands[] = {
		4, 		0xEF, 0x03, 0x80, 0x02,
		4, 		0xCF, 0x00, 0XC1, 0X30,				// Power control B
		5, 		0xED, 0x64, 0x03, 0X12, 0X81,		// Power on sequence control
		4, 		0xE8, 0x85, 0x00, 0x78,				// Driver timing control A
		6, 		0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02, // Power control A
		2, 		0xF7, 0x20,							// Pump ratio control
		3, 		0xEA, 0x00, 0x00,					// Driver timing control B
		2, 		ILI9341_PWCTR1, 0x23, 				// Power control
		2, 		ILI9341_PWCTR2, 0x10, 				// Power control
		3, 		ILI9341_VMCTR1, 0x3e, 0x28, 		// VCM control
		2, 		ILI9341_VMCTR2, 0x86, 				// VCM control2
		2, 		ILI9341_MADCTL, 0x48, 				// Memory Access Control
		2, 		ILI9341_PIXFMT, 0x55,				// 16bit 565 pixel format
		3, 		ILI9341_FRMCTR1, 0x00, 0x18,
		4, 		ILI9341_DFUNCTR, 0x08, 0x82, 0x27,	// Display Function Control
		2, 		0xF2, 0x00, 						// Gamma Function Disable
		2, 		ILI9341_GAMMASET, 0x01, 			// Gamma curve selected
		16, 	ILI9341_GMCTRP1, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
				0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // Set Gamma
		16, 	ILI9341_GMCTRN1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
				0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // Set Gamma
		3, 		0xb1, 0x00, 0x10, 					// FrameRate Control 119Hz
		
		2, 		ILI9341_MADCTL, MADCTL_MV | MADCTL_BGR,
		0
	};
	void DisplayILI9341::init( void ){
		
		// Set up SPI pins
		SPI.setMOSI(_mosi);
		SPI.setMISO(_miso);
		SPI.setSCK(_sclk);
		
		SPI.begin();
		
		if (SPI.pinIsChipSelect(_cs, _dc)) {
			_pcs_data = SPI.setCS(_cs);
			_pcs_command = _pcs_data | SPI.setCS(_dc);
		}

		// If the reset feature is used, reset the display.
		// Reset is active low.
		if (_rst < 255) {
			pinMode(_rst, OUTPUT);
			digitalWrite(_rst, HIGH);
			delay(5);
			digitalWrite(_rst, LOW);
			delay(20);
			digitalWrite(_rst, HIGH);
			delay(150);
		}
		
		// Send init commands
		SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
		const uint8_t *addr = init_commands;
		while (1) {
			uint8_t count = *addr++;
			if (count-- == 0) break;
			writeCommand(*addr++);
			while (count-- > 0) {
				writeData8(*addr++);
			}
		}
		writeCommand_last(ILI9341_SLPOUT);    // Exit Sleep
		SPI.endTransaction();

		// Turn on the display after a delay
		delay(120); 		
		SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
		writeCommand_last(ILI9341_DISPON);    // Display on
		SPI.endTransaction();
		
		// Turn on the backlight
		if (_bklt != 255){
			pinMode(_bklt, OUTPUT);
			digitalWrite(_bklt, HIGH);
		}

		// Create the framebuffer
		framebuffer = new FrameBuffer( pixelFormat, (uint16_t)(width/_px+0.5), (uint16_t)(height/_px+0.5) );

		// Create the linebuffer
		linebuffer = new LineBuffer( pixelFormat, (uint16_t)(width/_px+0.5) );

		// Reset draw area of display to first line
		resetDestinationLine();
	}

	/**
	 * Destructor
	 */
	DisplayILI9341::~DisplayILI9341( void ){
		delete framebuffer;
		delete linebuffer;
		// XXX: Shut down display?
	}

	/**
	 * Update the framebuffer to the display
	 * @param	continuous	If true, will continuously refresh until stopRefresh is called
	 **/
	void DisplayILI9341::update( boolean continuous ){
		if (framebuffer->invalidated()){	
			SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));

			// Full framebuffer
			resetDestinationArea();
			writeCommand(ILI9341_RAMWR);

			// No scaling. 1:1 framebuffer to display
			if (_px == pixelScale_1x1){
				/*
				// XXX: Break up into transactions?
				uint16_t* buffer_end_p = &framebuffer->data.data16[framebuffer->count-1];
				uint16_t* buffer_p = framebuffer->data.data16;

				// Quick write out the data;
				while (buffer_p < buffer_end_p) {
					writeData16(*buffer_p++);
				}
				writeData16_last(*buffer_p);
				*/
				uint8_t mv = (framebuffer->height>>4) + 1;
				uint8_t mh = (framebuffer->width>>4) + 1;
				uint8_t v = 0; // vert
				uint8_t h = 0; // horiz
				ClipRect* rect = new ClipRect();
				while (v < mv){
					rect->y = v<<4;
					h = 0;
					while (h < mh){
						rect->x = h<<4;
						rect->setSize(16,16);
						this->updateRect( rect );
						h++;
					}
					v++;
				}
				delete rect;
			}
			else{
				float x = 0;
				float y = 0;
				uint32_t i = 0;
				uint32_t t = width * height;

				while (++i < t){
					writeData16( framebuffer->data.data16[ framebuffer->index( (int16_t)x, (int16_t)y ) ] );
					x += _ipx;
					if (x >= framebuffer->width){
						x = 0;
						y += _ipx;
					}
				}
				writeData16_last( framebuffer->data.data16[ framebuffer->count-1 ] );
			}

			SPI.endTransaction();
		}
		else{
			// Step all dirty tiles line by line
			uint8_t v = 0; // vert
			uint8_t h = 0; // horiz
			uint32_t m = 0; // mask
			uint32_t b = 0; // buffer
			uint8_t mv = (framebuffer->height>>4) + 1;
			uint8_t mh = (framebuffer->width>>4) + 1;
/*
Serial.println("Invalidate buffer:");
uint8_t t = 0;
while (t < mv){
	b = framebuffer->invalideBuffer(t);
	Serial.println(b,BIN);
	t++;
}
*/
			uint8_t i = 0;
			ClipRect* rect = new ClipRect();
			while (v < mv){
				// Something in line is invalidated
				b = framebuffer->invalideBuffer(v);
				if (b){
					m = 1;
					h = 0;
					while (h < mh){
						if (b & m){
							// This bit is invalidated
							if (!i){
								rect->x = h<<4;
								rect->y = v<<4;
								rect->setHeight(16);
							}
							i++;
						}
						else if (i){
							rect->setWidth(16*i);
							this->updateRect( rect );
							i = 0;
						}
						m = m << 1;
						h++;
					} // h
					if (i){
						rect->setWidth(16*i);
						this->updateRect( rect );
						i = 0;
					} // i after h

				} //b
				v++;
			} // v
			delete rect;
		}
		framebuffer->validate();
	}

	/**
	 * Update the linebuffer to the display
	 * @param	continuous	If true, will continuously refresh until stopRefresh is called
	 **/
	void DisplayILI9341::updateLine( boolean continuous ){

		// Get reference to the back buffer
		LineBufferData* data = &linebuffer->data[ linebuffer->backIndex ];

		// Begin the transmission to hardware
		SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
		// Set the area of the display to write to (a single scan line)
		setDestinationLine( data );
		// Tell display we are about to send data
		writeCommand(ILI9341_RAMWR);

		// No pixel scaling (1:1). Send data sequentially as 16bit words
		if (_px == pixelScale_1x1){
			uint16_t x = linebuffer->data[ linebuffer->backIndex ].x1;
			uint16_t i = linebuffer->data[ linebuffer->backIndex ].x0;

			while (i < x){
				writeData16( data->pixels.data16[ i ] );
				i++;
			}
			writeData16_last( data->pixels.data16[ i ] );
		}

		// Pixel scaling. Send data sequentially as 16bit words, sending each pixel
		// multiple times depending on the user-defined pixel scale.
		else {
			uint16_t i = 0; // Destination index
			float si = linebuffer->data[ linebuffer->backIndex ].x0; // Source index
			uint16_t c = (linebuffer->data[ linebuffer->backIndex ].x1 - linebuffer->data[ linebuffer->backIndex ].x0) * _px; // Pixel count
			while (i++ < c){
				writeData16( data->pixels.data16[ (uint16_t)si ] );
				si += _ipx;
			}
			writeData16_last( data->pixels.data16[ (uint16_t)si ] );
		}

		SPI.endTransaction();
	}

	void DisplayILI9341::updateRect( ClipRect* rect ){
		rect->clipPosAndSize(0,0,framebuffer->width,framebuffer->height);
/*
Serial.print("  updateRect ");
Serial.print( rect->x );
Serial.print(", ");
Serial.print( rect->y );
Serial.print("  ");
Serial.print( rect->width );
Serial.print(" x ");
Serial.println( rect->height );
*/
		if (!rect->isEmpty()){
			SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));

			setDestinationArea( rect );
			writeCommand(ILI9341_RAMWR);

			// No scaling. 1:1 framebuffer to display
			if (_px == pixelScale_1x1){
				/*
				uint16_t y = rect->y;
				uint16_t x;
				uint32_t i;

				// Write out the data
				while(y < rect->y2){
					x = rect->x;
					i = y * framebuffer->width + x;
					while(x <= rect->x2){
						writeData16( framebuffer->data.data16[ i++ ] );
					}
					y++;
				}
				// Write last line except last pixel
				x = rect->x;
				i = y * framebuffer->width + x;
				while(x < rect->x2){
					writeData16( framebuffer->data.data16[ i++ ] );
				}
				writeData16_last( framebuffer->data.data16[ i ] );
				*/
				uint16_t x = rect->x;
				uint16_t y = rect->y;
				uint32_t i = 0;
				uint32_t t = rect->width * rect->height;

				while (++i < t){
					writeData16( framebuffer->data.data16[ framebuffer->index( x, y ) ] );
					x++;
					if (x > rect->x2){
						x = rect->x;
						y++;
					}
				}
				writeData16_last( framebuffer->data.data16[ framebuffer->index( x, y ) ] );
			}
			else{
				float x = rect->x;
				float y = rect->y;
				uint32_t i = 0;
				uint32_t t = rect->width * rect->height;

				while (++i < t){
					writeData16( framebuffer->data.data16[ framebuffer->index( (int16_t)x, (int16_t)y ) ] );
					x += _ipx;
					if (x > rect->x2){
						x = rect->x;
						y += _ipx;
					}
				}
				writeData16_last( framebuffer->data.data16[ framebuffer->index( (int16_t)x, (int16_t)y ) ] );
			}

			SPI.endTransaction();
		}
	}
	
	/**
	 * Turn the backlight on or off. If the backlight pin is set, the backlight
	 * is turned on automatically during init/construction. If you want it off, call
	 * setbacklight(0) after construction.
	 * @param	state	0 = off, 1 = on
	 **/
	void DisplayILI9341::setBacklight( boolean state ){
		if (_bklt==255) return;
		
		if (state){
			digitalWrite(_bklt, HIGH);
		}
		else{
			digitalWrite(_bklt, LOW);
		}
	}
	
	/**
	 * Sets the destination drawing area on the display
	 **/
	__attribute__((always_inline)) inline void DisplayILI9341::setDestinationArea( ClipRect* clipRect ){
		writeCommand(ILI9341_CASET); // Column addr set
		writeData16(clipRect->x);
		writeData16(clipRect->x2);
		writeCommand(ILI9341_PASET); // Row addr set
		writeData16(clipRect->y);
		writeData16(clipRect->y2);
	}
	__attribute__((always_inline)) inline void DisplayILI9341::resetDestinationArea( void ){
		writeCommand(ILI9341_CASET); // Column addr set
		writeData16( 0 );
		writeData16( width - 1 );
		writeCommand(ILI9341_PASET); // Row addr set
		writeData16( 0 );
		writeData16( height - 1 );
	}

	/**
	 * @brief Reset the display to draw to the first row
	 */
	__attribute__((always_inline)) inline void DisplayILI9341::resetDestinationLine( void ){
		_y = 0;
		_x0 = 0;
		_x1 = width - 1;
		writeCommand(ILI9341_CASET); // Column addr set
		writeData16( _x0 );
		writeData16( _x1 );
		writeCommand(ILI9341_PASET); // Row addr set
		writeData16( _y );
		writeData16( _y );
	}

	/**
	 * @brief Set the display to draw to the area specified by the supplied data
	 * @param	data		The properties of the line to draw to the display
	 */
	__attribute__((always_inline)) inline void DisplayILI9341::setDestinationLine( LineBufferData* data ) {
		if ( ( data->x0 != _x0 ) || ( data->x1 != _x1 ) ) {
			_x0 = data->x0;
			_x1 = data->x1;
			writeCommand(ILI9341_CASET); // Column addr set
			writeData16( _x0 * _px );
			writeData16( _x1 * _px );
		}
		if ( data->y != _y ) {
			_y = data->y;
			writeCommand(ILI9341_PASET); // Row addr set
			writeData16( _y * _px );
			writeData16( _y * _px );
		}
	}
	
	void DisplayILI9341::waitFifoNotFull(void) {
		uint32_t sr;
		uint32_t tmp __attribute__((unused));
		do {
			sr = KINETISK_SPI0.SR;
			if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
		} while ((sr & (15 << 12)) > (3 << 12));
	}
	void DisplayILI9341::waitFifoEmpty(void) {
		uint32_t sr;
		uint32_t tmp __attribute__((unused));
		do {
			sr = KINETISK_SPI0.SR;
			if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
		} while ((sr & 0xF0F0) > 0);             // wait both RX & TX empty
	}
	
	__attribute__((always_inline)) inline void DisplayILI9341::waitTransmitComplete(void){
		uint32_t tmp __attribute__((unused));
		while (!(KINETISK_SPI0.SR & SPI_SR_TCF)) ; // wait until final output done
		tmp = KINETISK_SPI0.POPR;                  // drain the final RX FIFO word
	}
	__attribute__((always_inline)) inline void DisplayILI9341::waitTransmitComplete(uint32_t mcr){
		uint32_t tmp __attribute__((unused));
		while (1) {
			uint32_t sr = KINETISK_SPI0.SR;
			if (sr & SPI_SR_EOQF) break;  // wait for last transmit
			if (sr &  0xF0) tmp = KINETISK_SPI0.POPR;
		}
		KINETISK_SPI0.SR = SPI_SR_EOQF;
		SPI0_MCR = mcr;
		while (KINETISK_SPI0.SR & 0xF0) {
			tmp = KINETISK_SPI0.POPR;
		}
	}
	
	__attribute__((always_inline)) inline void DisplayILI9341::writeCommand(uint8_t c){
		KINETISK_SPI0.PUSHR = c | (_pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}
	__attribute__((always_inline)) inline void DisplayILI9341::writeCommand_last(uint8_t c){
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = c | (_pcs_command << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}
	
	__attribute__((always_inline)) inline void DisplayILI9341::writeData8(uint8_t c){
		KINETISK_SPI0.PUSHR = c | (_pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}
	__attribute__((always_inline)) inline void DisplayILI9341::writeData8_last(uint8_t c){
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = c | (_pcs_data << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}
	
	__attribute__((always_inline)) inline void DisplayILI9341::writeData16(uint16_t d){
		KINETISK_SPI0.PUSHR = d | (_pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
		waitFifoNotFull();
	}
	__attribute__((always_inline)) inline void DisplayILI9341::writeData16_last(uint16_t d){
		uint32_t mcr = SPI0_MCR;
		KINETISK_SPI0.PUSHR = d | (_pcs_data << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
		waitTransmitComplete(mcr);
	}

} // namespace
