/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Text.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	DisplayObject* Text::pool = 0;

	/**
	 * Pool getter
	 */
	DisplayObject** Text::_getPool(){
		return &Text::pool;
	}

	/**
	 * Create a new object or take one from the pool
	 * @return The new or recycled object
	 */
	Text* Text::Create(){
		return (Text*)DisplayObject::Create<Text>();
	}

	Text* Text::Create( packedbdf_t* font ) {
		Text* object = (Text*)DisplayObject::Create<Text>();
		object->font( font );
		return object;
	}

	/**
	 * Reset the object back to default settings
	 */
	void Text::reset(){
		DisplayObject::reset();
	}

	/**
	 * @brief Set the font that this text uses
	 * @param font 	The font to use
	 */
	void Text::font( packedbdf_t* font ) {
		if (_font == font) return;
		
		_font = font;
		_fontbpp = 1;
		if (_font->version==23){
			_fontbpp = (_font->reserved & 0b000011)+1;
			_fontbppindex = (_fontbpp >> 2)+1;
			_fontbppmask = (1 << (_fontbppindex+1))-1;
			_fontppb = 8/_fontbpp;
			_fontalphamx = 31.0/((1<<_fontbpp)-1);
			_fontdeltaoffset = 3 + _font->bits_width + _font->bits_height + _font->bits_xoffset + _font->bits_yoffset;
			_fontspacewidth = _getCharWidth(32);
		}

		dirty();
	}

	/**
	 * @brief Set the text color
	 * @param c The text color
	 */
	void Text::color( color888 c ) {
		if (_color == c) return;
		_color = c;
		dirty();
	}

	/**
	 * @brief Set the line height of the text
	 * @param lh 	The line height. Usually between 1 - 1.5
	 */
	void Text::lineHeight( float lh ) {
		if (_lineHeight == lh) return;
		_lineHeight = lh;
		dirty();
	}

	/**
	 * @brief Set the text alignment
	 * @param align The alignment
	 */
	void Text::align( TextAlign a ) {
		if (_align == a) return;
		_align = a;
		dirty();
	}

	/**
	 * @brief Set the text string
	 * @param t The text string
	 */
	void Text::text( char* t ) {
		_text = t;
		dirty();
		size( _getTextWidth(), _font->cap_height );
	}

	/**
	 * Set the position at which to read the next pixel
	 * @param x The global x coordinate
	 * @param y The global y coordinate
	 */
	void Text::readPosition( int16_t gx, int16_t gy ){
		DisplayObject::readPosition( gx, gy );
		_prepareCharAt( gx );
		if (_ci >= 0) {
			_cdo = max( 0, (ry - _cy) * _cw) + _cb;
		}
	}

	/**
	 * Read a pixel from the object and advance position
	 * @param c (out) color
	 * @param a (out) alpha
	 */
	void Text::readPixel( color888 &c, float &a ) {
		a = 0;
		if (_ci >= 0) {
			// If we are above the letter, skip
			if (ry >= _cy) {
				// If we are below the letter, skip
				if (ry < _cy2) {
					// If we are before the letter, skip
					if (rx >= _cx) {
						// Read pixel
						uint8_t pv = _fetchpixel(_data, _bitoffset, _cdo);
						pv = (uint8_t)(pv * _fontalphamx);
						a = pv/31.0;
						c = _color;
						_cdo++;
>>>Fix this cdo thing. Eitehr that is wrong, or the bitoffset into the data is wrong?
						_bitoffset += _fontbpp;
					}
				}
			}
			// Advance x
			_cb++;
			if (_cb >= _cw) _nextChar();
		}
	}

	uint32_t Text::_fetchbit( const uint8_t *p, uint32_t index ){
		if (p[index >> 3] & (1 << (7 - (index & 7)))) return 1;
		return 0;
	}

	uint32_t Text::_fetchbits_unsigned( const uint8_t *p, uint32_t index, uint32_t required ){
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

	int32_t Text::_fetchbits_signed(const uint8_t *p, uint32_t index, uint32_t required){
		uint32_t val = _fetchbits_unsigned(p, index, required);
		if (val & (1 << (required - 1))) {
			return (int32_t)val - (1 << required);
		}
		return (int32_t)val;
	}

	uint32_t Text::_fetchpixel(const uint8_t *p, uint32_t index, uint32_t x){
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
	 * @brief Get the index into the string at the specified x position
	 * Assumes the string starts at x=0 and is on one line only
	 * @param x The x coordinate
	 * @return uint32_t The index into the string. -1 means char not found
	 */
	void Text::_prepareCharAt( int16_t x ){
Serial.printf("Text::_prepareCharAt %d\n  ", x);
		int32_t w = 0;
		char b;
		_ci = 0;
		_cb = 0;
		while(true){
			b = _text[_ci];
			if (b=='\0'){
				_cb = 0;
				_ci = -1;
				break; // End of string
			}
			_cb = x - w;
			w += _getCharWidth( b );
			if (w > x) break;
			_ci++;
		}
		if (_ci >= 0) _prepareChar();
	}

	/**
	 * @brief Move along to the next char
	 */
	void Text::_nextChar() {
		_ci++;
		_cb = 0;
		if (_text[_ci]=='\0'){
			_ci = -1;
			return;
		}
		if (_ci >= 0) _prepareChar();
	}

	/**
	 * @brief Prepare internal properties for drawing the char at the specified index
	 * @param i The index into the string
	 */
	void Text::_prepareChar() {
		char c = _text[_ci];
Serial.printf("_prepareChar %c\n", c);

		// Check character is supported by the _font
		if ((c >= _font->index1_first) && (c <= _font->index1_last)) {
			_bitoffset = c - _font->index1_first;
			_bitoffset *= _font->bits_index;
		}
		else if ((c >= _font->index2_first) && (c <= _font->index2_last)) {
			_bitoffset = c - _font->index2_first + _font->index1_last - _font->index1_first + 1;
			_bitoffset *= _font->bits_index;
		}
		else if (_font->unicode) {
Serial.println("    WARNING: Unsupported unicode character");
			_nextChar();
			return;
		}
		else {
Serial.println("    WARNING: Unsupported character");
			_nextChar();
			return;
		}

		// Char data
		uint32_t goffset = _fetchbits_unsigned( _font->index, _bitoffset, _font->bits_index );
		_data = _font->data + goffset;

		uint32_t encoding = _fetchbits_unsigned(_data, 0, 3);
		if (encoding != 0) {
			_nextChar();
			return;
		}

		int16_t width = _fetchbits_unsigned(_data, 3, _font->bits_width);
		_bitoffset = _font->bits_width + 3;
		int16_t height = _fetchbits_unsigned(_data, _bitoffset, _font->bits_height);
		_bitoffset += _font->bits_height;

		int32_t xoffset = _fetchbits_signed(_data, _bitoffset, _font->bits_xoffset);
		_bitoffset += _font->bits_xoffset;
		int32_t yoffset = _fetchbits_signed(_data, _bitoffset, _font->bits_yoffset);
		_bitoffset += _font->bits_yoffset;

		_cw = _fetchbits_unsigned(_data, _bitoffset, _font->bits_delta);
		_bitoffset += _font->bits_delta;

		// horizontal start
		_cx = xoffset;

		// vertically, the top and/or bottom can be clipped
		_cy = _font->cap_height - height - yoffset;
		_cy2 = _cy + height;

		// For antialiased fonts, bitoffset is rounded to byte boundary
		if (_fontbpp>1) _bitoffset = ((_bitoffset + 7) & (-8));
	}

	/**
	 * Get the width of a single character in the current font
	 * @param  c  	The character code (encoding)
	 * @return   The width in pixels (0 if not found in the font)
	 */
	int32_t Text::_getCharWidth( uint16_t c ){
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
	int32_t Text::_getWordWidth( char* c, char* &cw ){
		int32_t w = 0;
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
	 * @return   Pixel width of string
	 */
	int32_t Text::_getTextWidth(){
		uint32_t i = 0;
		int32_t w = 0;
		char b;

		while(true){
			b = _text[i];
			if (b<='\0') break;
			w += _getCharWidth( b );
			i++;
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
	int32_t Text::_getSentence( char* c, uint32_t w, char* &cw ){
//Serial.print("    _getSentence");
		int32_t sw = 0; 	// sentence width
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
	char* Text::_ignoreWhitespace( char* p ){
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
	
} // namespace