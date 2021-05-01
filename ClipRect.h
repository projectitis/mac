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
#ifndef _MAC_CLIPRECTH_
#define _MAC_CLIPRECTH_ 1

#include <Arduino.h>

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * A clipping rectangle class
	 */
	class ClipRect {
		
		public:
			/**
			 * Constructor
			 */
			ClipRect();

			/**
			 * Constructor
			 */
			ClipRect( int16_t px, int16_t py, int16_t w, int16_t h );

			/**
			 * X coordinate of top-left corner.
			 * Use setters to set so that x2 and width are adjusted appropriately.
			 */
			int16_t x;

			/**
			 * Y coordinate of top-left corner.
			 * Use setters to set so that y2 and height are adjusted appropriately.
			 */
			int16_t y;

			/**
			 * X coordinate of bottom-right corner.
			 * Use setters to set so that width is adjusted appropriately.
			 */
			int16_t x2;

			/**
			 * Y coordinate of bottom-right corner.
			 * Use setters to set so that height is adjusted appropriately.
			 */
			int16_t y2;

			/**
			 * Width of rect. If 0, rect is invalid.
			 * Use setters to set so that x2 is adjusted appropriately.
			 */
			uint16_t width;

			/**
			 * Height of rect. If 0, rect is invalid.
			 * Use setters to set so that y2 is adjusted appropriately.
			 */
			uint16_t height;

			/**
			 * Clear back to an empty rect
			 */
			void clear();

			/**
			 * Set (copy) the size of the rect from the supplied rect
			 * @param rect   	The rect to set from
			 */
			void set( ClipRect* rect );

			/**
			 * Set the top-left and bottom-right corners of rect.
			 * Adjusts width and height.
			 * @param px   	X coordinate of top-left corner 
			 * @param py 	Y coordinate of top-left corner
			 * @param px2	X coordinate of bottom-right corner
			 * @param py2  	Y coordinate of bottom-right corner
			 */
			void set( int16_t px, int16_t py, int16_t px2, int16_t py2 );

			/**
			 * Set width of rect
			 * @param w The width
			 */
			void setWidth( uint16_t w );

			/**
			 * Set height of rect
			 * @param h The height
			 */
			void setHeight( uint16_t h );

			/**
			 * Set the top-left corner of rect.
			 * Adjusts bottom-right corner
			 * @param px   	X coordinate of top-left corner 
			 * @param py 	Y coordinate of top-left corner
			 **/
			void setPos( int16_t px, int16_t py );

			/**
			 * Set the bottom-right corner of rect.
			 * Adjusts width and height
			 * @param px   	X coordinate of bottom-right corner 
			 * @param py 	Y coordinate of bottom-right corner
			 **/
			void setPos2( int16_t px, int16_t py );

			/**
			 * Set the width and height of the rect
			 * Adjusts x2, y2
			 * @param w   	Width of rect
			 * @param h 	Height of rect
			 **/
			void setSize( int16_t w, int16_t h );

			/**
			 * Set the top-left corner and width and height of the rect
			 * Adjusts x2, y2
			 * @param px   	X coordinate of top-left corner 
			 * @param py 	Y coordinate of top-left corner
			 * @param w   	Width of rect
			 * @param h 	Height of rect
			 **/
			void setPosAndSize( int16_t px, int16_t py, int16_t w, int16_t h );

			/**
			 * Move the rect by the specified amount
			 * @param x The amount to move in the X direction
			 * @param y The amount to move in the Y direction
			 */
			void translate( int16_t x, int16_t y );
			
			/**
			 * Return true if the clip rect contains the specified point
			 * @param  px  	X coord to test
			 * @param  py 	Y coord to test
			 * @return   True if point within rect
			 */
			boolean contains( int16_t px, int16_t py );

			/**
			 * Return true if the clip rect contains the specified X coord
			 * @param  px  	X coord to test
			 * @return   True if point within rect
			 */
			boolean containsX( int16_t px );

			/**
			 * Return true if the clip rect contains the specified Y coord
			 * @param  px  	Y coord to test
			 * @return   True if point within rect
			 */
			boolean containsY( int16_t py );

			/**
			 * Check if the clip rect is empty (has a 0 width or height)
			 * @return True if the rect is empty
			 */
			boolean isEmpty();

			/**
			 * Check if any part of another rect overlaps this one
			 * @param rect The other rect
			 * @return boolean True if any part of the rects overlap
			 */
			boolean overlaps( ClipRect* rect );

			/**
			 * Clip this rect to another rect (intersection)
			 * @param  rect  	The rect to clip to
			 */
			void clip( ClipRect* rect );

			/**
			 * Clip this rect to a rectangular area specified by position and size (intersection)
			 * @param  px  	X coord to clip
			 * @param  py 	Y coord to clip
			 * @param  w  	Width
			 * @param  h 	Height
			 */
			void clipPosAndSize( int16_t px, int16_t py, int16_t w, int16_t h );

			/**
			 * Expand this rect to also encompase the specified rect (union)
			 * @param rect The rect to encompase
			 */
			void grow( ClipRect* rect );

			/**
			 * Expand this rect to also encompase a rectangular area specified by position and size (union)
			 * @param  px  	X coord to clip
			 * @param  py 	Y coord to clip
			 * @param  w  	Width
			 * @param  h 	Height
			 */
			void growPosAndSize( int16_t px, int16_t py, int16_t w, int16_t h );

	};
	
} // namespace

#endif