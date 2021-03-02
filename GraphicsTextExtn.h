/**
 * Graphics library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 *
 * MIT LICENCE
 * -----------
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */	
 
#pragma once
#ifndef _MAC_GRAPHICSTEXTEXTNH_
#define _MAC_GRAPHICSTEXTEXTNH_ 1

#include "GraphicsExtension.h"
#include "PackedBDF.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Text
	 */
	typedef enum textAlignE{
		ALIGN_LEFT		= 0,
		ALIGN_RIGHT		= 1,
		ALIGN_CENTER	= 2
	} textAlign;
	


	class TextArea: public ClipRect {
		public:
			int16_t cx;			// X position of cursor
			int16_t cy;			// Y position of cursor
			textAlign align;	// Horizontal text alignment
			float lineSpace;	// Line space for new lines. Default 1.5 
			uint8_t tabWidth;	// Number of spaces for a tab. Default 8
			/*
			char* text;			// The text in the area
			uint32_t scroll;	// The number of pixels to scroll by
			uint32_t th;		// Text height - height of the text contained in area
			color bg;			// Clear color for text area
			*/
	};

	/**
	 * Adds text rendering functions to the Graphics library.
	 * 
	 * Drawing functions support the original ILI9341_t3_font_t implementation of
	 * the BDF font format (by Paul Stoffregen and others), but also supports the
	 * greymap extension (anti-aliasing) for BDF fonts in the same data format.
	 * mac calls these fonts 'packed BDF' fonts using the type alias packedbdf_t
	 * Officialy this extension is v2.3 of the BDF format. The font.version flag
	 * will be decimal '23' (uint8_t), and the lower 2 bits of font.reserved are
	 * the number of bits per pixel (bpp). 0=1bpp,1=2bpp,2=4bpp,3=8bpp
	 *
	 * NOTE: mac will support both the orignal 1bpp ILI9341_t3_font_t and also
	 * the 2,4 and 8bpp anti-aliased formats. However, existing libraries that
	 * use ILI9341_t3_font_t will NOT WORK with the more than 1bpp (they will
	 * not support anti-alias).
	 *
	 * BDF fonts can be generated using FontForge like this:
	 * 
	 * Reduce font character set:
	 * 1) Load the font TTF (or other)
	 * 2) Select the characters you want to keep
	 * 3) Edit > Select > Invert selection
	 * 4) Encoding > Detach and remove Glyphs
	 * 5) Encoding > Compact
	 * 
	 * Generate the bitmap sets that you want:
	 * 1) Element > Bitmap strikes available...
	 * 2) Enter the size of the font and the bpp like this: 10@2 (this is 10pt @2bpp) or 12@4 (this is 12pt @4bpp) etc. Example: I have been using 10@2,11@2,12@2,14@2,16@2 in the "Pixel Sizes" field
	 * 3) Click [OK] to generate the bitmap strikes of each glyph in various sizes
	 * 4) Use "View > ## pixel bitmap" options to view the bitmap strikes that were generated
	 * 5) You can click on each one and edit them individually if they aren't perfect
	 * 6) File > Generate Fonts. Select "No outline font" and "BDF", then hit [Generate]. A BDF file for each size will be generated.
	 *
	 * Convert the BDF files to packedbdf_t header files:
	 * 1) Copy the BDF files to the 'resources' folder within mac
	 * 2) Run the python script "scripts>build_resources.py"
	 * 3) That's it. The font files are now in the 'generated' folder
	 * 
	 */
	class GraphicsTextExtn: public GraphicsExtension {
		public:
			/**
			 * Called by the Graphics object internally. Override base method.
			 * @param	framebuffer		The framebuffer for rendering
			 **/
			void init( FrameBuffer* framebuffer );

			/**
			 * Set the active font to a mac::packedbdf_t
			 * @param f 	The font to use
			 */
			void setFont( const packedbdf_t *f );

			/**
			 * Set active font color
			 * @param color  	RGB888 24-bit color
			 */
			void setColor( color888 color );

			/**
			 * Set active font alpha
			 * @param alpha  	Alpha 0.0 - 1.0
			 */
			void setAlpha( alpha alpha = 1 );

			/**
			 * Set the text area for drawing text to. The cursor will start at the top
			 * left of the area, with the character within the area.
			 * @param x     X coord of top-left corner of area
			 * @param y     Y coord of top-left corner of area
			 * @param w     Width of area
			 * @param h     Height of area
			 * @param align Horizontal text align within area
			 */
			void setTextArea( int16_t x, int16_t y, uint16_t w, uint16_t h, textAlign align = ALIGN_LEFT );

			/**
			 * Reset the cursor back to the origin of the current text area
			 */
			void resetTextAreaCursor();

			/**
			 * Reset the text area to the entire display
			 */
			void resetTextArea();

			/**
			 * Set the clip area for drawing text to. The pixels outside the clip area will
			 * not be rendered.
			 * @param x     X coord of top-left corner of area
			 * @param y     Y coord of top-left corner of area
			 * @param w     Width of area
			 * @param h     Height of area
			 */
			void setClipArea( int16_t x, int16_t y, uint16_t w, uint16_t h );

			/**
			 * Reset the clip area to the entire display
			 */
			void resetClipArea();

			/**
			 * Render a line of text at the specified coords. Will not conform to the text
			 * area, and will not wrap.
			 * @see printTextArea
			 * @param  x    X coord
			 * @param  y    Y coord
			 * @param  text The text to render
			 * @return      The x coord of the next character after the text
			 */
			uint16_t print( int16_t x, int16_t y, const char* text );

			/**
			 * Render a line of text at the current cursor position. Will not conform to
			 * the text area, and will not wrap.
			 * @see printTextArea
			 * @param  text The text to render
			 * @return      The x coord of the next character after the text
			 */
			uint16_t print( const char* text );

			/**
			 * Move the cursor from its current location. Used by print. Does not affect
			 * text area.
			 * @param dx  	Amount to move in X direction
			 * @param dy 	Amount to move in Y direction
			 */
			void moveCursor( int16_t dx, int16_t dy );

			/**
			 * Move the cursor to a new location. Does not affect text area.
			 * @param x  	New X coord
			 * @param y 	New Y coord
			 */
			void moveCursorTo( int16_t x, int16_t y );

			/**
			 * Render a line of text at the current cursor position within the text
			 * area, with wrapping. Will use alignment set on text area.
			 * @see setTextArea
			 * @param  text 	The text to render
			 */
			void printTextArea( const char* text );

		protected:

			const packedbdf_t *_font;
			uint8_t _fontbpp = 1;
			uint8_t _fontbppindex = 0;
			uint8_t _fontbppmask = 1;
			uint8_t _fontppb = 8;
			float _fontalphamx = 1;
			uint32_t _fontdeltaoffset = 0;
			uint32_t _fontspacewidth = 0;

			PreparedColor* _color;

			/**
			 * Clip area
			 */
			ClipRect* _clip;

			/**
			 * text area for bounded, wrapped text
			 */
			TextArea* _textarea;

			/**
			 * Text cursor position for non-text area drawing
			 */
			int16_t _cx = 0;
			int16_t _cy = 0;

			/**
			 * Methods to read bits within packed data
			 */
			uint32_t _fetchbit( const uint8_t *p, uint32_t index );
			uint32_t _fetchbits_unsigned( const uint8_t *p, uint32_t index, uint32_t required );
			uint32_t _fetchbits_signed( const uint8_t *p, uint32_t index, uint32_t required );
			uint32_t _fetchpixel( const uint8_t *p, uint32_t index, uint32_t x );

			/**
			 * Methods to draw a string of characters
			 */
			void _drawFontChar( uint16_t c );
			int32_t _drawFontCharAt( uint16_t c, int16_t x, int16_t y );
			void _drawFontBits( uint32_t bits, uint32_t numbits, int16_t x, int16_t y, uint32_t repeat );
			void _drawFontPixel( uint8_t alpha, int16_t x, int16_t y );

			/**
			 * Methods to get text metrics
			 */
			uint32_t _getTextWidth( char* c );
			uint32_t _getWordWidth( char* c, char* &cw );
			uint32_t _getCharWidth( uint16_t c );
			uint32_t _getSentence( char* c, uint32_t w, char* &cw );
			char* _ignoreWhitespace( char* p );
	};
	
} // namespace

#endif