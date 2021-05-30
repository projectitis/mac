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
typedef enum {
	NOP			= 0x00,
	SWRESET		= 0x01,		// Resets the commands and parameters to their S/W Reset default values
	RDDID		= 0x04,		// Read display identification information (return 24bits)
	RDDST		= 0x09,		// Read display status 

	SLPIN		= 0x10,		// Enter sleep mode
	SLPOUT		= 0x11,		// Turns off sleep mode
	PTLON		= 0x12,		// Turns on partial mode
	NORON		= 0x13,		// Returns the display to normal mode (aprtial mode off)

	RDMODE		= 0x0A,		
	RDMADCTL	= 0x0B,
	RDPIXFMT	= 0x0C,
	RDIMGFMT	= 0x0D,
	RDSELFDIAG	= 0x0F,

	INVOFF		= 0x20,		// Recover from display inversion mode
	INVON		= 0x21,		// Enter into display inversion mode
	GAMMASET	= 0x26,		// Select the desired Gamma curve (only 01 is defined)
	DISPOFF		= 0x28,		// Display off
	DISPON		= 0x29,		// Display on

	CASET		= 0x2A,		// Column Address Set
	PASET		= 0x2B,		// Page Address Set
	RAMWR		= 0x2C,		// Memory Write
	RGBSET		= 0x2D,		// Color Set
	RAMRD		= 0x2E,		// Memory Read

	PTLAR		= 0x30,
	MADCTL		= 0x36,
	VSCRSADD	= 0x37,
	PIXFMT		= 0x3A,

	FRMCTR1		= 0xB1,
	FRMCTR2		= 0xB2,
	FRMCTR3		= 0xB3,
	INVCTR		= 0xB4,
	DFUNCTR		= 0xB6,

	PWCTR1		= 0xC0,		// Power control
	PWCTR2		= 0xC1,
	PWCTR3		= 0xC2,
	PWCTR4		= 0xC3,
	PWCTR5		= 0xC4,
	VMCTR1		= 0xC5,		// Set the VCOM voltage
	VMCTR2		= 0xC7,

	RDID1		= 0xDA,
	RDID2		= 0xDB,
	RDID3 		= 0xDC,
	RDID4		= 0xDD,

	GMCTRP1		= 0xE0,		// Positive Gamma correction
	GMCTRN1		= 0xE1,		// Negative Gamma correction

	IFCTL		= 0xF6		// Interface Control
} ILI9341_Command;

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
		DisplaySize sz,
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
		_bkltPWM = false;
		switch (sz){
			case displaySize_320x240:
			default:
				width    = 320;
				height   = 240;
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
		2, 		ILI9341_Command::PWCTR1, 0x23, 				// Power control
		2, 		ILI9341_Command::PWCTR2, 0x10, 				// Power control
		3, 		ILI9341_Command::VMCTR1, 0x3e, 0x28, 		// VCM control
		2, 		ILI9341_Command::VMCTR2, 0x86, 				// VCM control2
		2, 		ILI9341_Command::MADCTL, 0x48, 				// Memory Access Control
		2, 		ILI9341_Command::PIXFMT, 0x55,				// 16bit 565 pixel format
		3, 		ILI9341_Command::FRMCTR1, 0x00, 0x18,
		4, 		ILI9341_Command::DFUNCTR, 0x08, 0x82, 0x27,	// Display Function Control
		2, 		0xF2, 0x00, 						// Gamma Function Disable
		2, 		ILI9341_Command::GAMMASET, 0x01, 			// Gamma curve selected
		16, 	ILI9341_Command::GMCTRP1, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
				0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // Set Gamma
		16, 	ILI9341_Command::GMCTRN1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
				0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // Set Gamma
		3, 		0xb1, 0x00, 0x10, 					// FrameRate Control 119Hz
		
		2, 		ILI9341_Command::MADCTL, MADCTL_MV | MADCTL_BGR,
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
		writeCommand_last(ILI9341_Command::SLPOUT);    // Exit Sleep
		SPI.endTransaction();

		// Turn on the display after a delay
		delay(120); 		
		SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));
		writeCommand_last(ILI9341_Command::DISPON);    // Display on
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
	DisplayILI9341::~DisplayILI9341( void ){
		delete data[0].pixels;
		delete data[1].pixels;
		// XXX: Shut down display?
	}

	/**
	 * Update the linebuffer to the display
	 * @param	continuous	If true, will continuously refresh until stopRefresh is called
	 **/
	void DisplayILI9341::flip() {

		// Flip the buffers
		Display::flip();

// XXX: Use DMA to send data over SPI
//#if MAC_USE_DMA==0
		// DMA is not being used. Immediately send the back buffer to the display
		// as an SPI transaction

		// Get reference to the back buffer (ignore uninitialized warning)
		#pragma GCC diagnostic ignored "-Wuninitialized"
		LineBufferData* back = &data[ backIndex ];

		// Begin the transmission to hardware
		SPI.beginTransaction(SPISettings(SPICLOCK, MSBFIRST, SPI_MODE0));

		// Set the area of the display to write to (a single scan line)
		setDestinationLine( back );

		// Tell display we are about to send data
		writeCommand( ILI9341_Command::RAMWR );

		uint16_t i = back->x << _px;
		uint16_t c = i + ((back->x2 - back->x + 1) << _px) - 1;
		uint16_t l = 1 << _px;
		while (l--) {
			i = back->x << _px;
			while (i < (c + ((l>0)?1:0))) writeData16( convert888to565( back->pixels[ i++ >> _px ] ) );
		}
		writeData16_last( convert888to565( back->pixels[ i >> _px ] ) );

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
	__attribute__((always_inline)) inline void DisplayILI9341::setDestinationLine( LineBufferData* data ) {
		writeCommand( ILI9341_Command::CASET ); // Column addr set
		writeData16( data->x << _px );
		writeData16( ((data->x2 + 1) << _px) - 1 );

		writeCommand( ILI9341_Command::PASET ); // Row addr set
		writeData16( data->y << _px );
		writeData16( ((data->y + 1) << _px) - 1 );
	}

} // namespace
