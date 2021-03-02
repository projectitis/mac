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
#ifndef _MAC_GRAPHICSVECTOREXTNH_
#define _MAC_GRAPHICSVECTOREXTNH_ 1

#include "GraphicsExtension.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
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
		int16_t cx;		// Current X position
		uint32_t i;		// Framebuffer index
		float pc;		// Pixel coverage

		// Linked list
		SpanEdgeS* next;
	} SpanEdge;
	
	/**
	 * Adds vector drawing functions to the Graphics library
	 */
	class GraphicsVectorExtn: public GraphicsExtension {
		public:

			/**
			 * Set the line style for subsequent drawing calls
			 * @param	color		Color in 24-bit RGB format
			 * @param	alpha		Alpha from 0 to 1
			 * @param	thickness	thickness of line in pixels
			 * @param	cap			Cap style of line
			 * @param	join		Join style of line
			 **/
			void lineStyle(
				color888 color,
				alpha alpha = 1,
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
			 * @param	color		Color in 24-bit RGB format
			 * @param	alpha		Alpha from 0 to 1
			 **/
			void fillStyle(
				color888 color,
				alpha alpha = 1
			);
			
			/**
			 * Set drawing operations to draw without fill
			 **/
			void noFill();
			
			/**
			 * Draw a line from one point to another. This is a call to moveTo
			 * followed by lineTo.
			 * @param	p1		Start of line
			 * @param	p2		End of line
			 **/
			void line( VertexF p1, VertexF p2 );
			
			/**
			 * Draw a line from cursor position to specified coord
			 * @param	p		End position of line
			 **/
			void lineTo( VertexF p );
			
			/**
			 * Set cursor position without drawing line
			 * @param	p		Cursor position
			 **/
			void moveTo( VertexF p );

			/**
			 * Draw a rectangle
			 * @param p1 	Top-left
			 * @param p2 	Bottom-right
			 */
			void rectangle( VertexF p1, VertexF p2 );

			/**
			 * Triangle
			 * @param p1 	First point
			 * @param p2 	Second point
			 * @param p3	Third point
			 */
			void triangle( VertexF p1, VertexF p2, VertexF p3 );
			
		protected:
			
			/**
			 * Drawing state vars
			 **/
			VertexF _cursor = {0,0};
			boolean _lineState = false;
			boolean _fillState = false;
			PreparedColor* _lineColor;
			PreparedColor* _fillColor;
			float _lineThickness = 1.0;
			capStyle _lineCap = CAP_SQUARE;
			joinStyle _lineJoin = JOIN_BEVEL;

			/**
			 * Pool of re-usable span edges
			 */
			SpanEdge* _pool = 0;

			/**
			 * Delete an array of span edges. Actually places them back in the pool.
			 * @param	edges 		The array of edges to recycle/delete
			 * @param	numEdges	The number of edges in the array. Use sizeof(edges)/sizeof(edges[0])
			 **/
			void _deleteSpanEdges( SpanEdge* edges[], uint32_t numEdges );

			/**
			 * Create and populate a span edge (line segment) for rendering a polygon.
			 * Will take them from the pool if they exist, or allocate new ones.
			 * @param x1  	X coordinate of first point
			 * @param y1 	Y coordinate of first point
			 * @param x2 	X coordinate of second point
			 * @param y2 	Y coordinate of second point
			 */
			SpanEdge* _createSpanEdge( VertexF& p1, VertexF& p2 );

			/**
			 * A span is a left line and a right line, made up of one or more segments, that
			 * have a solid fill between them. The left and right lines may be made up of
			 * different numbers of segments, but the left and right lines MUST start and finish
			 * on the same y coordinates as each other at the start and end of the span.
			 **/
			void _span( SpanEdge* edges[], uint32_t len );

			/**
			 * Sort an array of vertices by Y
			 * @param vertices 		The array of vertices
			 * @param len      The number of vertices
			 */
			void _sortVertical( VertexF* vertices, uint32_t len );

			/**
			 * ### 
			 * ### SUBCLASSES TO IMPLEMENT
			 * ###
			 * The following methods need to be implemented by the pixel-format specific
			 * sub-classes.
			 */

			/**
			 * Draw a single horizontal line of a span.
			 * This should be implemented by the pixel-format specific version of the extension.
			 * @param	cy 			The Y coordinate of the scanline
			 * @param	left 		The left span edge
			 * @param 	right 		The right span edge
			 * @param	coverage 	The proportion of the line that is covered (0.0 - 1.0)
			 */
			virtual void _scanline( uint32_t cy, SpanEdge* left, SpanEdge* right, float coverage );

			/**
			 * Draw a pixel-width line.
			 * This should be implemented by the pixel-format specific version of the extension.
			 */
			virtual void _line( VertexF p1, VertexF p2 );

	};
	
} // namespace

#endif