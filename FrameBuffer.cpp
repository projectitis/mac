/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "FrameBuffer.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * ###
	 * ### COLOR PREPARED
	 * ###
	 */

	/**
	 * Constructor
	 */
	PreparedColorS::PreparedColorS( color888 c, alpha a, PixelFormat pixelFormat ){
		this->pf = pixelFormat;
		this->setAlpha( a );
		this->setColor( c );
	}

	/**
	 * Set the alpha value as float
	 * @param a  Alpha (0.0 - 1.0)
	 */
	void PreparedColorS::setAlpha( alpha a ){
		this->a = alphaClamp( a );
		switch (this->pf){
			case PF_565:
				this->a5 = (uint8_t)(this->a*31);
				break;
			case PF_888:
				this->a8 = (uint8_t)(this->a*255);
				break;
			case PF_INDEXED:// XXX: Indexed and mono
			default:
				break;
		}
	}

	/**
	 * Set the alpha value as 8-bit
	 * @param a  Alpha (0 - 255)
	 */
	void PreparedColorS::setAlpha( uint8_t a ){
		setAlpha( (float)(a / 255) );
	}

	/**
	 * Set the color as 24-bit RGB888
	 * @param c The color
	 */
	void PreparedColorS::setColor( color888 c ){
		this->c = c;
		switch (this->pf){
			case PF_565:
				this->c565 = convert888to565( this->c );
				this->cPre = colorPrepare565( this->c565 );
				break;
			case PF_888:
				colorPrepare888( c, this->cRB, this->cG );
				break;
			case PF_INDEXED:// XXX: Indexed and mono
			default:
				break;
		}
	}

	/**
	 * Set the color as R G B components
	 * @param R 	Color red component
	 * @param G 	Color green component
	 * @param B 	Color blue component
	 */
	void PreparedColorS::setColor( uint8_t R, uint8_t G, uint8_t B ){
		setColor( R<<16 | G<<8 | B );
	}

	/**
	 * Set the color as 24-bit RGB888, and the alpha as float
	 * @param c   	The color
	 * @param a 	The alpha (0.0 - 1.0)
	 */
	void PreparedColorS::set( color888 c, alpha a ){
		setColor( c );
		setAlpha( a );
	}

	/**
	 * Set the color as 24-bit RGB888, and the alpha as 8-bit
	 * @param c   	The color
	 * @param a 	The alpha (0 - 255)
	 */
	void PreparedColorS::set( color888 c, uint8_t a ){
		setColor( c );
		setAlpha( (float)(a / 255) );
	}

	/**
	 * Set the color as R G B components, and the alpha as 8-bit
	 * @param R 	Color red component
	 * @param G 	Color green component
	 * @param B 	Color blue component
	 * @param a 	The alpha (0 - 255)
	 */
	void PreparedColorS::set( uint8_t R, uint8_t G, uint8_t B, uint8_t A ){
		setColor( R<<16 | G<<8 | B );
		setAlpha( (float)(a / 255) );
	}

	/**
	 * ###
	 * ### FRAMEBUFFER
	 * ###
	 */

	/**
	 * Constructor
	 */
	FrameBuffer::FrameBuffer( PixelFormat pixelFormat, uint16_t w, uint16_t h ){
		width = w;
		height = h;
		count = w*h;
		switch (pixelFormat){
			case PF_INDEXED: data.data8 = new uint8_t[count]; break; // XXX: Implement indexed color
			case PF_565: data.data16 = new uint16_t[count]; break;
			case PF_888: data.data32 = new uint32_t[count]; break;
			case PF_MONO: data.data8 = new uint8_t[(uint32_t)((count+7)/8)]; break;
			default: // XXX: Handle this error
				break;
		}
		this->pixelFormat = pixelFormat;

		_invalidateBuffer = new uint32_t[(height>>4)+1];
		invalidate();
	}

	/**
	 * Destructor
	 */
	FrameBuffer::~FrameBuffer(){
		switch (pixelFormat){
			case PF_INDEXED: delete[] data.data8; break;
			case PF_565: delete[] data.data16; break;
			case PF_888: delete[] data.data32; break;
			case PF_MONO: delete[] data.data8; break;
			default: 
				break;
		}
	}

	/**
	 * Prepare a color for fast blending by pre-calculating a few things. Don't generally use
	 * this unless you know what it's for. See GraphicsVectorExtn for an example.
	 * @param  c The color in RGB888 24-bit format 
	 * @param  a The alpha (0.0 - 1.0)
	 * @return   The prepared color
	 */
	PreparedColor* FrameBuffer::prepareColor( color888 c, alpha a ){
		return new PreparedColor( c, a, pixelFormat );
	}

	/**
	 * Fill the framebuffer with a color
	 **/
	void FrameBuffer::clear( color888 c ){
		invalidate();
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
	 * @param  y 	The Y coordinate
	 */
	void FrameBuffer::pixel( color888 c, int16_t x, int16_t y ){
		switch (pixelFormat){
			case PF_565: {
				writePixelClip( convert888to565( c ), x, y );
				break;
			}
			case PF_888: {
				writePixelClip( c, x, y );
				break;
			}
			case PF_INDEXED: {
				// XXX: Pick color index from table that is closest to specified
				break;
			}
			case PF_MONO: {
				writePixelMonoClip( c, x, y );
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
	 * @param  y 	The Y coordinate
	 */
	void FrameBuffer::blend( color888 c, alpha a, int16_t x, int16_t y ){
		switch (pixelFormat){
			case PF_565: {
				blendPixelClip( convert888to565( c ), alpha5bit(a), x, y );
				break;
			}
			case PF_888: {
				blendPixelClip( c, alpha8bit(a), x, y );
				break;
			}
			case PF_INDEXED: {
				// XXX: Pick color index from table that is closest to specified
				break;
			}
			case PF_MONO: {
				blendPixelMonoClip( c, alpha8bit(a), x, y );
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
	 * @param  y 	The Y coordinate
	 * @return 		The color (pixel) in RGB 888 format
	 */
	color888 FrameBuffer::pixel( uint32_t i ){
		switch (pixelFormat){
			case PF_565: {
				color565 c;
				readPixelClip( i, c );
				return convert565to888( c );
			}
			case PF_888: {
				color888 c;
				readPixelClip( i, c );
				return c;
			}
			case PF_INDEXED: {
				// XXX: Pick color from table by index
				return 0;
			}
			case PF_MONO: {
				uint8_t c;
				readPixelMonoClip( i, c );
				return (c==0)?RGB888_Black:RGB888_White;
			}
			default:
				return 0;
				break;
		}
	}
	color888 FrameBuffer::pixel( int16_t x, int16_t y ){
		return pixel( index(x,y) );
	}


	/**
	 * Move an area of the framebuffer within itself
	 * @param rect The area to move
	 * @param dx   The X distance to move by
	 * @param dy   The Y distance to move by
	 */
	void FrameBuffer::scroll( ClipRect rect, int16_t dx, int16_t dy ){
		switch (pixelFormat){
			case PF_565: _scroll16( rect, dx, dy ); break;
			case PF_888: _scroll32( rect, dx, dy ); break;
			case PF_MONO: _scroll1( rect, dx, dy ); break;
			case PF_INDEXED:
			default:
				_scroll8( rect, dx, dy ); break;
		}
	}
	void FrameBuffer::_scroll8( ClipRect rect, int16_t dx, int16_t dy ){
		rect.clipPosAndSize( 0,0, width,height );
	}
	void FrameBuffer::_scroll16( ClipRect rect, int16_t dx, int16_t dy ){
		
	}
	void FrameBuffer::_scroll32( ClipRect rect, int16_t dx, int16_t dy ){
		
	}
	void FrameBuffer::_scroll1( ClipRect rect, int16_t dx, int16_t dy ){
		
	}

	/**
	 * Write the framebuffer to a File object as a BMP.
	 * Thanks to https://gist.github.com/binshengliu/1723715
	 * Note that BMP is little endian. Teensy is also little endian, so this works ok. If
	 * porting this to another system, please beware of endian-ness.
	 * @param  file 	The file to write to
	 * @param  area 	An area of the framebuffer to output
	 * @return      	True if successful
	 */
	boolean FrameBuffer::bmp( File& file, ClipRect* area ){
		uint8_t header[54] = {66,77,0,0,0,0,0,0,0,0,54,0,0,0,40,0,0,0,0,0,0,0,0,0,0,0,1,0,24}; //rest is zeroes
		uint32_t pixelBytesPerRow = width * 3;
		uint32_t paddingBytesPerRow = (4-(pixelBytesPerRow%4))%4;
		uint32_t* sizeOfFileEntry = (uint32_t*) &header[2];
		*sizeOfFileEntry = 54 + (pixelBytesPerRow+paddingBytesPerRow)*height;  
		uint32_t* widthEntry = (uint32_t*) &header[18];    
		*widthEntry = width;
		int32_t* heightEntry = (int32_t*) &header[22];    
		*heightEntry = -height;
		file.write( header, 54 );

		uint8_t zeroes[3] = {0,0,0}; //for padding
		uint32_t ri = index( area->x, area->y );
		uint32_t i = ri;
		uint8_t R,G,B;
		color888 c;
		for (int16_t row = 0; row < area->height; row++){
			for (int16_t col = 0; col < area->width; col++){
				c = pixel( i );
				convert888toRGB( c, R, G, B );
				file.write( B );
				file.write( G );
				file.write( R );
				i++;
			}
			file.write( zeroes, paddingBytesPerRow );
			ri += width;
			i = ri;
		}
		return true;
	}

	boolean FrameBuffer::bmp( File& file ){
		ClipRect* area = new ClipRect();
		area->setPosAndSize(  0,0, width,height );
		boolean result = bmp( file, area );
		delete area;
		return result;
	}

	boolean FrameBuffer::invalidated(){
		return _invalidated;
	}

	void FrameBuffer::validate(){
		_invalidated = false;
		uint8_t v = (height>>4)+1;
		while (v)_invalidateBuffer[--v] = 0x0;
	}

	uint32_t FrameBuffer::invalideBuffer( uint8_t v ){
		return _invalidateBuffer[v];
	}

	void FrameBuffer::invalidate(){
		_invalidated = true;
		uint8_t v = (height>>4)+1;
		while (v)_invalidateBuffer[--v] = 0xFFFFFFFF;
	}

	void FrameBuffer::invalidate( int16_t x, int16_t y ){
		if ((x>=0) && (x<width) && (y>=0) && (y<height)){
			_invalidateBuffer[y>>4] |= (x>>4);
		}
	}

	void FrameBuffer::invalidate( uint32_t i ){
		if (i>=0 && i<count){
			uint16_t y = uint16_t( i/width ); 
			uint16_t x = i - (y*width);
			_invalidateBuffer[y>>4] |= (x>>4);
		}
	}

	void FrameBuffer::invalidateRect( int16_t x, int16_t y, int16_t w, int16_t h ){
		// Clip
		if ((w<0) || (h<0)) return;
		if (x<0){
			w += x;
			x = 0;
		}
		if (y<0){
			h += y;
			y = 0;
		}
		if ((x<width) && (y<height)){
			if (x+w >= width) w = width-x-1;
			if (y+h >= height) h = height-y-1;
			if ((w>0) && (y>0)){
/*
Serial.print("invalidateRect ");
Serial.print( x );
Serial.print(", ");
Serial.print( y );
Serial.print("  ");
Serial.print( w );
Serial.print(" x ");
Serial.println( h );
*/
				uint16_t x1 = 1 << (x>>4);
				uint16_t x2 = 1 << ((x+w-1)>>4);
				uint16_t y1 = y>>4;
				uint16_t y2 = (y+h-1)>>4;
/*
Serial.print("x1 "); Serial.print( x1 );
Serial.print(" x2 "); Serial.println( x2 );
Serial.print("y1 "); Serial.print( y1 );
Serial.print(" y2 "); Serial.println( y2 );
*/
				while(y1<=y2){
					x = x1;
					while(x<=x2){
						_invalidateBuffer[y1] |= x;
//Serial.print("#");
						x = x << 1;
					}
					y1++;
				}
//Serial.println("");
			}
		}
	}

	void FrameBuffer::invalidateRect( ClipRect* rect ){
		rect->clipPosAndSize(0,0,width,height);
		if ((rect->width>0) && (rect->height>0)){
			uint16_t x1 = (rect->x)>>4;
			uint16_t x2 = (rect->x2)>>4;
			uint16_t y1 = (rect->y)>>4;
			uint16_t y2 = (rect->y2)>>4;
			uint16_t x;
			while(y1<=y2){
				x = x1;
				while(x<=x2){
					_invalidateBuffer[y1] |= x;
					x++;
				}
				y1++;
			}
		}
	}

	
} // namespace