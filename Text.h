/**
 * GUI library for "mac/μac"
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
#ifndef _MAC_TEXTH_
#define _MAC_TEXTH_ 1

#include "DisplayObject.h"
#include "Bitmap.h"
#include "PackedBDF.h"
#include "ClipRect.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Text alignment
	 */
	enum class TextAlign{
		left,
		right,
		center
	};

	/**
	 * A text area
	 */
	class Text: public DisplayObject {
		
		public:

			/**
			 * @brief Construct a new Text object
			 */
			Text();

			/**
			 * @brief Destroy the Text object
			 */
			~Text();

			/**
			 * Memory pool of recycled objects
			 */
			static DisplayObject* pool;

			/**
			 * Create a new object or take one from the pool
			 * @return The new or recycled object
			 */
			static Text* Create();
			static Text* Create( packedbdf_t* font );

			/**
			 * Type identifier for this object
			 **/
			static const DisplayObjectType TYPE = DisplayObjectType::text;

			/**
			 * Reset the object back to default settings
			 */
			void reset() override;

			/**
			 * @brief Set the font that this text uses
			 * @param font 	The font to use
			 */
			void font( packedbdf_t* font );

			/**
			 * @brief Set the text color
			 * @param c The text color
			 */
			void color( color888 c );

			/**
			 * @brief Set the line height of the text
			 * @param lh 	The line height. Usually between 1 - 1.5
			 */
			void lineHeight( float lh );

			/**
			 * @brief Set the text alignment
			 * @param align The alignment
			 */
			void align( TextAlign a );

			/**
			 * @brief Set the text string
			 * @param t The text string
			 */
			void text( char* t );

			/**
			 * Set the position at which to read the next pixel
			 * @param x The global x coordinate
			 * @param y The global y coordinate
			 */
			virtual void readPosition( int16_t gx, int16_t gy );

			/**
			 * Read a pixel from the sprite and advance position
			 * @param c (out) color
			 * @param a (out) alpha
			 */
			virtual void readPixel( color888 &c, float &a );

		protected:
			
			/**
			 * Pool getter
			 */
			DisplayObject** _getPool() override;

			char* _text = 0;
			packedbdf_t* _font = 0;
			color888 _color = 0;
			float _lineHeight = 1.25f;
			TextAlign _align = TextAlign::left;

			uint8_t _fontbpp = 1;
			uint8_t _fontbppmask = 1;
			uint8_t _fontppb = 8;
			float _fontalphamx = 1;
			uint32_t _fontdeltaoffset = 0;
			uint32_t _fontspacewidth = 0;

			/**
			 * Text cursor position for non-text area drawing
			 */
			int32_t _charIndex = 0;		// Current character index
			uint32_t _pixelOffset = 0;	// Current character bit (0 to char width)
			uint32_t _charWidth = 0;	// Current character width
			ClipRect* _glyphBounds;		// Bounds of glyph
			int32_t _nextCharX = 0;		// Next character X in string

			uint32_t _bitoffset = 0;	// Offset intp glyph data
			const uint8_t *_data;		// Glyph data

			/**
			 * Methods to prepare for drawing a font character
			 */
			void _prepareCharAt( int16_t x );
			void _prepareChar();
			void _nextChar();

			/**
			 * Methods to read bits within packed data
			 */
			uint32_t _fetchbit( const uint8_t *p, uint32_t index );
			uint32_t _fetchbits_unsigned( const uint8_t *p, uint32_t index, uint32_t required );
			int32_t _fetchbits_signed( const uint8_t *p, uint32_t index, uint32_t required );
			uint32_t _fetchpixel();

			/**
			 * Methods to get text metrics
			 */
			int32_t _getTextWidth();
			int32_t _getWordWidth( char* c, char* &cw );
			int32_t _getCharWidth( uint16_t c );
			int32_t _getSentence( char* c, uint32_t w, char* &cw );
			char* _ignoreWhitespace( char* p );

	};
	
} // namespace

#endif