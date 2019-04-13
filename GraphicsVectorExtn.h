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
	 * Defines a span edge for rendering polygons
	 **/
	typedef struct SpanEdgeS {
		float x1;		// X position of start
		float y1;		// Y position of start
		float x2;		// X position of end
		float y2;		// Y position of end
		float dx;
		float dy;
		float g;		// Gradient
		float ga;		// abs(gradient)
		float gi;		// 1/gradient (inverse)
		float gia;		// abs(1/gradient)
		boolean steep;

		// Temp vars
		boolean solid;	// Flag that pixels should be solid
		float x;		// Actual x position
		uint32_t cx;	// X coord
		float pc;		// Pixel coverage
	} SpanEdge;
	
	/**
	 * Adds vector drawing functions to the Graphics library
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
				uint32_t rgb,
				float alpha = 1,
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
				uint32_t rgb,
				float alpha = 1
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

			/**
			 * Triangle
			 * @param x0 	First point x
			 * @param y0 	First point y
			 * @param x1 	Second point x
			 * @param y1 	Second point y
			 * @param x2	Third point x
			 * @param y2 	Third point y
			 */
			void triangle(
				float x0, float y0,
				float x1, float y1,
				float x2, float y2
			);
			
			void test();
			
		protected:
			
			/**
			 * Drawing state vars
			 **/
			float _cursorX = 0;
			float _cursorY = 0;
			boolean _antialias = true;
			boolean _lineState = true;
			boolean _fillState = true;
			uint32_t _lineColorExp = 0; // Pre-expanded
			uint8_t _lineAlphaPre = 32; // Pre-multiplied
			float _lineThickness = 1.0;
			capStyle _lineCap = CAP_SQUARE;
			joinStyle _lineJoin = JOIN_BEVEL;
			uint32_t _fillColorExp = 0; // Pre-expanded
			uint8_t _fillAlphaPre = 32; // Pre-multiplied

			/**
			 * Scanline temp vars
			 */
			float _y1;
			float _y2;
			uint32_t _cy1;
			uint32_t _cy2;
			float _solid;
			float _solidPre;

			/**
			 * Create a span edge (line segment) for rendering a polygon
			 * @param x1 [description]
			 * @param y1 [description]
			 * @param x2 [description]
			 * @param y2 [description]
			 */
			void _createSpanEdge( SpanEdge* edge, float x1, float y1, float x2, float y2 );

			/**
			 * A span is a left line and a right line, made up of one or more segments, that
			 * have a solid fill between them. The left and right lines may be made up of
			 * different numbers of segments, but the left and right lines MUST start and finish
			 * on the same y coordinates as each other at the start and end of the span.
			 **/
			void _span( SpanEdge* edges, uint32_t len );

			/**
			 * Draw a single horizontal line of a span
			 */
			void _scanline( uint32_t cy, SpanEdge* left, SpanEdge* right );
	};
	
} // namespace

#endif