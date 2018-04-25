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
 
#ifndef _MAC_GRAPHICSVECTOREXTNH_
#define _MAC_GRAPHICSVECTOREXTNH_ 1

#include "GraphicsExtension.h"
#include <agg2d.h>

/**
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Line cap styles
	 **/
	typedef enum {
		CAP_BUTT,
		CAP_SQUARE,
		CAP_ROUND
	} capStyle;
	
	/**
	 * Line join styles
	 **/
	typedef enum {
		JOIN_MITER,
		JOIN_ROUND,
		JOIN_BEVEL
	} joinStyle;
	
	/**
	 * Adds vector drawing functions to the Graphics library via anti-grain geometry (AGG).
	 * Many thanks to the grate work of the late Maxim "mcseem" Shemanarev: http://www.antigrain.com/
	 */
	class GraphicsVectorExtn: public GraphicsExtension {
		public:
			
			/**
			 * Constructor
			 **/
			GraphicsVectorExtn();
			
			/**
			 * Destructor
			 **/
			~GraphicsVectorExtn();
			
			/**
			 * Called by the Graphics object internally. Override base method.
			 * @param	graphics	The graphics object passes itself as a reference
			 **/
			void init( BufferRect* framebuffer );
			
			/**
			 * Set the line style for subsequent drawing calls
			 * @param	color		Color in RGB565 format
			 * @param	alpha		Alpha from 0 to 255
			 * @param	thickness	thickness of line in pixels
			 * @param	cap			Cap style of line
			 * @param	join		Join style of line
			 **/
			void lineStyle(
				color color,
				alpha alpha = 255,
				float thickness = 1,
				capStyle cap = CAP_ROUND,
				joinStyle join = JOIN_ROUND
			);
			
			/**
			 * Set drawing operations to draw without line
			 **/
			void noLine();
			
			/**
			 * Set the fill style for subsequent drawing calls
			 * @param	color	Color in RGB565 format
			 * @param	alpha	Alpha from 0 to 1
			 **/
			void fillStyle(
				color color,
				alpha alpha = 255
			);
			
			/**
			 * Set drawing operations to draw without fill
			 **/
			void noFill();
			
			/**
			 * Draw a line from one point to another. This is a call to moveTo
			 * followed by lineTo.
			 * @param	x1		X Position of start of line
			 * @param	y1		Y position of start of line
			 * @param	x2		X Position of end of line
			 * @param	y2		Y position of end of line
			 **/
			void line(
				float x1,
				float y1,
				float x2,
				float y2
			);
			
			/**
			 * Draw a line from cursor position to specified coord
			 * @param	x		X Position of end of line
			 * @param	y		Y position of end of line
			 **/
			void lineTo(
				float x,
				float y
			);
			
			/**
			 * Set cursor position without drawing line
			 * @param	x		X Position of cursor
			 * @param	y		Y position of cursor
			 **/
			void moveTo(
				float x,
				float y
			);
			
		protected:
			
			/**
			 * Instance of the AGG 2D class that does all the work
			 **/
			Agg2D *_agg2D;
			
			/**
			 * Cursor position
			 **/
			float _cursorX = 0;
			float _cursorY = 0;

	};
	
} // namespace

#endif