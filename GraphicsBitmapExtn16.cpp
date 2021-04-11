/**
 * Graphics library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "GraphicsBitmapExtn16.h"
#include "Arduino.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Blit a tile from the tilemap
	 * @param 	tileMap A tilemap containing the tile data
	 * @param 	index   The index of the tile in the map
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	alpha	The alpha value (0.0-1.0)
	 * @param	r		The rotation
	 */
	void GraphicsBitmapExtn16::blit(
		const Tilemap* tilemap,
		uint32_t index,
		int16_t x,
		int16_t y,
		alpha alpha,
		BitmapRotation r
	){
		// Get source address (p) and step (s)
		uint32_t si = index * tilemap->tileWidth * tilemap->tileHeight;		// Index into the source, in pixels
		uint32_t di;						// Destination index 
		int16_t dp;							// Destination pixel step 
		int16_t ds;							// Destination stride 
		uint16_t sw = tilemap->tileWidth;	// Number of pixels to draw horizontally
		uint16_t sh = tilemap->tileHeight;	// Number of pixels to draw vertically

		// Calculate clipping values
		if (!_clip( r, x, y, sw, sh, si, di, dp, ds )) return;

		// Draw tilemap with full alpha or on/off transparency
		uint8_t pw = pixelFormatBitWidth( tilemap->pixelFormat );
		if (pw < 8) {
			// Single-channel grayscale pixel format
			_blitG( (uint8_t*)tilemap->data, getAccessor5565( tilemap->pixelFormat ), pw, si, sw, sh, tilemap->tileWidth, alpha, di, dp, ds, tilemap->transparentColor ); 
		}
		else {
			// Whole-byte pixel formats
			pw >>= 3; // Bits to bytes
			if (pixelFormatHasAlpha(tilemap->pixelFormat)){
				_blitA( (uint8_t*)tilemap->data, getAccessor5565( tilemap->pixelFormat ), pw, si, sw, sh, tilemap->tileWidth, alpha, di, dp, ds );
			}
			else{
				_blitT( (uint8_t*)tilemap->data, getAccessor5565( tilemap->pixelFormat ), pw, si, sw, sh, tilemap->tileWidth, alpha, di, dp, ds, tilemap->transparentColor ); 
			}
		}
	}

	/**
	 * Blit a bitmap
	 * @param 	bitmap 	A bitmap containing the pixel data
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	alpha	The alpha value (0.0-1.0)
	 * @param	r		The rotation
	 */
	void GraphicsBitmapExtn16::blit(
		Bitmap bitmap,
		int16_t x,
		int16_t y,
		alpha alpha,
		BitmapRotation r
	){
		// Get source address (p) and step (s)
		uint32_t si = 0;					// Source index
		uint32_t di;						// Destination index 
		int16_t dp;							// Destination pixel step 
		int16_t ds;							// Destination stride 
		uint16_t sw = bitmap.width;			// Number of pixels to draw horizontally
		uint16_t sh = bitmap.height;		// Number of pixels to draw vertically

		// Calculate clipping values
		if (!_clip( r, x, y, sw, sh, si, di, dp, ds )) return;

		// Draw bitmap with full alpha or on/off transparency
		uint8_t pw = pixelFormatBitWidth( bitmap.pixelFormat );
		if (pw < 8) {
			// Single-channel grayscale pixel format
			_blitG( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, bitmap.transparentColor ); 
		}
		else {
			// Whole-byte pixel formats
			pw >>= 3; // Bits to bytes
			if (pixelFormatHasAlpha(bitmap.pixelFormat)){
				_blitA( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds ); 
			}
			else{
				_blitT( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, bitmap.transparentColor ); 
			}
		}
	}

	/**
	 * Blit an area of a bitmap
	 * @param 	bitmap 	A bitmap containing the pixel data
	 * @param 	rect 	The area of the bitmap to blit
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	alpha	The alpha value (0.0-1.0)
	 * @param	r		The rotation
	 */
	void GraphicsBitmapExtn16::blit(
		Bitmap bitmap,
		ClipRect* rect,
		int16_t x,
		int16_t y,
		alpha alpha,
		BitmapRotation r
	){
		// Calculate drawable area of bitmap
		ClipRect* sr = new ClipRect( 0,0, bitmap.width,bitmap.height );
		sr->clip( rect );

		// Set up src/dest values for drawing
		x += sr->x;
		y += sr->y;
		uint32_t si = y * bitmap.width + x;	// Source index
		uint32_t di;						// Destination index 
		int16_t dp;							// Destination pixel step 
		int16_t ds;							// Destination stride 
		uint16_t sw = sr->width;			// Number of pixels to draw horizontally
		uint16_t sh = sr->height;			// Number of pixels to draw vertically

		// Calculate clipping values
		if (!_clip( r, x, y, sw, sh, si, di, dp, ds )) return;

		// Draw bitmap with full alpha or on/off transparency
		uint8_t pw = pixelFormatBitWidth( bitmap.pixelFormat );
		if (pw < 8) {
			// Single-channel grayscale pixel format
			_blitG( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, bitmap.transparentColor ); 
		}
		else {
			// Whole-byte pixel formats
			pw >>= 3; // Bits to bytes
			if (pixelFormatHasAlpha(bitmap.pixelFormat)){
				_blitA( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds ); 
			}
			else{
				_blitT( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, bitmap.transparentColor ); 
			}
		}
	}

	/**
	 * Blit a tile from the tilemap as a stamp
	 * @param 	color 	The color of the stamp
	 * @param 	tileMap A tilemap containing the tile data
	 * @param 	index   The index of the tile in the map
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	alpha	The alpha value (0.0-1.0)
	 * @param	r		The rotation
	 */
	void GraphicsBitmapExtn16::stamp(
		color888 color,
		const Tilemap* tilemap,
		uint32_t index,
		int16_t x,
		int16_t y,
		alpha alpha,
		BitmapRotation r
	){
		// Get source address (p) and step (s)
		uint32_t si = index * tilemap->tileWidth * tilemap->tileHeight;		// Index into the source, in pixels
		uint32_t di;						// Destination index 
		int16_t dp;							// Destination pixel step 
		int16_t ds;							// Destination stride
		uint16_t sw = tilemap->tileWidth;	// Number of pixels to draw horizontally
		uint16_t sh = tilemap->tileHeight;	// Number of pixels to draw vertically

		// Calculate clipping values
		if (!_clip( r, x, y, sw, sh, si, di, dp, ds )) return;

		// Draw tilemap as stamp with full alpha or on/off transparency
		uint8_t pw = pixelFormatBitWidth( tilemap->pixelFormat );	
		color565 sc = convert888to565( color );
		if (pw < 8) {
			// Single-channel grayscale pixel formats
			_stampG( (uint8_t*)tilemap->data, getAccessor5565( tilemap->pixelFormat ), pw, si, sw, sh, tilemap->tileWidth, alpha, di, dp, ds, sc, tilemap->transparentColor ); 
		}
		else {
			// Whole byte pixel formats
			pw >>= 3; // Bits to bytes
			if (pixelFormatHasAlpha(tilemap->pixelFormat)){
				_stampA( (uint8_t*)tilemap->data, getAccessor5565( tilemap->pixelFormat ), pw, si, sw, sh, tilemap->tileWidth, alpha, di, dp, ds, sc ); 
			}
			else{
				_stampT( (uint8_t*)tilemap->data, getAccessor5565( tilemap->pixelFormat ), pw, si, sw, sh, tilemap->tileWidth, alpha, di, dp, ds, sc, tilemap->transparentColor ); 
			}
		}
	}

	/**
	 * Blit an area of a tile from the tilemap as a stamp
	 * @param 	color 	The color of the stamp
	 * @param 	tileMap A tilemap containing the tile data
	 * @param 	index   The index of the tile in the map
	 * @param 	rect 	The area of the tile to blit
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	alpha	The alpha value (0.0-1.0)
	 * @param	r		The rotation
	 */
	void GraphicsBitmapExtn16::stamp(
		color888 color,
		const Tilemap* tilemap,
		uint32_t index,
		ClipRect* rect,
		int16_t x,
		int16_t y,
		alpha alpha,
		BitmapRotation r
	) {
		// Calculate drawable area of tile
		ClipRect* sr = new ClipRect( 0,0, tilemap->tileWidth,tilemap->tileHeight );
		sr->clip( rect );

		// Set up src/dest values for drawing
		uint32_t si = ( index * tilemap->tileWidth * tilemap->tileHeight ) + ( sr->y * tilemap->tileWidth + sr->x );		// Index into the source, in pixels
		uint32_t di;						// Destination index 
		int16_t dp;							// Destination pixel step 
		int16_t ds;							// Destination stride
		uint16_t sw = sr->width;			// Number of pixels to draw horizontally
		uint16_t sh = sr->height;			// Number of pixels to draw vertically

		// Calculate clipping values
		if (!_clip( r, x, y, sw, sh, si, di, dp, ds )) return;

		// Draw tilemap as stamp with full alpha or on/off transparency
		uint8_t pw = pixelFormatBitWidth( tilemap->pixelFormat );	
		color565 sc = convert888to565( color );
		if (pw < 8) {
			// Single-channel grayscale pixel formats
			_stampG( (uint8_t*)tilemap->data, getAccessor5565( tilemap->pixelFormat ), pw, si, sw, sh, tilemap->tileWidth, alpha, di, dp, ds, sc, tilemap->transparentColor ); 
		}
		else {
			// Whole byte pixel formats
			pw >>= 3; // Bits to bytes
			if (pixelFormatHasAlpha(tilemap->pixelFormat)){
				_stampA( (uint8_t*)tilemap->data, getAccessor5565( tilemap->pixelFormat ), pw, si, sw, sh, tilemap->tileWidth, alpha, di, dp, ds, sc ); 
			}
			else{
				_stampT( (uint8_t*)tilemap->data, getAccessor5565( tilemap->pixelFormat ), pw, si, sw, sh, tilemap->tileWidth, alpha, di, dp, ds, sc, tilemap->transparentColor ); 
			}
		}
	}

	/**
	 * Blit a bitmap
	 * @param 	color 	The color of the stamp
	 * @param 	bitmap 	A bitmap containing the pixel data
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	alpha	The alpha value (0.0-1.0)
	 * @param	r		The rotation
	 */
	void GraphicsBitmapExtn16::stamp(
		color888 color,
		Bitmap bitmap,
		int16_t x,
		int16_t y,
		alpha alpha,
		BitmapRotation r
	){
		// Get source address (p) and step (s)
		uint32_t si = 0;					// Source index
		uint32_t di;						// Destination index 
		int16_t dp;							// Destination pixel step 
		int16_t ds;							// Destination stride 
		uint16_t sw = bitmap.width;			// Number of pixels to draw horizontally
		uint16_t sh = bitmap.height;		// Number of pixels to draw vertically

		// Calculate clipping values
		if (!_clip( r, x, y, sw, sh, si, di, dp, ds )) return;

		// Draw bitmap as stamp with full alpha or on/off transparency
		uint8_t pw = pixelFormatBitWidth( bitmap.pixelFormat );	
		color565 sc = convert888to565( color );
		if (pw < 8) {
			// Single-channel grayscale pixel formats
			_stampG( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, sc, bitmap.transparentColor ); 
		}
		else{
			// Whole byte pixel formats
			pw >>= 3; // Bits to bytes
			if (pixelFormatHasAlpha(bitmap.pixelFormat)){
				_stampA( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, sc ); 
			}
			else{
				_stampT( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, sc, bitmap.transparentColor ); 
			}
		}
		
	}

	/**
	 * Blit an area of a bitmap as a stamp
	 * @param 	color 	The color of the stamp
	 * @param 	bitmap 	A bitmap containing the pixel data
	 * @param 	rect 	The area of the bitmap to blit
	 * @param	dx		The destination x coord within the framebuffer
	 * @param	dy		The destination y coord within the framebuffer
	 * @param	alpha	The alpha value (0.0-1.0)
	 * @param	r		The rotation
	 */
	void GraphicsBitmapExtn16::stamp(
		color888 color,
		Bitmap bitmap,
		ClipRect* rect,
		int16_t x,
		int16_t y,
		alpha alpha,
		BitmapRotation r
	){
		// Calculate drawable area of bitmap
		ClipRect* sr = new ClipRect( 0,0, bitmap.width,bitmap.height );
		sr->clip( rect );

		// Set up src/dest values for drawing
		x += sr->x;
		y += sr->y;
		uint32_t si = y * bitmap.width + x;	// Source index
		uint32_t di;						// Destination index 
		int16_t dp;							// Destination pixel step 
		int16_t ds;							// Destination stride 
		uint16_t sw = sr->width;			// Number of pixels to draw horizontally
		uint16_t sh = sr->height;			// Number of pixels to draw vertically

		// Calculate clipping values
		if (!_clip( r, x, y, sw, sh, si, di, dp, ds )) return;

		// Draw bitmap as stamp with full alpha or on/off transparency
		uint8_t pw = pixelFormatBitWidth( bitmap.pixelFormat );	
		color565 sc = convert888to565( color );
		if (pw < 8) {
			// Single-channel grayscale pixel formats
			_stampG( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, sc, bitmap.transparentColor );
		}
		else {
			pw >>= 3; // Bits to bytes
			if (pixelFormatHasAlpha(bitmap.pixelFormat)){
				_stampA( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, sc ); 
			}
			else{
				_stampT( (uint8_t*)bitmap.data, getAccessor5565( bitmap.pixelFormat ), pw, si, sw, sh, bitmap.width, alpha, di, dp, ds, sc, bitmap.transparentColor ); 
			}
		}
	}

	boolean GraphicsBitmapExtn16::_clip( BitmapRotation r, int16_t& x, int16_t& y, uint16_t& sw, uint16_t& sh, uint32_t& si, uint32_t& di, int16_t& dp, int16_t& ds ){
		// Some early bounds checking
		if ((x>=_framebuffer->width) || (y>=_framebuffer->height)) return false;
		// Check if we need to flip x and y
		if ( (r == BitmapRotation::cw_90) || (r == BitmapRotation::cw_270) ) swap( sw, sh );
		// Some more early bounds checking
		if (((x+sw)<0) || ((y+sh)<0)) return false;

		// Calculate real values with clipping
		if (y<0){
			si -= (y * sw); 
			sh += y;
			y = 0;
		}
		if (x<0){
			si -= x;
			sw += x;
			x = 0;
		}
		int16_t d = (y+sh) - _framebuffer->height;
		if (d > 0){
			sh -= d;
		}
		d = (x+sw) - _framebuffer->width;
		if (d > 0){
			sw -= d;
		}

		switch (r) {
			case BitmapRotation::cw_90:{
				dp = _framebuffer->width;
				ds = 1;
				di = y * _framebuffer->width + x;
				break;
			}
			case BitmapRotation::cw_180: {
				dp = -1;
				ds = -_framebuffer->width;
				di = (y + sh - 1) * _framebuffer->width + (x + sw -1);
				break;
			}
			case BitmapRotation::cw_270:{
				dp = -_framebuffer->width;
				ds = -1;
				di = (y + sh - 1) * _framebuffer->width + (x + sw -1);
				break;
			}
			default: {
				dp = 1;
				ds = _framebuffer->width;
				di = y * _framebuffer->width + x;
				break;
			}
		}
		return true;
	}

	/**
	 * @brief Blit part of an image to the frambuffer using source alpha channel
	 * 
	 * @param data 		Pointer to the source data
	 * @param getPixel 	Accesor function for reading a single pixel from the source (@see Bitmap.cpp)
	 * @param pw 		Width of a pixel in the source, in bytes
	 * @param si 		Source index to start at (note: index, not address)
	 * @param sw 		The width of the area to blit
	 * @param sh 		The height of the area to blit
	 * @param ss 		The stride of the source (pixel width of source data)
	 * @param alpha 	The alpha to apply when blitting
	 * @param di 		The destination index to start at (note: index, not address)
	 */
	void GraphicsBitmapExtn16::_blitA( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds ){
		uint8_t pa; // Pixel alpha
		color565 c;	// Pixel color
		uint16_t sx = sw;
		uint32_t dli = di;
		si *= pw;	// Adjust width in pixels to width in bytes
		ss *= pw;
		uint32_t sli = si;
		alpha = alphaClamp(alpha);

		// Copy pixel by pixel
		while (sh>0){
			while (sx>0){
				// Grab pixel from tilemap, calculate alpha, and draw pixel
				getPixel( (uint8_t*)&data[si], c, pa );
				pa = (uint8_t)(pa * alpha);
				_framebuffer->data.data16[di] = alphaBlend5565( c, _framebuffer->data.data16[di], pa );
				// Destination
				di += dp;
				// Source
				si += pw;	// Move bytes to next pixel
				sx--;
			}
			// Destination
			dli += ds; di = dli;
			// Source
			sli += ss; si = sli;
			sx = sw;
			sh--;
		}
	}

	/**
	 * @brief Blit part of an image to the frambuffer using transparent colour (no source alpha channel)
	 * 
	 * @param data 		Pointer to the source data
	 * @param getPixel 	Accesor function for reading a single pixel from the source (@see Bitmap.cpp)
	 * @param pw 		Width of pixels in the source, in bytes
	 * @param si 		Source index to start at (note: index, not address)
	 * @param sw 		The width of the area to blit
	 * @param sh 		The height of the area to blit
	 * @param ss 		The stride of the source (pixel width of source data)
	 * @param alpha 	The alpha to apply when blitting
	 * @param di 		The destination index to start at (note: index, not address)
	 * @param tc		The transparent colour (pixels of this color treated as transparent)
	 */
	void GraphicsBitmapExtn16::_blitT( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 tc ){
		// Convert alpha to 0-31
		uint8_t a = alpha5bit(alpha);
		uint8_t pa; // pixel alpha. ignored for solid bitmaps
		color565 c;
		uint16_t sx = sw;
		uint32_t dli = di;
		si *= pw;	// Adjust width in pixels to width in bytes
		ss *= pw;
		uint32_t sli = si;

		// Copy pixel by pixel
		while (sh>0){
			while (sx>0){
				// Grab pixel from tilemap and draw it if it isn't the transparent color
				getPixel( (uint8_t*)&data[si], c, pa );
				if (c != tc){
					_framebuffer->data.data16[di] = alphaBlend5565( c, _framebuffer->data.data16[di], a );
				}
				// Destination
				di += dp;
				// Source
				si += pw;
				sx--;
			}
			// Destination
			dli += ds; di = dli;
			// Source
			sli += ss; si = sli;
			sx = sw;
			sh--;
		}
	}

	/**
	 * @brief Blit part of an image to the frambuffer using (T)ransparent colour, for (G)rayscale image format
	 * 
	 * @param data 		Pointer to the source data
	 * @param getPixel 	Accesor function for reading a single pixel from the source (@see Bitmap.cpp)
	 * @param pw 		Width of pixels in the source, in bits
	 * @param si 		Source index to start at (note: index, not address)
	 * @param sw 		The width of the area to blit
	 * @param sh 		The height of the area to blit
	 * @param ss 		The stride of the source (pixel width of source data)
	 * @param alpha 	The alpha to apply when blitting
	 * @param di 		The destination index to start at (note: index, not address)
	 * @param tc		The transparent colour (pixels of this color treated as transparent)
	 */
	void GraphicsBitmapExtn16::_blitG( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 tc ){
		// Convert alpha to 0-31
		uint8_t a = alpha5bit(alpha);
		color565 c;
		uint16_t sx = sw;
		uint32_t dli = di;
		si *= pw;	// Adjust width in pixels to width in bits
		ss *= pw;
		uint32_t sli = si;
		uint8_t bit_max = 8 / pw;
		uint8_t bit_index;
		uint32_t byte_index;
		uint8_t bit_div = pw >> 1;

		// Copy pixel by pixel
		while (sh>0){
			// Calculate initial byte and bit index for row
			byte_index = (si >> 3);
			bit_index = (si - (byte_index << 3)) >> bit_div;
			// Step all pixels in row
			while (sx>0){
				// Grab pixel from tilemap and draw it if it isn't the transparent color
				getPixel( (uint8_t*)&data[byte_index], c, bit_index );
				if ( c != tc ) {
					_framebuffer->data.data16[di] = alphaBlend5565( c, _framebuffer->data.data16[di], a );
				}
				// Destination - next pixel
				di += dp;
				// Source - next pixel
				bit_index++;
				if ( bit_index >= bit_max ) {
					bit_index = 0;
					byte_index++;
				}
				sx--;
			}
			// Destination - next row
			dli += ds; di = dli;
			// Source - next row
			sli += ss; si = sli;
			sx = sw;
			sh--;
		}
	}

	/**
	 * @brief Stamp part of an image to the frambuffer using source (A)lpha channel
	 * 
	 * @param data 		Pointer to the source data
	 * @param getPixel 	Accesor function for reading a single pixel from the source (@see Bitmap.cpp)
	 * @param pw 		Width of pixels in the source, in bytes
	 * @param si 		Source index to start at (note: index, not address)
	 * @param sw 		The width of the area to blit
	 * @param sh 		The height of the area to blit
	 * @param ss 		The stride of the source (pixel width of source data)
	 * @param alpha 	The alpha to apply when blitting
	 * @param di 		The destination index to start at (note: index, not address)
	 * @param sc		The stamp colour
	 */
	void GraphicsBitmapExtn16::_stampA( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 sc ){
		uint8_t pa; // Pixel alpha
		color565 c;	// Pixel color
		uint16_t sx = sw;
		uint32_t dli = di;
		si *= pw;	// Adjust width in pixels to width in bytes
		ss *= pw;
		uint32_t sli = si;
		alpha = alphaClamp(alpha);

		// Copy pixel by pixel
		while (sh>0){
			while (sx>0){
				// Grab pixel from tilemap, calculate alpha, and draw pixel as stamp
				getPixel( (uint8_t*)&data[si], c, pa );
				pa = (uint8_t)(pa * alpha);
				_framebuffer->data.data16[di] = alphaBlend5565( sc, _framebuffer->data.data16[di], pa );
				// Destination
				di += dp;
				// Source
				si += pw;
				sx--;
			}
			// Destination
			dli += ds; di = dli;
			// Source
			sli += ss; si = sli;
			sx = sw;
			sh--;
		}
	}

	/**
	 * @brief Stamp part of an image to the frambuffer using on/off (T)ransparency
	 * 
	 * @param data 		Pointer to the source data
	 * @param getPixel 	Accesor function for reading a single pixel from the source (@see Bitmap.cpp)
	 * @param pw 		Width of pixels in the source, in bytes
	 * @param si 		Source index to start at (note: index, not address)
	 * @param sw 		The width of the area to blit
	 * @param sh 		The height of the area to blit
	 * @param ss 		The stride of the source (pixel width of source data)
	 * @param alpha 	The alpha to apply when blitting
	 * @param di 		The destination index to start at (note: index, not address)
	 * @param sc		The stamp colour
	 * @param tc		Transparent colour (pixels of this color treated as transparent)
	 */
	void GraphicsBitmapExtn16::_stampT( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 sc, color565 tc ){
		// Convert alpha to 0-31
		uint8_t pa; // pixel alpha. ignored for solid bitmaps
		color565 c;
		uint16_t sx = sw;
		uint32_t dli = di;
		si *= pw;	// Adjust width in pixels to width in bytes
		ss *= pw;
		uint32_t sli = si;

		// Copy pixel by pixel
		while (sh>0){
			while (sx>0){
				// Grab pixel from tilemap and draw stamp pixel if it isn't the transparent color
				getPixel( (uint8_t*)&data[si], c, pa );
				if (c != tc){
					// Convert color to grayscale, then use single channel (5-bit width) as alpha
					_framebuffer->data.data16[di] = alphaBlend5565( sc, _framebuffer->data.data16[di], (uint8_t)(convert565to5gs(c) * alpha) );
				}
				// Destination
				di += dp;
				// Source
				si += pw;
				sx--;
			}
			// Destination
			dli += ds; di = dli;
			// Source
			sli += ss; si = sli;
			sx = sw;
			sh--;
		}
	}

	/**
	 * @brief Stamp part of a single-channel (G)rayscale image to the frambuffer using on/off transparency
	 * 
	 * @param data 		Pointer to the source data
	 * @param getPixel 	Accesor function for reading a single pixel from the source (@see Bitmap.cpp)
	 * @param pw 		Width of pixels in the source, in bits
	 * @param si 		Source index to start at (note: index, not address)
	 * @param sw 		The width of the area to blit
	 * @param sh 		The height of the area to blit
	 * @param ss 		The stride of the source (pixel width of source data)
	 * @param alpha 	The alpha to apply when blitting
	 * @param di 		The destination index to start at (note: index, not address)
	 * @param dp		The destination pixel step
	 * @param ds		The destination stride
	 * @param sc		The stamp colour
	 * @param tc		Transparent colour (pixels of this color treated as transparent)
	 */
	void GraphicsBitmapExtn16::_stampG( uint8_t* data, access5565 getPixel, uint8_t pw, uint32_t si, uint16_t sw, uint16_t sh, uint16_t ss, alpha alpha, uint32_t di, int16_t dp, int16_t ds, color565 sc, color565 tc ){
		// Convert alpha to 0-31
		color565 c;
		uint16_t sx = sw;
		uint32_t dli = di;
		si *= pw;	// Adjust width in pixels to width in bytes
		ss *= pw;
		uint32_t sli = si;
		uint8_t bit_max = 8 / pw;
		uint8_t bit_index;
		uint32_t byte_index;
		uint8_t bit_div = pw >> 1;
		alpha = alphaClamp(alpha);

		// Copy pixel by pixel
		while (sh>0){
			// Calculate initial byte and bit index for row
			byte_index = (si >> 3);
			bit_index = (si - (byte_index << 3)) >> bit_div;
			// Step all pixels in row
			while (sx>0){
				// Grab pixel from tilemap and draw stamp pixel if it isn't the transparent color
				getPixel( (uint8_t*)&data[byte_index], c, bit_index );
				if (c != tc){
					// Use blue channel as alpha (could be any of the channels - it's grayscale!)
					_framebuffer->data.data16[di] = alphaBlend5565( sc, _framebuffer->data.data16[di], (uint8_t)( (c & 0b11111) * alpha) );
				}
				// Destination - next pixel
				di += dp;
				// Source - next pixel
				bit_index++;
				if ( bit_index >= bit_max ) {
					bit_index = 0;
					byte_index++;
				}
				sx--;
			}
			// Destination - next row
			dli += ds; di = dli;
			// Source - next row
			sli += ss; si = sli;
			sx = sw;
			sh--;
		}
	}

	
}