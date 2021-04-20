/**
 * Display library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "DisplaySSD1306.h"
#include <i2c_t3.h> // Wire replacement

/**
 * Define SSD1306 commands
 **/
#define SSD1306_SETLOWCOLUMN		0x00
#define SSD1306_SETHIGHCOLUMN		0x10
#define SSD1306_MEMORYMODE			0x20
#define SSD1306_COLUMNADDR			0x21
#define SSD1306_PAGEADDR			0x22
#define SSD1306_SETSTARTLINE		0x40
#define SSD1306_DEFAULT_ADDRESS		0x78
#define SSD1306_SETCONTRAST			0x81
#define SSD1306_CHARGEPUMP			0x8D
#define SSD1306_SEGREMAP			0xA0
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 		0xA5
#define SSD1306_NORMALDISPLAY 		0xA6
#define SSD1306_INVERTDISPLAY 		0xA7
#define SSD1306_SETMULTIPLEX 		0xA8
#define SSD1306_DISPLAYOFF 			0xAE
#define SSD1306_DISPLAYON 			0xAF
#define SSD1306_SETPAGE 			0xB0
#define SSD1306_COMSCANINC 			0xC0
#define SSD1306_COMSCANDEC 			0xC8
#define SSD1306_SETDISPLAYOFFSET 	0xD3
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE 		0xD9
#define SSD1306_SETCOMPINS 			0xDA
#define SSD1306_SETVCOMDETECT 		0xDB
#define SSD1306_SWITCHCAPVCC 		0x02
#define SSD1306_NOP 				0xE3

/**
 * Define SSD1306 supported memory modes
 */
#define SSD1306_HORIZONTAL_ADDRMODE 0x00
#define SSD1306_VERTICAL_ADDRMODE 	0x01
#define SSD1306_PAGE_ADDRMODE 		0x02

/**
 * Define I2C parameters
 **/


/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Constructor for I2C.
	 * Note - constructor calls init, so you don't need to.
	 * @param	scl		Pin used for clock
	 * @param	sda		Pin used for data
	 * @param	px 		Scale factor from framebuffer to display. Normally 1:1 (pixelScale_1x1)
	 **/
	DisplaySSD1306::DisplaySSD1306(
		uint8_t scl,
		uint8_t sda,
		PixelScale px
	){
		_scl   	= scl;
		_sda   	= sda;
		_px 	= px;
		width    = 128;
		height   = 64;
		_i2caddr = 0x3D;
		pixelFormat = PF_MONO;
		_ipx = 1.0/(float)_px;
		rect.setSize( (uint16_t)(width * _ipx), (uint16_t)(height * _ipx) );
		
		init();
	}
	
	/**
	 * Initialise the display. Called from the constructor.
	 **/
	void DisplaySSD1306::init( void ){
		// Initialise I2C
		Wire.begin( I2C_MASTER, 0, _scl, _sda, I2C_PULLUP_EXT, 400000 );

		// Create the framebuffer
		framebuffer = new FrameBuffer( pixelFormat, (uint16_t)(width/_px+0.5), (uint16_t)(height/_px+0.5) );
	}

	/**
	 * Destructor
	 */
	DisplaySSD1306::~DisplaySSD1306( void ){
		delete framebuffer;

		// XXX: Shut down display? Shut down I2C?
		
	}

	/**
	 * Update the framebuffer to the display
	 * @param	continuous	If true, will continuously refresh until stopRefresh is called
	 **/
	void DisplaySSD1306::update( boolean continuous ){
		resetDestinationArea();

		// No scaling. 1:1 framebuffer to display
		if (_px == pixelScale_1x1){
			writeData( framebuffer->data.data8, (framebuffer->count+7)/8 );
		}

		// Pixel scaling
		else{
			// Not yet supported
			
		}
	}

	/**
	 * Set drawing area to entire display
	 */
	void DisplaySSD1306::resetDestinationArea(){
		uint8_t commands[] = {
		    SSD1306_PAGEADDR, 0, (uint8_t)(height-1),
		    SSD1306_COLUMNADDR, 0, (uint8_t)(width-1)
		};
		writeCommand(commands, sizeof(commands));
	}

	/**
	 * Set drawing area to cliprect
	 */
	void DisplaySSD1306::setDestinationArea( ClipRect* clipRect ){
		// Ensure area is clipped to display
		ClipRect* displayRect = new ClipRect(0,0,width,height);
		displayRect->clip( clipRect );
		// Set drawing area
		uint8_t commands[] = {
		    SSD1306_PAGEADDR, (uint8_t)displayRect->y, (uint8_t)displayRect->y2,
		    SSD1306_COLUMNADDR, (uint8_t)displayRect->x, (uint8_t)displayRect->x2
		};
		writeCommand(commands, sizeof(commands));
	}

	/**
	 * Begin a transmission
	 */
	void DisplaySSD1306::beginTransmission(){
		Wire.beginTransmission( _i2caddr );
	}

	/**
	 * End a transmission
	 */
	void DisplaySSD1306::endTransmission(){
		Wire.endTransmission();
	}

	/**
	 * Write a data byte
	 */
	void DisplaySSD1306::writeData( uint8_t c ){
		beginTransmission();
		Wire.write((uint8_t)0x40);
	    Wire.write(c);
	    endTransmission();
	}

	/**
	 * Write list of data bytes
	 */
	void DisplaySSD1306::writeData( uint8_t *c, uint32_t n ) {
		while (n){
			beginTransmission();
			Wire.write((uint8_t)0x40);
			uint32_t sending = min(n, I2C_TX_BUFFER_LENGTH-1);
	   		Wire.write(c , sending);
	   		endTransmission();
	   		n -= sending;
	   	}
	}

	/**
	 * Write a command
	 */
	void DisplaySSD1306::writeCommand( uint8_t c ){
		beginTransmission();
	    Wire.write((uint8_t)0x00);
	    Wire.write(c);
	    endTransmission();
	}

	/**
	 * Write a list of commands
	 */
	void DisplaySSD1306::writeCommand( uint8_t *c, uint32_t n ) {
		while (n){
			beginTransmission();
			Wire.write((uint8_t)0x00);
			uint32_t sending = min(n, I2C_TX_BUFFER_LENGTH-1);
	   		Wire.write(c , sending);
	   		endTransmission();
	   		n -= sending;
	   	}
	}

} // namespace
