/**
 * Display library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Graphics.h"

#define fastFloorU(x) ((uint16_t)(x))
#define fastFloorF(x) ((float)(uint16_t)(x))
#define fastCeilU(x) (((uint16_t)((x)+1))
#define fastCeilF(x) ((float)(uint16_t)((x)+1))
#define fpart(x) ((x)-fastFloorF(x))
#define ipart(x) (fastFloorF(x))

/**
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Constructor.
	 * Pass in a TFT Print instance (e.g. ILI8431_t3 or ADA_GUI) which is used for
	 * all underlying graphic calls. the GUI is actually the root display object,
	 * which is empty. Add display objects to this one to render them.
	 * @param	display			A Display instance for the hardware display being used
	 **/
	Graphics::Graphics( Display* display ){
		// Display
		_display = display;
		
		// Allocate buffer
		_framebuffer.x = 0;
		_framebuffer.y = 0;
		_framebuffer.w = _display->width;
		_framebuffer.h = _display->height;
		_framebuffer.stride = _display->width;
		_framebuffer.buffer = new uint16_t[ _framebuffer.w * _framebuffer.h ];
		_framebuffer.start = _framebuffer.buffer;
		
		// Buffer rect
		_bufferRect[0] = new BufferRect();
		_bufferRect[0]->stride = _framebuffer.w;
		_bufferRect[1] = new BufferRect();
		_bufferRect[1]->stride = _framebuffer.w;
		_resetBufferRect( 0 );
		_resetBufferRect( 1 );
		_bufferRectIndex = 0;
		
		// Extensions
		bitmap = new GraphicsBitmapExtn();
		bitmap->init( &_framebuffer );
		vector = new GraphicsVectorExtn();
		vector->init( &_framebuffer );
	}
	
	/**
	 * Destructor
	 **/
	Graphics::~Graphics(){
		delete[] _framebuffer.buffer;
	}
	
	/**
	 * Update the framebuffer to the display
	 * @param	useBufferRect	If true, will update only the portion of the framebuffer that
	 *							has changed since the last call to update.
	 **/
	void Graphics::update( boolean useBufferRect ){
		if (useBufferRect){
			_display->update(
				_getBuffer(),
				_getBufferRect(),
				false
			);
		}
		else{
			_display->update(
				_getBuffer(),
				NULL,
				false
			);
		}
	}
	
	/**
	 * Grab the address of the buffer and swap the buffer rect so that it is preserved
	 * during transfer to the display. This is done during update. All other drawing
	 * commands that access the framebuffer should do so using _framebuffer directly.
	 **/
	uint16_t* Graphics::_getBuffer( void ){
		// Prepare BufferRect by calculating the start address of the changed area, and then swap the
		// rects to preserve them (i.e. so that the rect is not changed by draw calls while the user is
		// reading it for the buffer transfer).
		if ((_bufferRect[_bufferRectIndex]->w>0) && (_bufferRect[_bufferRectIndex]->h>0)){
			_bufferRect[_bufferRectIndex]->start = _framebuffer.buffer
				+ _bufferRect[_bufferRectIndex]->y * _bufferRect[_bufferRectIndex]->stride
				+ _bufferRect[_bufferRectIndex]->x;
		}
		_bufferRectIndex = _bufferRectIndex ^ 1;
		_resetBufferRect( _bufferRectIndex );
		
		// Return the buffer
		return _framebuffer.buffer;
	}
	
	/**
	 * Returns pointer to structure that defines the area of the framebuffer that has
	 * changed since the last call to getBuffer.
	 **/
	BufferRect* Graphics::_getBufferRect( void ){
		// Return the non-active buffer rect
		return _bufferRect[ _bufferRectIndex ^ 1 ];
	}
	
	/**
	 * Reset the BufferRect back to default. The default has w and h of 0, and x and y at
	 * their maximum values of framebuffer width and height respectively. Always check that
	 * the w and h are > 0 before applying the rect.
	 **/
	void Graphics::_resetBufferRect( uint8_t index ){
		_bufferRect[index]->x = _framebuffer.w;
		_bufferRect[index]->y = _framebuffer.h;
		_bufferRect[index]->w = 0;
		_bufferRect[index]->h = 0;
	}
	
	/**
	 * Copy an area of the buffer described by rect (the src) to the destination buffer.
	 * @param	dst		The destination buffer
	 * @param	rect	A BufferRrect which describes the src
	 **/
	void Graphics::_copyArea( BufferRect* src, uint16_t* dst ){
		if ((src->w==0) || (src->h==0)) return;
		
		// Set up src and dst addresses
		_copyAreaSrc = src->start;
		_copyAreaDst = dst + src->y*src->stride + src->x;
		_copyAreaIter = src->h;
		
		// Copy line by line
		while (_copyAreaIter>0){
			memcpy( _copyAreaDst, _copyAreaSrc, src->w*2 );
			_copyAreaSrc += src->stride;
			_copyAreaDst += src->stride;
			_copyAreaIter--;
		}
	}
	
	/**
	 * Fill the framebuffer with a color
	 **/
	void Graphics::clear( color color ){
		uint16_t* dst_p = _framebuffer.buffer;
		uint32_t pixels = _framebuffer.w * _framebuffer.h;
		while (pixels--){
			*(dst_p++) = color;
		}
	}
	
} // namespace