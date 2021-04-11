/**
 * Graphics library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "GraphicsTextExtn.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Called by the Graphics object internally. Override base method.
	 * @param	graphics	The graphics object passes itself as a reference
	 **/
	void GraphicsTextExtn::init( FrameBuffer* framebuffer ){
		GraphicsExtension::init( framebuffer );
		_textarea = new TextArea();
		_clip = new ClipRect();
		_color = framebuffer->prepareColor( RGB888_White, 1 );
		resetTextArea();
		resetClipArea();
	}

	void GraphicsTextExtn::setFont( const packedbdf_t *f ){
		_font = f;
		_fontbpp = 1;
		if (_font->version==23){
			_fontbpp = (_font->reserved & 0b000011)+1;
			_fontbppindex = (_fontbpp >> 2)+1;
			_fontbppmask = (1 << (_fontbppindex+1))-1;
			_fontppb = 8/_fontbpp;
			_fontalphamx = 31/((1<<_fontbpp)-1);
			_fontdeltaoffset = 3 + _font->bits_width + _font->bits_height + _font->bits_xoffset + _font->bits_yoffset;
			_fontspacewidth = _getCharWidth(32);
//Serial.printf("Font bpp = %d, ppb = %d, index = %d, mask = %d, amx = %f\n", _fontbpp,_fontppb,_fontbppindex,_fontbppmask,_fontalphamx);
		}
	}

	void GraphicsTextExtn::setColor( color888 color ){
		_color->setColor( color );
	}

	/**
	 * Set active font alpha
	 * @param alpha  	Alpha 0.0 - 1.0
	 */
	void GraphicsTextExtn::setAlpha( alpha alpha ){
		_color->setAlpha( alpha );
	}

	void GraphicsTextExtn::setTextArea( int16_t x, int16_t y, uint16_t w, uint16_t h, TextAlign align ){
		_textarea->setPosAndSize( x, y, w, h );
		_textarea->align = align;
		_textarea->lineSpace = 1.5;
		_textarea->tabWidth = 8;
		//_textarea->th = 0;
		resetTextAreaCursor();
		setClipArea( x, y, w, h );
	}

	void GraphicsTextExtn::resetTextAreaCursor(){
		_textarea->cx = _textarea->x;
		_textarea->cy = _textarea->y;
		if (_font) _textarea->cy += _font->cap_height;
	}

	void GraphicsTextExtn::resetTextArea(){
		_textarea->setPosAndSize( 0, 0, _framebuffer->width, _framebuffer->height );
		_textarea->align = TextAlign::left;
		resetTextAreaCursor();
	}

	void GraphicsTextExtn::setClipArea( int16_t x, int16_t y, uint16_t w, uint16_t h ){
		_clip->setPosAndSize( x, y, w, h );
	}

	void GraphicsTextExtn::resetClipArea(){
		_clip->setPosAndSize( 0, 0, _framebuffer->width, _framebuffer->height );
	}

	uint16_t GraphicsTextExtn::print( const char* text ){
		if (!_font) return _cx;

		char c;
		char* p = (char*)text;
		while(true){
			c = *p;
			if (c == '\0') break;
			_drawFontChar(c);
			p++;
		}
		return _cx;
	}

	uint16_t GraphicsTextExtn::print( int16_t x, int16_t y, const char* text ){
		_cx = x;
		_cy = y;
		return print( text );
	}

	/**
	 * XXX: Justify
	 */
	void GraphicsTextExtn::printTextArea( const char* text ){
		if (!_font) return;
		if (_textarea->cy > _textarea->y2) return;
//Serial.printf("printTextArea %s\n", text);
//Serial.printf("  Area %d-%d,%d-%d\n", _textarea->x,_textarea->x2,_textarea->y,_textarea->y2);
//Serial.printf("  Clip %d-%d,%d-%d\n", _clip->x,_clip->x2,_clip->y,_clip->y2);

		char c;
		char* p = (char*)text;
		char* pw;
		uint32_t rw;	// remaining width
		uint32_t sw;	// sentence width
		while(true){
			// Grab next sentence that will fit
			rw = (_textarea->x2 - _textarea->cx);
			sw = _getSentence( p, rw, pw );
//Serial.printf("  sentence width %d (to fit %d), num chars %d\n", sw, rw, (pw-p+1));
			// Offset based on alignment
			if (_textarea->align == TextAlign::right) _textarea->cx += rw-sw;
			else if (_textarea->align == TextAlign::center) _textarea->cx += ((rw-sw)>>1);
			// Print the sentence
			while (p <= pw){
				c = *p;
				_textarea->cx = _drawFontCharAt( c, _textarea->cx, _textarea->cy );
//Serial.printf("    print char %c at %d,%d\n", c, _textarea->cx, _textarea->cy);
				p++;
			}
			// If the next character is null, exit
			c = *p;
			if (c == '\0'){
//Serial.print("printTextArea END(1)");
				break;
			}
			// Ignore whitespace
			p = _ignoreWhitespace( p );
			if (p==0){
//Serial.print("printTextArea END(2)");
				return; // End of string
			}
			// Wrap to next line
			_textarea->cy += (int16_t)(_font->line_space * _textarea->lineSpace);
			_textarea->cx = _textarea->x;
			// Exit when we are drawing below the text area
			if ((_textarea->cy - _font->cap_height) > _textarea->y2) break;
		}
	}

	uint32_t GraphicsTextExtn::_fetchbit(const uint8_t *p, uint32_t index){
		if (p[index >> 3] & (1 << (7 - (index & 7)))) return 1;
		return 0;
	}

	uint32_t GraphicsTextExtn::_fetchbits_unsigned(const uint8_t *p, uint32_t index, uint32_t required){
		uint32_t val = 0;
		do {
			uint8_t b = p[index >> 3];
			uint32_t avail = 8 - (index & 7);
			if (avail <= required) {
				val <<= avail;
				val |= b & ((1 << avail) - 1);
				index += avail;
				required -= avail;
			} else {
				b >>= avail - required;
				val <<= required;
				val |= b & ((1 << required) - 1);
				break;
			}
		} while (required);
		return val;
	}

	uint32_t GraphicsTextExtn::_fetchbits_signed(const uint8_t *p, uint32_t index, uint32_t required){
		uint32_t val = _fetchbits_unsigned(p, index, required);
		if (val & (1 << (required - 1))) {
			return (int32_t)val - (1 << required);
		}
		return (int32_t)val;
	}

	uint32_t GraphicsTextExtn::_fetchpixel(const uint8_t *p, uint32_t index, uint32_t x){
		// The byte
		uint8_t b = p[index >> 3];
		// Shift to LSB position
		uint8_t s = ((_fontppb-(x % _fontppb)-1)*_fontbpp);
//Serial.printf("[%d>>%d (%d)] ",b,s,x);
		b = b >> s;
		// Mask and return
		return b & _fontbppmask;
	}

	/**
	 * Draw single char at current cursor position
	 * @param c  	The char to draw
	 */
	void GraphicsTextExtn::_drawFontChar( uint16_t c ){
		_cx = _drawFontCharAt( c, _cx, _cy );
	}
	
	/**
	 * Draw a single char
	 * Adapted from ILI9341_t3DMA::drawFontChar
	 * Changes include moving wrapping to higher level, changing y to baseline (instead of top
	 * of char), supporting alpha, and clipping to a rect
	 * 
	 * @param c 	The char to draw
	 */
	int32_t GraphicsTextExtn::_drawFontCharAt( uint16_t c, int16_t cx, int16_t cy ){
		uint32_t bitoffset;
		const uint8_t *data;

		// Check character is supported by the _font
		if ((c >= _font->index1_first) && (c <= _font->index1_last)) {
			bitoffset = c - _font->index1_first;
			bitoffset *= _font->bits_index;
		}
		else if ((c >= _font->index2_first) && (c <= _font->index2_last)) {
			bitoffset = c - _font->index2_first + _font->index1_last - _font->index1_first + 1;
			bitoffset *= _font->bits_index;
		}
		else if (_font->unicode) {
			return cx; // XXX: implement sparse unicode
		}
		else {
			return cx; // Unsupported char
		}

		// Char data
		uint32_t goffset = _fetchbits_unsigned( _font->index, bitoffset, _font->bits_index );
		data = _font->data + goffset;
//Serial.printf("  goffset =	%d\n", goffset);

		uint32_t encoding = _fetchbits_unsigned(data, 0, 3);
		if (encoding != 0) return cx;
		int16_t width = _fetchbits_unsigned(data, 3, _font->bits_width);
		bitoffset = _font->bits_width + 3;
		int16_t height = _fetchbits_unsigned(data, bitoffset, _font->bits_height);
		bitoffset += _font->bits_height;

		int32_t xoffset = _fetchbits_signed(data, bitoffset, _font->bits_xoffset);
		bitoffset += _font->bits_xoffset;
		int32_t yoffset = _fetchbits_signed(data, bitoffset, _font->bits_yoffset);
		bitoffset += _font->bits_yoffset;
//Serial.printf("  offset = %d,%d\n", xoffset, yoffset);

		uint32_t delta = _fetchbits_unsigned(data, bitoffset, _font->bits_delta);
		bitoffset += _font->bits_delta;
//Serial.printf("  delta = %d\n", delta);

		// horizontal start
		int32_t origin_x = cx + xoffset;
		cx += delta;

		// Whole char outside of clip area horizontally
		if ((origin_x>_clip->x2) || (cx<=_clip->x)) return cx;

		// vertically, the top and/or bottom can be clipped
		int32_t origin_y = cy - height - yoffset;

		// Whole char outside of clip area vertically
		if ((origin_y>_clip->y2) || ((origin_y+_font->line_space)<_clip->y)) return cx;

		// TODO: compute top skip and number of lines
		int32_t linecount = height;
		int16_t y = origin_y;

		// 16-bit
		if (_framebuffer->pixelFormat == PF_565){

			// Anti-aliased font handled differently
			if (_fontbpp>1){
				bitoffset = ((bitoffset + 7) & (-8)); // byte-boundary
				uint32_t xp = 0;
				while (linecount) {
					if (_clip->containsY(y)){ // clip y
						int16_t x = 0;
						while(x<width) {
							// One pixel at a time
							if (_clip->containsX( origin_x + x )){
								uint8_t alpha = _fetchpixel(data, bitoffset, xp);
								_drawFontPixel(alpha, origin_x + x, y);
							}
							bitoffset += _fontbpp;
							x++;
							xp++;
						}
					}
					y++;
					linecount--;
				}
			}

			// original, non-antialias
			else{
				while (linecount) {
					uint32_t b = _fetchbit(data, bitoffset++);
					if (b == 0) {
						int16_t x = 0;
						do {
							uint32_t xsize = width - x;
							if (xsize > 32) xsize = 32;
							uint32_t bits = _fetchbits_unsigned(data, bitoffset, xsize);
							_drawFontBits(bits, xsize, origin_x + x, y, 1);
							bitoffset += xsize;
							x += xsize;
						} while (x < width);
						y++;
						linecount--;
					} else {
						uint32_t n = _fetchbits_unsigned(data, bitoffset, 3) + 2;
						bitoffset += 3;
						int16_t x = 0;
						do {
							uint32_t xsize = width - x;
							if (xsize > 32) xsize = 32;
							uint32_t bits = _fetchbits_unsigned(data, bitoffset, xsize);
							_drawFontBits(bits, xsize, origin_x + x, y, n);
							bitoffset += xsize;
							x += xsize;
						} while (x < width);
						y += n;
						linecount -= n;
					}
				}
			}

		}
//Serial.println("");

		return cx;
	}

	/**
	 * Draw a single row of pixels for a glyph. 1bpp
	 * @param bits    The bit data
	 * @param numbits The number of bits (width)
	 * @param x       X coord
	 * @param y       Y coord
	 * @param repeat  Number of times to repeat the line vertically
	 */
	void GraphicsTextExtn::_drawFontBits( uint32_t bits, uint32_t numbits, int16_t x, int16_t y, uint32_t repeat ){
		if (bits == 0) return;
		do {
			int16_t x1 = x;
			uint32_t n = numbits;
			do {
				n--;
				//if (bits & (1 << n)) _pixel( x1, y, _color, _clip );
				if (bits & (1 << n)){
					if (_clip->contains(x1,y)) _framebuffer->writePixel( _color->c565, x1, y );
				}
				x1++;
			} while (n > 0);
			y++;
			repeat--;
		} while (repeat);
	}

	void GraphicsTextExtn::_drawFontPixel( uint8_t alpha, int16_t x, int16_t y ){
		// Adjust alpha based on the number of alpha levels supported by the font (based on bpp)
		// XXX: Combine _fontalphamx and _alpha ahead of time
		alpha = (uint8_t)(alpha * _fontalphamx * _color->a);
//Serial.printf("%d ",alpha);
		uint32_t i = _framebuffer->index(x,y);
		_framebuffer->data.data16[i] = alphaBlendPrepared5565( _color->cPre, _framebuffer->data.data16[i], alpha );
	}

	/**
	 * Get the width of a single character in the current font
	 * @param  c  	The character code (encoding)
	 * @return   The width in pixels (0 if not found in the font)
	 */
	uint32_t GraphicsTextExtn::_getCharWidth( uint16_t c ){
		uint32_t bitoffset;
		const uint8_t *data;

		// Check character is supported by the _font
		if ((c >= _font->index1_first) && (c <= _font->index1_last)) {
			bitoffset = c - _font->index1_first;
			bitoffset *= _font->bits_index;
		}
		else if ((c >= _font->index2_first) && (c <= _font->index2_last)) {
			bitoffset = c - _font->index2_first + _font->index1_last - _font->index1_first + 1;
			bitoffset *= _font->bits_index;
		}
		else if (_font->unicode) {
			return 0;
		}
		else {
			return 0;
		}

		// Char data
		uint32_t goffset = _fetchbits_unsigned( _font->index, bitoffset, _font->bits_index );
		data = _font->data + goffset;
		return _fetchbits_unsigned(data, _fontdeltaoffset, _font->bits_delta);
	}

	/**
	 * Step through string and calculate width until a space or non-printable
	 * character is encountered. If the string starts with whitespace, this is
	 * included in the word.
	 * @param  c  The start of the string to calculate from
	 * @param  cw (out) The last character in the word
	 * @return    The width of all characters from c to cw
	 */
	uint32_t GraphicsTextExtn::_getWordWidth( char* c, char* &cw ){
		uint32_t w = 0;
		boolean started = false;
		char b;
		cw = c;
		while(true){
			b = *c;
			if (b<' ') break;
			else if (b>'~') break;
			else if (b==' '){
				if (started) break;
			}
			else started = true;
			cw = c;
			w += _getCharWidth( b );
			c++;
		}
		return w;
	}

	/**
	 * Get the pixel width of the entire string (to null)
	 * @param  c 	The string
	 * @return   Pixel width of string
	 */
	uint32_t GraphicsTextExtn::_getTextWidth( char* c ){
		uint32_t w = 0;
		char b;

		while(true){
			b = *c;
			if (b<='\0') break;
			w += _getCharWidth( b );
			c++;
		}
		return w;
	}

	/**
	 * Check how many words from a string will fit into the given width (w), with word
	 * wrap. The last character in the sentence is stored in cw, and the pixel width of the
	 * sentence is returned.
	 * last character 
	 * @param  c  The string
	 * @param  w  The width to fit in
	 * @param  cw (out) the last character in the sentence
	 * @return    The width of the sentence in pixels
	 */
	uint32_t GraphicsTextExtn::_getSentence( char* c, uint32_t w, char* &cw ){
//Serial.print("    _getSentence");
		uint32_t sw = 0; 	// sentence width
		uint32_t ww;		// word width
		char* sp;			// sentence pointer
		char* p = c;		// string pointer
		char b;				// byte
		cw = c;
		while (true){
			// Get width of next word
			ww = _getWordWidth( p, sp );
			// Check if word fits
			if ((sw + ww) <= w){
//Serial.printf("    %.*s = %d\n", (sp-p+1), p, ww);
				// Fits. Save this word
				sw += ww;
				cw = sp;
				// Move to next character
				p = sp+1;
				// See if next character is end of string or newline
				b = *p;
				if ((b=='\0') || (b=='\n')) break;
				// Carry on to next word
			}
			// Word doesn't fit
			else break;
		}
		return sw;
	}

	/**
	 * Step through a string and return pointer to the first character that is
	 * not a whitespace or unprintable character (i.e. a character not supported
	 * by the current font).
	 * @param  p 	The string to step through
	 * @return   The pointer to the first printable character, or 0 if null is found
	 */
	char* GraphicsTextExtn::_ignoreWhitespace( char* p ){
//Serial.print("    _ignoreWhitespace");
		char c;
		while (true){
			c = *p;
			if (c=='\0') return 0; // end of string
			if (c=='\n') return p; // newline
			if (c=='\t') return p; // tab
			if (c>32){ // After space
				// Check character is supported by the _font
				if ((c >= _font->index1_first) && (c <= _font->index1_last)) {
					return p;
				}
				else if ((c >= _font->index2_first) && (c <= _font->index2_last)) {
					return p;
				}
			}
			p++;
		}
	}

	/**
	 * Move the cursor from its current location
	 * @param dx  	Amount to move in X direction
	 * @param dy 	Amount to move in Y direction
	 */
	void GraphicsTextExtn::moveCursor( int16_t dx, int16_t dy ){
		_cx += dx;
		_cy += dy;
	}

	/**
	 * Move the cursor to a new location
	 * @param x  	New X
	 * @param y 	New Y
	 */
	void GraphicsTextExtn::moveCursorTo( int16_t x, int16_t y ){
		_cx = x;
		_cy = y;
	}



}