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
		_agg2D = new Agg2D();
	}
	
	/**
	 * Destructor
	 **/
	GraphicsVectorExtn::~GraphicsVectorExtn(){
		delete _agg2D;
	}
	
	/**
	 * Called by the Graphics object internally. Override base method.
	 * @param	graphics	The graphics object passes itself as a reference
	 **/
	void GraphicsVectorExtn::init( BufferRect* framebuffer ){
		GraphicsExtension::init( framebuffer );
		
		// Attach the framebuffer
		_agg2D->attach(
			(uint8_t*)_framebuffer->buffer,		// Start address
			_framebuffer->w,					// Width
			_framebuffer->h,					// Height
			_framebuffer->w * 2					// Stride (in bytes)
		);

		// Define the viewport as the entire display
		_agg2D->viewport(
			0, 0, _framebuffer->w, _framebuffer->h,		// World coords
			0, 0, _framebuffer->w, _framebuffer->h,		// Display coords
			Agg2D::XMinYMin								// 0 is top left
		);
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
		color color,
		alpha alpha,
		float thickness,
		capStyle cap,
		joinStyle join
	){
		// Color and alpha
		uint8_t r,g,b;
		CONV_RGB888( color );
		//_agg2D->lineColor(r, g, b, alpha);
		_agg2D->lineColor(255, 255, 255, 255);
		
		// Line cap
		switch (cap){
			case CAP_BUTT:
				_agg2D->lineCap( Agg2D::CapButt );
				break;
			case CAP_SQUARE:
				_agg2D->lineCap( Agg2D::CapSquare );
				break;
			case CAP_ROUND:
			default:
				_agg2D->lineCap( Agg2D::CapRound );
				break;
		}
		
		// Line join
		switch (join){
			case JOIN_MITER:
				_agg2D->lineJoin( Agg2D::JoinMiter );
				break;
			case JOIN_BEVEL:
				_agg2D->lineJoin( Agg2D::JoinBevel );
				break;
			case JOIN_ROUND:
			default:
				_agg2D->lineJoin( Agg2D::JoinRound );
				break;
		}
		
		// Thickness
		_agg2D->lineWidth( thickness );
	}
	
	/**
	 * Set drawing operations to draw without line
	 **/
	void GraphicsVectorExtn::noLine(){
		_agg2D->noLine();
	}
	
	/**
	 * Set the fill style for subsequent drawing calls
	 * @param	color	Color in RGB565 format
	 * @param	alpha	Alpha from 0 to 1
	 **/
	void GraphicsVectorExtn::fillStyle(
		color color,
		alpha alpha
	){
		// Color and alpha
		uint8_t r,g,b;
		CONV_RGB888( color );
		_agg2D->fillColor(r, g, b, alpha);
	}
	
	/**
	 * Set drawing operations to draw without fill
	 **/
	void GraphicsVectorExtn::noFill(){
		_agg2D->noFill();
	}
	
	/**
	 * Draw a line from one point to another.
	 * @param	x1		X Position of start of line
	 * @param	y1		Y position of start of line
	 * @param	x2		X Position of end of line
	 * @param	y2		Y position of end of line
	 **/
	void GraphicsVectorExtn::line(
		float x1,
		float y1,
		float x2,
		float y2
	){
		_agg2D->line( x1, y1, x2, y2 );
		_cursorX = x2;
		_cursorY = y2;
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
		_agg2D->line( _cursorX, _cursorY, x, y );
		_cursorX = x;
		_cursorY = y;
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
	
}