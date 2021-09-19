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
#ifndef _MAC_GLYPHINFOH_
#define _MAC_GLYPHINFOH_ 1

#include "Common.h"
#include "geom/ClipRect.h"
#include "utils/LinkedList.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Display list that stores a list of display objects in render order.
	 */
	class GlyphInfo: public LinkedList {
		
		public:

			/**
			 * Memory pool of recycled objects
			 */
			static GlyphInfo* pool;

			/**
			 * Create factory method. Uses memory pool
			 * for object re-use.
			 */
			static GlyphInfo* Create( char c ){
				GlyphInfo* obj;
				if (GlyphInfo::pool){
					obj = GlyphInfo::pool;
					GlyphInfo::pool = obj->_poolNext;
					obj->code = c;
				}
				else{
					obj = new GlyphInfo( c );
				}
				return obj;
			}

			/**
			 * @brief Construct a new Glyph Info object
			 */
			GlyphInfo( char c );

			/**
			 * @brief Destroy the Glyph Info object
			 */
			~GlyphInfo();

			/**
			 * Return this object to the pool
			 */
			void recycle();

			/**
			 * @brief Set the glyph to a new character and position
			 * 
			 * @param c 	The character code
			 * @param x 	The x position
			 * @param baseline The y position of the baseline
			 */
			void set( char c );

			/**
			 * @brief The character code of this glyph
			 */
			char code;

			/**
			 * @brief Rect describing the graphical bounds of the glyph
			 */
			ClipRect* bounds;

			/**
			 * @brief Pointer to the character data
			 */
			const uint8_t* charData = 0;

			/**
			 * @brief Pointer to the current line of character data
			 */
			uint32_t dataOffset = 0;

			/**
			 * @brief Indicate whether the glyph has started rendering yet
			 */
			boolean renderStarted = false;

			/**
			 * @brief Number of repeats left on current line
			 */
			int8_t lineRepeat = 0;

			/**
			 * @brief Width of the glyph (to next character)
			 */
			uint32_t width;

			/**
			 * Return the next item
			 */
			GlyphInfo* next();
			
			/**
			 * Inserts a glyph into the list into sorted order by x position
			 * @param glyph The glyph to insert
			 */
			void insert( GlyphInfo* glyph );

			/**
			 * Remove the current node from the list
			 * @return The removed node (self)
			 */
			GlyphInfo* remove();

		protected:

			/**
			 * Pointer to next object in memory pool of recycled objects
			 */
			GlyphInfo * _poolNext = 0;
			
	};
	
} // namespace

#endif