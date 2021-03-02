/**
 * Graphics library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "GraphicsVectorExtn.h"
#include <Math.h>

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Set the line style for subsequent drawing calls
	 * @param	color		Color in 24-bit RGB888 format
	 * @param	alpha		Alpha from 0.0 - 1.0
	 * @param	thickness	Thickness of line in pixels
	 * @param	cap			Cap style of line
	 * @param	join		Join style of line
	 **/
	void GraphicsVectorExtn::lineStyle(
		color888 c,
		alpha alpha,
		float thickness,
		capStyle cap,
		joinStyle join
	){
		alpha = alphaClamp( alpha );
		_lineColor = _framebuffer->prepareColor( c, alpha );
		_lineThickness = thickness;
		_lineCap = cap;
		_lineJoin = join;
		if ((_lineThickness>0) && (alpha>0)){
			_lineState = true;
		}
		else {
			_lineState = false;
			if (_lineThickness<0) _lineThickness = 0;
		}
	}
	
	/**
	 * Set drawing operations to draw without line
	 **/
	void GraphicsVectorExtn::noLine(){
		_lineState = false;
	}
	
	/**
	 * Set the fill style for subsequent drawing calls
	 * @param	color	Color in RGB565 format
	 * @param	alpha	Alpha from 0 to 1
	 **/
	void GraphicsVectorExtn::fillStyle(
		color888 c,
		float alpha
	){
		alpha = alphaClamp( alpha );
		_fillColor = _framebuffer->prepareColor( c, alpha );
		_fillState = (alpha>0);
	}
	
	/**
	 * Set drawing operations to draw without fill
	 **/
	void GraphicsVectorExtn::noFill(){
		_fillState = false;
	}
	
	/**
	 * Draw a line from cursor position to specified coord
	 * @param	x		X Position of end of line
	 * @param	y		Y position of end of line
	 **/
	void GraphicsVectorExtn::lineTo( VertexF p ){
		line( _cursor, p );
	}
	
	/**
	 * Set cursor position without drawing line
	 * @param	x		X Position of cursor
	 * @param	y		Y position of cursor
	 **/
	void GraphicsVectorExtn::moveTo( VertexF p ){
		_cursor.x = p.x;
		_cursor.y = p.y;
	}

	/**
	 * Draw a line from one point to another.
	 * @param	x1		X Position of start of line
	 * @param	y1		Y position of start of line
	 * @param	x2		X Position of end of line
	 * @param	y2		Y position of end of line
	 **/
	void GraphicsVectorExtn::line( VertexF p1, VertexF p2 ){
		_cursor.x = p2.x;
		_cursor.y = p2.y;

		if (!_lineState) return;
		if (_lineThickness<=0) return;

		// Draw a 1-pixel wide line
		if (_lineThickness <= 1){
			_line( p1, p2 );
			return;
		}

		// Draw a rectangle if line exactly horizontal
		/*
		if (p1.y == p2.y){
			if (p1.x > p2.x) swap(p1,p2);
			VertexF p3 = { p1.x, p1.y - _lineThickness*0.5 }; // top-left
			VertexF p4 = { p2.x, p2.y + _lineThickness*0.5 }; // bottom-right
			rectangle( p3, p4 );
			return;
		}

		// Draw a rectangle if line exactly vertical
		if (p1.x == p2.x){
			if (p1.y > p2.y) swap(p1,p2);
			VertexF p3 = { p1.x - _lineThickness*0.5, p1.y }; // top-left
			VertexF p4 = { p2.x + _lineThickness*0.5, p2.y }; // bottom-right
			rectangle( p3, p4 );
			return;
		}
		*/

		// XXX:
		// Calculate corners of rotated rect (line with thickness)
		float g = -(p2.y - p1.y) / (p2.x - p1.x);
		float a = atan2f((p2.x - p1.x),(p2.y - p1.y));
		float dy = sinf(a) * _lineThickness * 0.5;
		float dx = dy * g;
		VertexF p[6] = {
			{p1.x-dx, p1.y-dy},
			{p1.x+dx, p1.y+dy},
			{p2.x-dx, p2.y-dy},
			{p2.x+dx, p2.y+dy}
		};
		_sortVertical( p, 4 );
		// p[0] A
		// p[1] B
		p[4] = p[2]; // C
		p[5] = p[3]; // D

		// Calculate additional points to split triangles
		float m = (p[4].y-p[0].y)/(p[4].x-p[0].x);
		p[2] = {p[0].x + (p[1].y-p[0].y)/m, p[1].y};
		p[3] = {p[1].x + (p[4].y-p[1].y)/m, p[4].y};

		if (p[1].x > p[2].x){
			swap(p[1],p[2]);
			swap(p[3],p[4]);
		}

		SpanEdge* edges[6];
		edges[0] = _createSpanEdge( p[0], p[1] );
		edges[1] = _createSpanEdge( p[0], p[2] );
		edges[2] = _createSpanEdge( p[1], p[3] );
		edges[3] = _createSpanEdge( p[2], p[4] );
		edges[4] = _createSpanEdge( p[3], p[5] );
		edges[5] = _createSpanEdge( p[4], p[5] );
		PreparedColor* c = _fillColor;
		_fillColor = _lineColor;
		_span( edges, 6 );
		_fillColor = c;
		_deleteSpanEdges( edges, sizeof(edges)/sizeof(edges[0]) );
	}

	/**
	 * Draw a rectangle
	 */
	void GraphicsVectorExtn::rectangle( VertexF p1, VertexF p2 ){
		// Vertices
		// p1 top-left
		// p2 bottom-right
		VertexF p3 = { p2.x ,p1.y }; // top-right
		VertexF p4 = { p1.x, p2.y }; // bottom-left
		if (p2.y < p1.y){
			swap(p1,p4);
			swap(p2,p3);
		}

		// Draw solid fill
		if (_fillState){
			SpanEdge* edges[2];
			edges[0] = _createSpanEdge( p1, p4 );
			edges[1] = _createSpanEdge( p3, p2 );
			_span( edges, 2 );
			_deleteSpanEdges( edges, sizeof(edges)/sizeof(edges[0]) );
		}

		// Draw lines
		if (_lineState){
			line( p1, p3 );
			lineTo( p2 );
			lineTo( p4 );
			lineTo( p1 );
		}
	}
	
	/**
	 * Triangle
	 **/
	void GraphicsVectorExtn::triangle( VertexF p1, VertexF p2, VertexF p3 ){
		// Draw solid fill
		if (_fillState){
			// Order by Y
			if (p1.y > p2.y) swap(p1,p2);
			if (p1.y > p3.y) swap(p1,p3);
			if (p2.y > p3.y) swap(p2,p3);
			
			// Flat top
			if (p1.y == p2.y){
				if (p1.x > p2.x) swap(p1,p2);

				SpanEdge* edges[2];
				edges[0] = _createSpanEdge( p1, p3 );
				edges[1] = _createSpanEdge( p2, p3 );
				_span( edges, 2 );
				_deleteSpanEdges( edges, sizeof(edges)/sizeof(edges[0]) );
			}
			// Flat bottom
			else if (p2.y == p3.y){
				if (p2.x > p3.x) swap(p2,p3);

				SpanEdge* edges[2];
				edges[0] = _createSpanEdge( p1, p2 );
				edges[1] = _createSpanEdge( p1, p3 );
				_span( edges, 2 );
				_deleteSpanEdges( edges, sizeof(edges)/sizeof(edges[0]) );
			}
			// Point at top and bottom (and one somewhere inbetween)
			else{
				// Split triangle
				float m = (p3.y-p1.y)/(p3.x-p1.x);
				VertexF p4 = {p1.x + (p2.y-p1.y)/m, p2.y};

				if (p2.x > p4.x) swap(p2,p4);

				SpanEdge* edges[4];
				edges[0] = _createSpanEdge( p1, p2 );
				edges[1] = _createSpanEdge( p1, p4 );
				edges[2] = _createSpanEdge( p2, p3 );
				edges[3] = _createSpanEdge( p4, p3 );
				_span( edges, 4 );
				_deleteSpanEdges( edges, sizeof(edges)/sizeof(edges[0]) );
			}
		}

		// Draw lines
		if (_lineState){
			line( p1, p2 );
			lineTo( p3 );
			lineTo( p1 );
		}
	}

	/**
	 * Sort an array of vertices by Y
	 * @param vertices 		The array of vertices
	 * @param len      The number of vertices
	 */
	void GraphicsVectorExtn::_sortVertical( VertexF* vertices, uint32_t len ){
		if (len<2) return;

		// Insertion sort
		VertexF v;
		int16_t j;
		uint16_t i = 0;
		while (i<len){
			v = vertices[i];
			j = i;
			while (j>0){
				if (v.y < vertices[j-1].y){
					vertices[j] = vertices[j-1];
					j--;
				}
				else break;
			}
			vertices[j] = v;
			i++;
		}
	}

	/**
	 * Delete an array of span edges. Actually places them back in the pool.
	 * @param	edges 		The array of edges to recycle/delete
	 * @param	numEdges	The number of edges in the array. Use sizeof(edges)/sizeof(edges[0])
	 **/
	void GraphicsVectorExtn::_deleteSpanEdges( SpanEdge* edges[], uint32_t numEdges ){
		while (numEdges--){
			edges[numEdges]->next = _pool;
			_pool = edges[numEdges];
		}
	}
	
	/**
	 * Populate a span edge (line segment) for rendering a polygon
	 */
	SpanEdge* GraphicsVectorExtn::_createSpanEdge( VertexF& p1, VertexF& p2 ){
		SpanEdge* edge;
		if (_pool){
			edge = _pool;
			_pool = edge->next;
		}
		else{
			edge = new SpanEdge();
		}
		edge->next = 0;

		edge->x1 = p1.x;
		edge->y1 = p1.y;
		edge->x2 = p2.x;
		edge->y2 = p2.y;

		edge->dx = edge->x2-edge->x1;
		edge->dy = edge->y2-edge->y1;
		if (edge->dx==0){
			edge->g = 100000;
			edge->gi = 0;
		}
		else{
			edge->g = edge->dy/edge->dx;
			edge->gi = 1/edge->g;
		}
		edge->ga = abs(edge->g);
		edge->gia = abs(edge->gi);
		edge->steep = (edge->g<-1) || (edge->g>1);

		return edge;
	}

	/**
	 * A span is a left line and a right line, made up of one or more segments, that
	 * have a solid fill between them. The left and right lines may be made up of
	 * different numbers of segments, but the left and right lines MUST start and finish
	 * on the same y coordinates as each other at the start and end of the span.
	 **/
	void GraphicsVectorExtn::_span( SpanEdge* edges[], uint32_t len ){
		uint32_t indexL = 0;
		uint32_t indexR = 1;

		float y1;
		float y2 = 0;
		uint32_t cy1 = 0;
		uint32_t cy2;
		float coverage;

		// Step through pairs
		// XXX: When a new scanline immediately follows another, the two lines may be partial lines each
		// and need to be be blended somehow. This should be handled at (1) below.
		while (indexL<len){

			// Starting values
			edges[indexL]->x = edges[indexL]->x1;
			edges[indexR]->x = edges[indexR]->x1;
			y1 = edges[indexL]->y1;
			y2 = edges[indexL]->y2;

			// Starting and finishing y coords
			cy1 = (uint32_t)y1;
			cy2 = (uint32_t)y2;

			// (1) Very first scanline might be a partial line
			coverage = 1;
			if (indexL==0) coverage -= (y1-cy1); // Leading partial scanline

			// Adjust line starting point based on slope. Normally the line will start at the top of the
			// pixel and travel downward. The following adjusts for the line starting at the bottom of the
			// pixel and traveling upwards.
			if (edges[indexL]->g < 0) edges[indexL]->x += edges[indexL]->gi*coverage;
			if (edges[indexR]->g > 0) edges[indexR]->x += edges[indexR]->gi*coverage;

			// Step each scanline until reach end of span
			while (cy1 < cy2){
				_scanline(
					cy1,
					edges[indexL],
					edges[indexR],
					coverage
				);
				cy1++;
				coverage = 1;
				edges[indexL]->x += edges[indexL]->gi;
				edges[indexR]->x += edges[indexR]->gi;
			}

			// Next edge pair
			indexL+=2;
			indexR+=2;
		}

		// Final partial scanline
		if ((y2-cy1)>0){
			indexL-=2;
			indexR-=2;
			coverage = y2-cy1;
			_scanline(
				cy1,
				edges[indexL],
				edges[indexR],
				coverage
			);
		}
	}
	
}