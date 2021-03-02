/**
 * Graphics library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "GraphicsVectorExtn16.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Draw a line from one point to another. Based on wu's algorithm
	 **/
	void GraphicsVectorExtn16::_line( VertexF p1, VertexF p2 ){
		// Grab pre-multiplied alpha for blending 565 (0-31)
		uint8_t alpha = _lineColor->a5;

		// If the line is very thin, simply modify the alpha to make it appear thinner. The actual algorith
		// should probably not be linear, but this works ok.
		if (_lineThickness < 1){
			alpha = (uint8_t)(alpha * _lineThickness + 0.5);
			if (alpha==0) return;
		}
		
		// Different tact for lines > 45 degrees (steep)
		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;
		uint32_t index;
		if (abs(dx) > abs(dy)){
			// Steep
			if ( p2.x < p1.x ) swap(p1, p2);

			// Gradient of line rise/run
			float gradient = dy / dx;
			
			// Pixel pair at end
			float xs = (int16_t)(p2.x + 0.5);
			float ys = p2.y + gradient * (xs - p2.x);
			float gap = (p2.x+0.5) - xs;
			float f = ys - (int16_t)ys;

			index = _framebuffer->index( xs, ys );
			_framebuffer->data.data16[index] = alphaBlendPrepared5565(
				_lineColor->cPre,
				_framebuffer->data.data16[index],
				( alpha * (1 - f) * gap )
			);
			index += _framebuffer->width;
			_framebuffer->data.data16[index] = alphaBlendPrepared5565(
				_lineColor->cPre,
				_framebuffer->data.data16[index],
				( alpha * f * gap )
			);
			float xe = xs;
			
			// Pixel pair at the start
			xs = (uint32_t)(p1.x+0.5);
			ys = p1.y + gradient * (xs - p1.x);
			gap = 1 - ((p1.x + 0.5) - xs);
			f = ys - (uint32_t)ys;

			index = _framebuffer->index( xs, ys );
			_framebuffer->data.data16[index] = alphaBlendPrepared5565(
				_lineColor->cPre,
				_framebuffer->data.data16[index],
				( alpha * (1 - f) * gap )
			);
			index += _framebuffer->width;
			_framebuffer->data.data16[index] = alphaBlendPrepared5565(
				_lineColor->cPre,
				_framebuffer->data.data16[index],
				( alpha * f * gap )
			);
			float y = ys + gradient;
			
			// Pixel pairs in between ends
			for( uint32_t x = xs+1; x < xe; x++) {
				f = y - (uint32_t)y;
				index = _framebuffer->index(x,y);		
				_framebuffer->data.data16[index] = alphaBlendPrepared5565(
					_lineColor->cPre,
					_framebuffer->data.data16[index],
					( alpha * (1 - f) )
				);
				index += _framebuffer->width;
				_framebuffer->data.data16[index] = alphaBlendPrepared5565(
					_lineColor->cPre,
					_framebuffer->data.data16[index],
					( alpha * f )
				);
				y += gradient;
			}
			
		}
		else{
			// Steep
			if ( p2.y < p1.y ) swap(p1, p2);
			
			// Gradient flipped
			float gradient = dx / dy;
			
			// Pixel pair at end
			float ys = (uint32_t)(p2.y + 0.5);
			float xs = p2.x + gradient * (ys - p2.y);
			float gap = (p2.y+0.5) - ys;
			float f = xs - (uint32_t)xs;

			index = _framebuffer->index( xs, ys );
			_framebuffer->data.data16[index] = alphaBlendPrepared5565(
				_lineColor->cPre,
				_framebuffer->data.data16[index],
				( alpha * (1 - f) * gap )
			);
			index++;
			_framebuffer->data.data16[index] = alphaBlendPrepared5565(
				_lineColor->cPre,
				_framebuffer->data.data16[index],
				( alpha * (f * gap) )
			);
			float ye = ys;
			
			// Pixel pair at the start
			ys = (uint32_t)(p1.y+0.5);
			xs = p1.x + gradient * (ys - p1.y);
			gap = 1 - ((p1.y + 0.5) - ys);
			f = xs - (uint32_t)xs;

			index = _framebuffer->index( xs, ys );
			_framebuffer->data.data16[index] = alphaBlendPrepared5565(
				_lineColor->cPre,
				_framebuffer->data.data16[index],
				( alpha * (1 - f) * gap )
			);
			index++;
			_framebuffer->data.data16[index] = alphaBlendPrepared5565(
				_lineColor->cPre,
				_framebuffer->data.data16[index],
				( alpha * f * gap )
			);
			float x = xs + gradient;
			
			// Pixel pairs in between ends
			for( uint32_t y = ys+1; y < ye; y++) {
				f = x - (uint32_t)x;
				index = _framebuffer->index( x, y );
				_framebuffer->data.data16[index] = alphaBlendPrepared5565(
					_lineColor->cPre,
					_framebuffer->data.data16[index],
					( alpha * (1 - f) )
				);
				index++;
				_framebuffer->data.data16[index] = alphaBlendPrepared5565(
					_lineColor->cPre,
					_framebuffer->data.data16[index],
					( alpha * f )
				);
				x += gradient;
			}
		}
	}

	/**
	 * Draw a single horizontal line of a span
	 */
	void GraphicsVectorExtn16::_scanline( uint32_t cy, SpanEdge* left, SpanEdge* right, float coverage ){
		// Final alpha based on coverage and fill alpha
		uint8_t alpha = (uint8_t)(_fillColor->a5 * coverage + 0.5);
//Serial.printf("SCANLINE\nCoverage:%f\n", coverage);

		// Setup left
		left->cx = (int16_t)left->x;
		left->i = _framebuffer->index( left->cx, cy );
		left->solid = false;

		// Pixel coverage
//Serial.printf(" cx:%d x:%f", left->cx, left->x);
/*
Serial.printf("Setting up left edge\n");
Serial.printf("  X: %f\n",left->x);
Serial.printf("  CX: %d\n",left->cx);
Serial.printf("  g: %f\n",left->g);
Serial.printf("  ga: %f\n",left->ga);
Serial.printf("  gi: %f\n",left->gi);
Serial.printf("  gia: %f\n",left->gia);
Serial.printf("  Steep: %d\n",(left->steep)?1:0);
Serial.printf("  Rect: %f\n",(left->cx+1-left->x));
Serial.printf("  Tri: %f\n",(left->gia*0.5));
*/
		if (left->steep) left->pc = (left->cx+1-left->x) - left->gia*0.5;
		else left->pc = (left->cx+1-left->x)*left->ga*0.5;
//Serial.printf("  Cover: %f\n",left->pc);
//Serial.printf(" pc:%f", left->pc);
		if (left->pc >= 1){
			left->pc = 1;
			left->solid = true;
		}
//Serial.printf(" pc:%f\n", left->pc);

		// Setup right
		right->cx = (int16_t)right->x;
		right->i = _framebuffer->index( right->cx, cy );
		right->solid = false;

		// Pixel coverage
//Serial.printf(" cx:%d x:%f", right->cx, right->x);
/*
Serial.printf("Setting up right edge\n");
Serial.printf("  Steep: %d\n",(right->steep)?1:0);
Serial.printf("  Rect: %f\n",(right->x-right->cx));
Serial.printf("  Tri: %f\n",(right->gia*0.5));
*/
		if (right->steep) right->pc = (right->x-right->cx) - right->gia*0.5;
		else right->pc = (right->x-right->cx)*right->ga*0.5;
//Serial.printf("  Cover: %f\n",right->pc);
//Serial.printf(" pc:%f", right->pc);
		if (right->pc >= 1){
			right->pc = 1;
			right->solid = true;
		}
//Serial.printf(" pc:%f\n", right->pc);

		// Clipping
		if (left->cx < 0){
			left->pc -= left->ga*left->cx;
			if (left->pc >= 1){
				left->solid = true;
				left->pc = 1;
			}
			left->i -= left->cx;
			left->cx = 0;
		}
		if (right->cx >= _framebuffer->width){
			uint16_t d = right->cx - _framebuffer->width + 1;
			right->pc += right->ga*d;
			if (right->pc >= 1){
				right->solid = true;
				right->pc = 1;
			}
			right->i -= d;
			right->cx -= d;
		}
	
		// Render two points at a time, one from left, one from right, and converge.
		while (true){
			// Overlapped. We are done
			if (left->i > right->i) break;

			// Combined
			if (left->i==right->i){
				left->pc = (left->pc + right->pc)*0.5; // Average
				_framebuffer->data.data16[left->i] = alphaBlendPrepared5565(
					_fillColor->cPre,
					_framebuffer->data.data16[left->i],
					(uint8_t)(alpha * left->pc)
				);
				break;
			}

			// Left
			if (left->solid){
				_framebuffer->data.data16[left->i] = alphaBlendPrepared5565(
					_fillColor->cPre,
					_framebuffer->data.data16[left->i],
					alpha
				);
			}
			else{
				_framebuffer->data.data16[left->i] = alphaBlendPrepared5565(
					_fillColor->cPre,
					_framebuffer->data.data16[left->i],
					(uint8_t)(alpha * left->pc)
				);

				left->pc += left->ga;
				if (left->pc >= 1){
					left->solid = true;
					left->pc = 1;
				}
			}

			// Right
			if (right->solid){
				_framebuffer->data.data16[right->i] = alphaBlendPrepared5565(
					_fillColor->cPre,
					_framebuffer->data.data16[right->i],
					alpha
				);
			}
			else{
				_framebuffer->data.data16[right->i] = alphaBlendPrepared5565(
					_fillColor->cPre,
					_framebuffer->data.data16[right->i],
					(uint8_t)(alpha * right->pc)
				);

				right->pc += right->ga;
				if (right->pc >= 1){
					right->solid = true;
					right->pc = 1;
				}
			}

			left->i++;
			left->cx++;
			right->i--;
			right->cx--;
		}
	}
	
}