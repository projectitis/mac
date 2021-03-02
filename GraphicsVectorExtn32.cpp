/**
 * Graphics library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "GraphicsVectorExtn32.h"

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
	 * @param	x1		X Position of start of line
	 * @param	y1		Y position of start of line
	 * @param	x2		X Position of end of line
	 * @param	y2		Y position of end of line
	 **/
	void GraphicsVectorExtn32::_line( VertexF p1, VertexF p2 ){
		// 8-bit alpha (0-255)
		uint8_t alpha = _lineColor->a8;

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
			// Not steep
			if (p2.x < p1.x) swap(p1, p2);
			
			// Gradient of line rise/run
			float gradient = dy / dx;
			
			// Pixel pair at end
			float xs = (int16_t)(p2.x + 0.5);
			float ys = p2.y + gradient * (xs - p2.x);
			float gap = (p2.x+0.5) - xs;
			float f = ys - (int16_t)ys;

			index = (uint32_t)ys * _framebuffer->width * (uint32_t)xs;
			_framebuffer->data.data32[index] = alphaBlendPrepared8888(
				_lineColor->cRB,
				_lineColor->cG,
				_framebuffer->data.data32[index],
				( alpha * (1 - f) * gap )
			);
			index += _framebuffer->width;
			_framebuffer->data.data32[index] = alphaBlendPrepared8888(
				_lineColor->cRB,
				_lineColor->cG,
				_framebuffer->data.data32[index],
				( alpha * f * gap )
			);
			float xe = xs;
			
			// Pixel pair at the start
			xs = (uint32_t)(p1.x+0.5);
			ys = p1.y + gradient * (xs - p1.x);
			gap = 1 - ((p1.x + 0.5) - xs);
			f = ys - (uint32_t)ys;

			index = (uint32_t)ys * _framebuffer->width * (uint32_t)xs;
			_framebuffer->data.data32[index] = alphaBlendPrepared8888(
				_lineColor->cRB,
				_lineColor->cG,
				_framebuffer->data.data32[index],
				( alpha * (1 - f) * gap )
			);
			index += _framebuffer->width;
			_framebuffer->data.data32[index] = alphaBlendPrepared8888(
				_lineColor->cRB,
				_lineColor->cG,
				_framebuffer->data.data32[index],
				( alpha * f * gap )
			);
			float y = ys + gradient;
			
			// Pixel pairs in between ends
			for( uint32_t x = xs+1; x < xe; x++) {
				f = y - (uint32_t)y;
				index = (uint32_t)y * _framebuffer->width * x;
				_framebuffer->data.data32[index] = alphaBlendPrepared8888(
					_lineColor->cRB,
					_lineColor->cG,
					_framebuffer->data.data32[index],
					( alpha * (1 - f) )
				);
				index += _framebuffer->width;
				_framebuffer->data.data32[index] = alphaBlendPrepared8888(
					_lineColor->cRB,
					_lineColor->cG,
					_framebuffer->data.data32[index],
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

			index = (uint32_t)ys * _framebuffer->width * (uint32_t)xs;
			_framebuffer->data.data32[index] = alphaBlendPrepared8888(
				_lineColor->cRB,
				_lineColor->cG,
				_framebuffer->data.data32[index],
				( alpha * (1 - f) * gap )
			);
			index++;
			_framebuffer->data.data32[index] = alphaBlendPrepared8888(
				_lineColor->cRB,
				_lineColor->cG,
				_framebuffer->data.data32[index],
				( alpha * (f * gap) )
			);
			float ye = ys;
			
			// Pixel pair at the start
			ys = (uint32_t)(p1.y+0.5);
			xs = p1.x + gradient * (ys - p1.y);
			gap = 1 - ((p1.y + 0.5) - ys);
			f = xs - (uint32_t)xs;

			index = (uint32_t)ys * _framebuffer->width * (uint32_t)xs;
			_framebuffer->data.data32[index] = alphaBlendPrepared8888(
				_lineColor->cRB,
				_lineColor->cG,
				_framebuffer->data.data32[index],
				( alpha * (1 - f) * gap )
			);
			index++;
			_framebuffer->data.data32[index] = alphaBlendPrepared8888(
				_lineColor->cRB,
				_lineColor->cG,
				_framebuffer->data.data32[index],
				( alpha * f * gap )
			);
			float x = xs + gradient;
			
			// Pixel pairs in between ends
			for( uint32_t y = ys+1; y < ye; y++) {
				f = x - (uint32_t)x;
				index = y * _framebuffer->width * (uint32_t)x;
				_framebuffer->data.data32[index] = alphaBlendPrepared8888(
					_lineColor->cRB,
					_lineColor->cG,
					_framebuffer->data.data32[index],
					( alpha * (1 - f) )
				);
				index++;
				_framebuffer->data.data32[index] = alphaBlendPrepared8888(
					_lineColor->cRB,
					_lineColor->cG,
					_framebuffer->data.data32[index],
					( alpha * f )
				);
				x += gradient;
			}
		}
	}

	/**
	 * Draw a single horizontal line of a span
	 */
	void GraphicsVectorExtn32::_scanline( uint32_t cy, SpanEdge* left, SpanEdge* right, float coverage ){

		uint8_t alpha = (uint8_t)(_fillColor->a8 * coverage + 0.5);

		// Setup left
		left->cx = (int16_t)left->x;
		left->i = cy * _framebuffer->width + left->cx; // Framebuffer index
		left->solid = false;

		// Pixel coverage
		if (left->steep) left->pc = (left->cx+1-left->x) - left->gi*0.5;
		else left->pc = (left->cx+1-left->x)*left->ga*0.5;

		// Setup right
		right->cx = (int16_t)right->x;
		right->i = cy * _framebuffer->width + right->cx; // Framebuffer index
		right->solid = false;

		// Pixel coverage
		if (right->steep) right->pc = (right->x-right->cx) + right->gi*0.5;
		else right->pc = (right->x-right->cx)*right->ga*0.5;

		// CLipping
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
				_framebuffer->data.data32[left->i] = alphaBlendPrepared8888(
					_fillColor->cRB,
					_fillColor->cG,
					_framebuffer->data.data32[left->i],
					(uint8_t)(alpha * left->pc)
				);
				break;
			}

			if (left->solid){
				_framebuffer->data.data32[left->i] = alphaBlendPrepared8888(
					_fillColor->cRB,
					_fillColor->cG,
					_framebuffer->data.data32[left->i],
					alpha
				);
			}
			else{
				_framebuffer->data.data32[left->i] = alphaBlendPrepared8888(
					_fillColor->cRB,
					_fillColor->cG,
					_framebuffer->data.data32[left->i],
					(uint8_t)(alpha * left->pc)
				);

				left->pc += left->ga;
				if (left->pc >= 1){
					left->solid = true;
					left->pc = 1;
				}
			}
			if (right->solid){
				_framebuffer->data.data32[right->i] = alphaBlendPrepared8888(
					_fillColor->cRB,
					_fillColor->cG,
					_framebuffer->data.data32[right->i],
					alpha
				);
			}
			else{
				_framebuffer->data.data32[right->i] = alphaBlendPrepared8888(
					_fillColor->cRB,
					_fillColor->cG,
					_framebuffer->data.data32[right->i],
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