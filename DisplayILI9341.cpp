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
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
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
		uint8_t bklt
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
	}

	/**
	 * Update the framebuffer to the display
	 * @param	buffer	A pointer to the buffer
	 * @param	rect	If not NULL, the portion of the buffer to refresh
	 * @param	continuous	If true, will continuously refresh until stopRefresh is called
	 **/
	void DisplayILI9341::update(
		uint16_t* buffer,
		BufferRect* rect,
		boolean continuous		// XXX: Currently unsupported
	){
		SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
		if (rect){
			// Clipping area
			setDestinationArea( rect );
			writeCommand( ILI9341_RAMWR );

			// XXX: Break up into transactions?
			uint16_t* row_p = rect->start;
			for (uint16_t y = 0; y < rect->h; y++) {
				uint16_t* pixel_p = row_p;
				for (uint16_t x = 0; x < (rect->w - 1); x++) {
					writeData16(*pixel_p++);
				}
				if (y < (rect->h - 1))
					writeData16(*pixel_p);
				else	
					writeData16_last(*pixel_p);
				row_p += rect->stride;	// setup for the next row. 
			}
		}
		else{
			// Full framebuffer
			resetDestinationArea();
			writeCommand(ILI9341_RAMWR);

			// XXX: Break up into transactions?
			uint16_t* buffer_end_p = &buffer[(width*height)-1];	// setup 
			uint16_t* buffer_p = buffer;

			// Quick write out the data;
			while (buffer_p < buffer_end_p) {
				writeData16(*buffer_p++);
			}
			writeData16_last(*buffer_p);
		}
		// End transaction
		digitalWriteFast(_dc, 1);
		digitalWriteFast(_cs, 1);
		SPI.endTransaction();
	}
		
	/**
	 * Stop the display from continuously refreshing
	 **/
	void DisplayILI9341::stopRefresh( void ){
		// XXX: Currently unsupported
	}
			
	/**
	 * Wait until the current refresh is complete. Not recommended if you have other
	 * (usually non-graphics) code that shouldn't be delayed!
	 **/
	void DisplayILI9341::waitForRefresh( void ){
		// XXX: Currently unsupported
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
		writeData16(clipRect->x + clipRect->w - 1);
		writeCommand(ILI9341_PASET); // Row addr set
		writeData16(clipRect->y);
		writeData16(clipRect->y + clipRect->h - 1);
	}
	__attribute__((always_inline)) inline void DisplayILI9341::resetDestinationArea( void ){
		writeCommand(ILI9341_CASET); // Column addr set
		writeData16( 0 );
		writeData16( width - 1 );
		writeCommand(ILI9341_PASET); // Row addr set
		writeData16( 0 );
		writeData16( height - 1 );
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
