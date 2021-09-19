/**
 * Display library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "../DisplayST7735.h"
#include <SPI.h>
#include <DMAChannel.h>

/**
 * Define ST7735 commands
 **/
typedef enum {
    NOP       = 0x00,
	SWRESET   = 0x01, /* Software Reset */
	RDDID     = 0x04, /* Read Display ID */
	RDDST     = 0x09, /* Read Display Status */
	RDDPM     = 0x0a, /* Read Display Power Mode */
	RDDMADCTL = 0x0b, /* Read Display MADCTL */
	RDDCOLMOD = 0x0c, /* Read Display Pixel Format */
	RDDIM     = 0x0d, /* Read Display Image Mode */
	RDDSM     = 0x0e, /* Read Display Signal Mode */
	RDDSDR    = 0x0f, /* Read Display Self-Diagnostic Result */
	SLPIN     = 0x10, /* Sleep In */
	SLPOUT    = 0x11, /* Sleep Out */
	PTLON     = 0x12, /* Partial Display Mode On */
	NORON     = 0x13, /* Normal Display Mode On */
	INVOFF    = 0x20, /* Display Inversion Off */
	INVON     = 0x21, /* Display Inversion On */
	GAMSET    = 0x26, /* Gamma Set */
	DISPOFF   = 0x28, /* Display Off */
	DISPON    = 0x29, /* Display On */
	CASET     = 0x2a, /* Column Address Set */
	RASET     = 0x2b, /* Row Address Set */
	RAMWR     = 0x2c, /* Memory Write */
	RGBSET    = 0x2d, /* Color Setting 4k, 65k, 262k */
	RAMRD     = 0x2e, /* Memory Read */
	PTLAR     = 0x30, /* Partial Area */
	SCRLAR    = 0x33, /* Scroll Area Set */
	TEOFF     = 0x34, /* Tearing Effect Line OFF */
	TEON      = 0x35, /* Tearing Effect Line ON */
	MADCTL    = 0x36, /* Memory Data Access Control */
	VSCSAD    = 0x37, /* Vertical Scroll Start Address of RAM */
	IDMOFF    = 0x38, /* Idle Mode Off */
	IDMON     = 0x39, /* Idle Mode On */
	COLMOD    = 0x3a, /* Interface Pixel Format */
	RDID1     = 0xda, /* Read ID1 Value */
	RDID2     = 0xdb, /* Read ID2 Value */
	RDID3     = 0xdc, /* Read ID3 Value */
	FRMCTR1   = 0xb1, /* Frame Rate Control in normal mode, full colors */
	FRMCTR2   = 0xb2, /* Frame Rate Control in idle mode, 8 colors */
	FRMCTR3   = 0xb3, /* Frame Rate Control in partial mode, full colors */
	INVCTR    = 0xb4, /* Display Inversion Control */
	DISSET5   = 0xB6, /* Display settings */
	PWCTR1    = 0xc0, /* Power Control 1 */
	PWCTR2    = 0xc1, /* Power Control 2 */
	PWCTR3    = 0xc2, /* Power Control 3 in normal mode, full colors */
	PWCTR4    = 0xc3, /* Power Control 4 in idle mode 8colors */
	PWCTR5    = 0xc4, /* Power Control 5 in partial mode, full colors */
	VMCTR1    = 0xc5, /* VCOM Control 1 */
	VMOFCTR   = 0xc7, /* VCOM Offset Control */
	WRID2     = 0xd1, /* Write ID2 Value */
	WRID3     = 0xd2, /* Write ID3 Value */
	NVFCTR1   = 0xd9, /* NVM Control Status */
	NVFCTR2   = 0xde, /* NVM Read Command */
	NVFCTR3   = 0xdf, /* NVM Write Command */
	GMCTRP1   = 0xe0, /* Gamma '+'Polarity Correction Characteristics Setting */
	GMCTRN1   = 0xe1, /* Gamma '-'Polarity Correction Characteristics Setting */
	GCV       = 0xfc, /* Gate Pump Clock Frequency Variable */
} ST7735_Command;

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
	 * @param	width	TFT pixel width
	 * @param	Height	TFT pixel height
	 **/
	DisplayST7735::DisplayST7735(
		DisplaySize sz,
		uint8_t cs,
		uint8_t dc,
		uint8_t rst,
		uint8_t mosi,
		uint8_t sclk,
		uint8_t bklt,
		PixelScale px
	){
		_cs   = cs;
		_dc   = dc;
		_rst  = rst;
		_mosi = mosi;
		_sclk = sclk;
		_bklt = bklt;
		_bkltPWM = false;
		_xoffset = 2;
		_yoffset = 3;
		switch (sz){
			case displaySize_128x128:
			default:
				width    = 128;
				height   = 128;
		}
		pixelFormat = PF_565;
		_px = px;
		rect.setSize( width >> _px, height >> _px );
		region.set( &rect );

		data[0].pixels = new uint32_t[ rect.width ];
		data[0].y = 0;
		data[0].x = 0;
		data[0].x2 = rect.x2;
		data[1].pixels = new uint32_t[ rect.width ];
		data[1].y = 0;
		data[1].x = 0;
		data[1].x2 = rect.x2;
		
		init();
	}

	static const uint8_t init_commands[] = {
		// From Adafruit_ST7735 common
		1, ST7735_Command::SWRESET,
    	1,  ST7735_Command::SLPOUT,
		4, ST7735_Command::FRMCTR1, 0x01, 0x2C, 0x2D, 
		4, ST7735_Command::FRMCTR2, 0x01, 0x2C, 0x2D,
		7, ST7735_Command::FRMCTR3, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D,
		2,  ST7735_Command::INVCTR, 0x07,

		4,  ST7735_Command::PWCTR1, 0xA2, 0x02, 0x84,
		2,  ST7735_Command::PWCTR2, 0xC5,
		3,  ST7735_Command::PWCTR3, 0x0A, 0x00,
		3,  ST7735_Command::PWCTR4, 0x8A, 0x2A,
		3,  ST7735_Command::PWCTR5, 0x8A, 0xEE,
		2,  ST7735_Command::VMCTR1, 0x0E,

		2, ST7735_Command::VMOFCTR, 0x0f,
		1,  ST7735_Command::INVOFF,
		2,  ST7735_Command::MADCTL, 0xC8,
		2,  ST7735_Command::COLMOD, 0x05,

		0
	};
	const uint8_t* DisplayST7735::getInitCommands(){
		return init_commands;
	}

	void DisplayST7735::init( void ){
		
		// Set up SPI pins
		SPI.setMOSI(_mosi);
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
		const uint8_t *addr = getInitCommands();
		while (1) {
			uint8_t count = *addr++;
			if (count-- == 0) break;
			writeCommand(*addr++);
			while (count-- > 0) writeData8(*addr++);
		}
		writeCommand_last( ST7735_Command::DISPON ); // Turn on display
		SPI.endTransaction();
		
		// Turn on the backlight
		if (_bklt != 255){
			pinMode(_bklt, OUTPUT);
			backlight( 1.0 );
		}
	}

	/**
	 * Destructor
	 */
	DisplayST7735::~DisplayST7735( void ){
		delete data[0].pixels;
		delete data[1].pixels;
		// XXX: Shut down display?
	}

	/**
	 * Update the linebuffer to the display
	 * @param	continuous	If true, will continuously refresh until stopRefresh is called
	 **/
	void DisplayST7735::flip() {

		// Flip the buffers
		Display::flip();

// XXX: Use DMA to send data over SPI
//#if MAC_USE_DMA==0
		// DMA is not being used. Immediately send the back buffer to the display
		// as an SPI transaction

		// Get reference to the back buffer (ignore uninitialized warning using pragma)
#pragma GCC diagnostic ignored "-Wuninitialized"
		LineBufferData* back = &data[ backIndex ];

		// Begin the transmission to hardware
		SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));

		// Set the area of the display to write to (a single scan line)
		setDestinationLine( back );

		// Tell display we are about to send data
		writeCommand( ST7735_Command::RAMWR );

		uint16_t i = back->x << _px;
		uint16_t c = i + ((back->x2 - back->x + 1) << _px) - 1;
		uint16_t l = 1 << _px;
		while (l--) {
			i = back->x << _px;
			while (i < (c + ((l>0)?1:0))) writeData16( to565( back->pixels[ i++ >> _px ] ) );
		}
		writeData16_last( to565( back->pixels[ i >> _px ] ) );

		// Done with complete transaction
		SPI.endTransaction();

//#endif // MAC_USE_DMA

		// Indicate that drawing has finished
		ready = 0;
	}
	
	/**
	 * @brief Set the display to draw to the area specified by the supplied data
	 * @param	data		The properties of the line to draw to the display
	 */
	__attribute__((always_inline)) inline void DisplayST7735::setDestinationLine( LineBufferData* data ) {
		writeCommand( ST7735_Command::CASET); // Column addr set
		writeData16( (data->x << _px) + _xoffset );
		writeData16( ((data->x2 + 1) << _px) - 1 + _xoffset );

		writeCommand( ST7735_Command::RASET ); // Row addr set
		writeData16( (data->y << _px) + _yoffset );
		writeData16( ((data->y + 1) << _px) - 1 + _yoffset );
	}

} // namespace
