/**
 * Graphics library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "GraphicsVectorExtn.h"

/**
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Constructor
	 **/
	GraphicsVectorExtn::GraphicsVectorExtn(){
		
	}
	
	/**
	 * Destructor
	 **/
	GraphicsVectorExtn::~GraphicsVectorExtn(){
		
	}
	
	/**
	 * Called by the Graphics object internally. Override base method.
	 * @param	graphics	The graphics object passes itself as a reference
	 **/
	void GraphicsVectorExtn::init( BufferRect* framebuffer ){
		GraphicsExtension::init( framebuffer );
		
	}
	
	/**
	 * Set the line style for subsequent drawing calls
	 * @param	color		Color in RGB565 format
	 * @param	alpha		Alpha from 0 to 255
	 * @param	thickness	Thickness of line in pixels
	 * @param	cap			Cap style of line
	 * @param	join		Join style of line
	 **/
	void GraphicsVectorExtn::lineStyle(
		uint32_t rgb,
		float alpha,
		float thickness,
		capStyle cap,
		joinStyle join
	){
		_lineColorExp = ((rgb & 0xF80000) >> 8) | ((rgb & 0xFC00) << 11) | ((rgb & 0xF8) >> 3);
		_lineThickness = thickness;
		_lineAlphaPre = ((uint8_t)(alpha*255)+4) >> 3;
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
		uint32_t rgb,
		float alpha
	){
		_fillColorExp = ((rgb & 0xF80000) >> 8) | ((rgb & 0xFC00) << 11) | ((rgb & 0xF8) >> 3);
		_fillAlphaPre = ((uint8_t)(alpha*255)+4) >> 3;
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
	void GraphicsVectorExtn::lineTo(
		float x,
		float y
	){
		line( _cursorX, _cursorY, x, y );
	}
	
	/**
	 * Set cursor position without drawing line
	 * @param	x		X Position of cursor
	 * @param	y		Y position of cursor
	 **/
	void GraphicsVectorExtn::moveTo(
		float x,
		float y
	){
		_cursorX = x;
		_cursorY = y;
	}

	/**
	 * Draw a line from one point to another. Based on wu's algorithm
	 * @param	x1		X Position of start of line
	 * @param	y1		Y position of start of line
	 * @param	x2		X Position of end of line
	 * @param	y2		Y position of end of line
	 **/
	void GraphicsVectorExtn::line(
		float x0, float y0,
		float x1, float y1
	){
		_cursorX = x1;
		_cursorY = y1;

		if (!_lineState) return;

		/*
		// XXX: If the line is thick, draw a rectanglular polygon
		if (_lineThickness > 1){
			rectangle();
			return;
		}
		*/
		
		// Split ARGB to alpha and color pre-multiplied for blending 
		uint8_t alpha = _lineAlphaPre;

		// If the line is very thin, simply modify the alpha to make it appear thinner. The actual algorith
		// should probably not be linear, but this works ok.
		if (_lineThickness < 1){
			alpha = (uint8_t)(alpha * _lineThickness + 0.5);
		}
		
		// Different tact for lines > 45 degrees (steep)
		float dx = x1 - x0;
		float dy = y1 - y0;
		if (abs(dx) > abs(dy)){
			// Not steep
			if (x1 < x0){
				swap(x0, x1);
				swap(y0, y1);
			}
			
			// Gradient of line rise/run
			float gradient = dy / dx;
			
			// Pixel pair at end
			float xs = (uint32_t)(x1 + 0.5);
			float ys = y1 + gradient * (xs - x1);
			float gap = (x1+0.5) - xs;
			float f = ys - (uint32_t)ys;
			_pixelBlendExp(
				xs,
				(uint32_t)ys,
				_lineColorExp,
				( alpha * (1 - f) * gap )
			);
			_pixelBlendExp(
				xs,
				(uint32_t)ys + 1,
				_lineColorExp,
				( alpha * f * gap )
			);
			float xe = xs;
			
			// Pixel pair at the start
			xs = (uint32_t)(x0+0.5);
			ys = y0 + gradient * (xs - x0);
			gap = 1 - ((x0 + 0.5) - xs);
			f = ys - (uint32_t)ys;
			_pixelBlendExp(
				xs,
				(uint32_t)ys,
				_lineColorExp,
				( alpha * (1 - f) * gap )
			);
			_pixelBlendExp(
				xs,
				(uint32_t)ys + 1,
				_lineColorExp,
				( alpha * f * gap )
			);
			float y = ys + gradient;
			
			// Pixel pairs in between ends
			for( uint32_t x = xs+1; x < xe; x++) {
				f = y - (uint32_t)y;
				_pixelBlendExp(
					x,
					(uint32_t)y,
					_lineColorExp,
					( alpha * (1 - f) )
				);
				_pixelBlendExp(
					x,
					(uint32_t)y + 1,
					_lineColorExp,
					( alpha * f )
				);
				y += gradient;
			}
			
		}
		else{
			// Steep
			if ( y1 < y0 ) {
				swap(x0, x1);
				swap(y0, y1);
			}
			
			// Gradient flipped
			float gradient = dx / dy;
			
			// Pixel pair at end
			float ys = (uint32_t)(y1 + 0.5);
			float xs = x1 + gradient * (ys - y1);
			float gap = (y1+0.5) - ys;
			float f = xs - (uint32_t)xs;
			_pixelBlendExp(
				(uint32_t)xs,
				ys,
				_lineColorExp,
				( alpha * (1 - f) * gap )
			);
			_pixelBlendExp(
				(uint32_t)xs + 1,
				ys,
				_lineColorExp,
				( alpha * (f * gap) )
			);
			float ye = ys;
			
			// Pixel pair at the start
			ys = (uint32_t)(y0+0.5);
			xs = x0 + gradient * (ys - y0);
			gap = 1 - ((y0 + 0.5) - ys);
			f = xs - (uint32_t)xs;
			_pixelBlendExp(
				(uint32_t)xs,
				ys,
				_lineColorExp,
				( alpha * (1 - f) * gap )
			);
			_pixelBlendExp(
				(uint32_t)xs + 1,
				ys,
				_lineColorExp,
				( alpha * f * gap )
			);
			float x = xs + gradient;
			
			// Pixel pairs in between ends
			for( uint32_t y = ys+1; y < ye; y++) {
				f = x - (uint32_t)x;
				_pixelBlendExp(
					(uint32_t)x,
					y,
					_lineColorExp,
					( alpha * (1 - f) )
				);
				_pixelBlendExp(
					(uint32_t)x+1,
					y,
					_lineColorExp,
					( alpha * f )
				);
				x += gradient;
			}
		}
	}
	
	/**
	 * Triangle
	 **/
	void GraphicsVectorExtn::triangle(
		float x0, float y0,
		float x1, float y1,
		float x2, float y2
	){
		// Order by Y
		VertexF p0 = {x0,y0};
		VertexF p1 = {x1,y1};
		VertexF p2 = {x2,y2};
		if (p0.y > p1.y) swap(p0,p1);
		if (p0.y > p2.y) swap(p0,p2);
		if (p1.y > p2.y) swap(p1,p2);
		
		// Flat top
		if (p0.y == p1.y){
			if (p0.x > p1.x) swap(p0,p1);

			SpanEdge* edges = new SpanEdge[2];
			_createSpanEdge( &edges[0], p0.x,p0.y, p2.x,p2.y );
			_createSpanEdge( &edges[1], p1.x,p1.y, p2.x,p2.y );

			_span( edges, 2 );
		}
		// Flat bottom
		else if (p1.y == p2.y){
			if (p1.x > p2.x) swap(p1,p2);

			SpanEdge* edges = new SpanEdge[2];
			_createSpanEdge( &edges[0], p0.x,p0.y, p1.x,p1.y );
			_createSpanEdge( &edges[1], p0.x,p0.y, p2.x,p2.y );

			_span( edges, 2 );
		}
		// Point at top and bottom (and one somewhere inbetween)
		else{
			// Split triangle
			float m = (p2.y-p0.y)/(p2.x-p0.x);
			VertexF p3 = {p0.x + (p1.y-p0.y)/m, p1.y};

			if (p1.x > p3.x) swap(p1,p3);

			SpanEdge* edges = new SpanEdge[4];
			_createSpanEdge( &edges[0], p0.x,p0.y, p1.x,p1.y );
			_createSpanEdge( &edges[1], p0.x,p0.y, p3.x,p3.y );
			_createSpanEdge( &edges[2], p1.x,p1.y, p2.x,p2.y );
			_createSpanEdge( &edges[3], p3.x,p3.y, p2.x,p2.y );

			_span( edges, 4 );
		}
	}

	void GraphicsVectorExtn::test(){
		SpanEdge* edges = new SpanEdge[2];
		_createSpanEdge( &edges[0], 10,10, 80,190 );
		_createSpanEdge( &edges[1], 50,10, 200,190 );

		_span( edges, 2 );
	}
	
	/**
	 * Create a span edge (line segment) for rendering a polygon
	 * @param x1 [description]
	 * @param y1 [description]
	 * @param x2 [description]
	 * @param y2 [description]
	 */
	void GraphicsVectorExtn::_createSpanEdge( SpanEdge* edge, float x1, float y1, float x2, float y2 ){
		edge->x1 = x1;
		edge->y1 = y1;
		edge->x2 = x2;
		edge->y2 = y2;

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
	}

	/**
	 * A span is a left line and a right line, made up of one or more segments, that
	 * have a solid fill between them. The left and right lines may be made up of
	 * different numbers of segments, but the left and right lines MUST start and finish
	 * on the same y coordinates as each other at the start and end of the span.
	 **/
	void GraphicsVectorExtn::_span( SpanEdge* edges, uint32_t len ){
		uint32_t indexL = 0;
		uint32_t indexR = 1;

		// Step through pairs
		while (indexL<len){

			// Starting values
			edges[indexL].x = edges[indexL].x1;
			edges[indexR].x = edges[indexR].x1;
			_y1 = edges[indexL].y1;
			_y2 = edges[indexL].y2;

			// Starting and finishing y coords
			_cy1 = (uint32_t)_y1;
			_cy2 = (uint32_t)_y2;

			// Adjust line starting point based on slope. Normally the line will start at the top of the
			// pixel and travel downward. The following adjusts for the line starting at the bottom of the
			// pixel and traveling upwards.
			if (edges[indexL].g < 0) edges[indexL].x += edges[indexL].gi;
			if (edges[indexR].g > 0) edges[indexR].x += edges[indexR].gi;

			// Very first scanline might be a partial line
			_solid = 1;
			if (indexL==0) _solid -= (_y1-_cy1); // Leading partial scanline

			// Step each scanline until reach end of span
			while (_cy1 < _cy2){
				_scanline(
					_cy1,
					&edges[indexL],
					&edges[indexR]
				);
				_cy1++;
				_solid = 1;
				edges[indexL].x += edges[indexL].gi;
				edges[indexR].x += edges[indexR].gi;
			}

			// Next edge pair
			indexL+=2;
			indexR+=2;
		}

		// Final partial scanline
		if ((_y2-_cy1)>0){
			indexL-=2;
			indexR-=2;
			_solid = _y2-_cy1;
			_scanline(
				_cy1,
				&edges[indexL],
				&edges[indexR]
			);
		}
	}

	/**
	 * Draw a single horizontal line of a span
	 */
	void GraphicsVectorExtn::_scanline( uint32_t cy, SpanEdge* left, SpanEdge* right ){

		_solidPre = _solid * _fillAlphaPre;

		left->cx = (uint32_t)left->x; 		// Coord-x
		left->solid = false;

		// Pixel coverage
		if (left->steep) left->pc = (left->cx+1-left->x) - left->gi*0.5;
		else left->pc = (left->cx+1-left->x)*left->ga*0.5;

		right->cx = (uint32_t)right->x; 	// Coord-x
		right->solid = false;

		// Pixel coverage
		if (right->steep) right->pc = (right->x-right->cx) + right->gi*0.5;
		else right->pc = (right->x-right->cx)*right->ga*0.5;
	
		// Render two points at a time, one from left, one from right, and converge.
		while (true){
			// Overlapped. We are done
			if (left->cx > right->cx) break;

			// Combined
			if (left->cx==right->cx){
				left->pc = (left->pc + right->pc)*0.5; // Average
				_pixelBlendExp(
					left->cx,
					cy,
					_fillColorExp,
					(uint8_t)(_solidPre * left->pc)
				);
				break;
			}

			if (left->solid){
				_pixelBlendExp(
					left->cx,
					cy,
					_fillColorExp,
					(uint8_t)(_solidPre)
				);
			}
			else{
				_pixelBlendExp(
					left->cx,
					cy,
					_fillColorExp,
					(uint8_t)(_solidPre * left->pc)
				);

				left->pc += left->ga;
				if (left->pc >= 1){
					left->solid = true;
					left->pc = 1;
				}
			}
			if (right->solid){
				_pixelBlendExp(
					right->cx,
					cy,
					_fillColorExp,
					(uint8_t)(_solidPre)
				);
			}
			else{
				_pixelBlendExp(
					right->cx,
					cy,
					_fillColorExp,
					(uint8_t)(_solidPre * right->pc)
				);

				right->pc += right->ga;
				if (right->pc >= 1){
					right->solid = true;
					right->pc = 1;
				}
			}

			left->cx++;
			right->cx--;
		}
	}
	
}